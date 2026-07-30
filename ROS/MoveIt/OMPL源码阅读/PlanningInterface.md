# PlanningInterface：OMPL 配置与上下文协调

## 结论

`PlanningInterface` 是 OMPL 与 MoveIt 之间的协调层：它读取规划器配置，持有机器人模型和约束采样器，并通过 `PlanningContextManager` 提供可执行的规划上下文。它不等同于某个 RRT 算法实现。

## 算法对象管理

🚀一个管家的身份，根据参数服务器的信息及机器人默认来选择构造算法对象还是复制对象
💡 主要职责是**管理配置**和**构建规划上下文**。它不直接执行规划算法，而是根据当前的场景和请求，配置并生成一个 `ModelBasedPlanningContext`对象，由该对象负责实际的路径规划。

构造函数：
- 输入参数：模型指针、节点句柄、(规划器配置)
- 对应两种构造方式, 区别在于如何获取规划器的配置
- 一种通过**参数服务器**来构造规划器`loadPlannerConfigurations()`，一种通过**代码传参**实现借助于`PlannerConfigurationMap`
- 第一种属于标准用法


核心成员变量

- `PlanningContextManager context_manager_`：
	- 最为核心的成员变量 planning context 包工头，管理planning context的生成
- `constraint_samplers::ConstraintSamplerManagerPtr constraint_sampler_manager_`
	- 负责“约束采样”的专家团队，负责加载特定的插件，专门生成满足姿态约束的路径点。
- `moveit::core::RobotModelConstPtr robot_model_`
- `ros::NodeHandle nh_`
- 状态标志位
	- `use_constraints_approximations_` 是否允许使用预先计算好的近似约束数据库来加速求解
	- `simplify_solutions_` 决定了规划完成后，要不要用 OMPL 的路径平滑工具

核心函数

`loadPlannerConfigurations()`函数：职责**读取 ROS 参数服务器上的规划器配置**，构造`PlannerConfigurationMap`数据
`setPlannerConfigurations()`函数：**加载和初始化 OMPL 规划器的配置参数**，并确保机器人模型中的每一个“规划组”（Planning Group）都至少拥有一个默认的规划配置



我们在 [[Planning Pipeline]] 介绍了可以通过设置`ompl_planning.yaml`设置规划器的配置参数，这里我们简单设置一个规划器的`yaml`配置文件

```yaml
# ompl_planning.yaml 
arm: 
  default_planner_config: RRTConnect
  planner_configs: 
    - RRTConnect 
    - PRMstar 
  projection_evaluator: joints(shoulder_joint,upperArm_joint)
  longest_valid_segment_fraction: 0.005
  
planner_configs: 
  RRTConnect: 
    type: geometric::RRTConnect 
    range: 0.05 
  PRMstar: 
    type: geometric::PRMstar 
    goal_bias: 0.05
```

需要的数据类型
1. `XmlRpc::XmlRpcValue`，从参数服务器上读取的任何 `YAML` 数据结构, 可以是数组或者是MAP
2. `std::map<std::string, std::string> specific_group_params` 规划器一些通用参数设置
	 🧬 <`group/params`, `string/bool/double/int -> string`>
	 💡 例如：`{"arm/projection_evaluator": "joints(shoulder_joint,upperArm_joint)",  
	         `"arm/longest_valid_segment_fraction": "0.005"}`
3.  `planning_interface::PlannerConfigurationSettings` 经过解析和处理的、单个规划器的配置清单
	 🧬 <`string: planner_name, string: group, map<string, string>: config`>
	 💡 例如：`{ name: "arm[RRTConnect]", 
		    `group: "arm",   
		   `config: {{"projection_evaluator", "joints(shoulder_joint, upperArm_joint)"}, 
		   `{"type", "geometric::RRTConnect"}, {"range", "0.15"}},
		   `{"longest_valid_segment_fraction", "0.005"}}`
4.  `planning_interface::PlannerConfigurationMap` `PlannerConfigurationSettings的map`，其name作为key
	🧬 <`string: config_name, planning_interface::PlannerConfigurationSettings: setting`>
    💡 例如：
```cpp
// planning_interface::PlannerConfigurationMap pconfig; 
{ 
 "arm":  // Key 是组名 
 { 
   name: "panda_arm", 
   group: "panda_arm", 
   // 继承了组参数，并使用了硬编码的 RRTConnect 作为默认 
   config: { 
     type {"projection_evaluator", "joints(shoulder_joint,upperArm_joint)"}, {"type", "geometric::RRTConnect"},
     {"longest_valid_segment_fraction", "0.005"} }
 }, 
     
  // --- RRTConnect_custom 的配置 --- 
  "arm[RRTConnect]": 
  {
   name: "arm[RRTConnect]", 
   group: "arm", 
   config: {{"projection_evaluator","joints(shoulder_joint,upperArm_joint)"}, {"type","geometric::RRTConnect"},                 {"range","0.05"}, {"longest_valid_segment_fraction", "0.005"} }
   }, 
   
 // --- PRM_custom 的配置 --- 
  "arm[PRMstar]": {
   name: "arm[PRMstar]", 
   group: "arm", 
   config: { {"projection_evaluator", "joints(shoulder_joint,upperArm_joint)"}, {"type", "geometric::PRMstar"},                 {"goal_bias", "0.05"}, {"longest_valid_segment_fraction", "0.005"} } 
  }
```
⚠️ 需要注意的是 planner name会被程序进行处理，处理方法是`group name + "[" + planner name + "]"`
 例如`arm[RRTConnect]` 、`arm[PRMstar]`

执行过程

1. 遍历规划组（因为可能有很多的规划组例如夹爪组、双臂、底盘等等）
2. 读取特别的参数指定`(projection_evaluator`, `longest_valid_segment_fraction, enforce_joint_model_state_space)`
3. 读取规划器对应的默认规划器(没有的话, 默认`RRTConnect`) 设置默认规划器的 `PlannerConfigurationSettings`
4. 遍历规划组下planner_configs中的规划器，设置规划器的 `PlannerConfigurationSettings`
5. 3 和 4 中的配置都会加载到 `PlannerConfigurationMap` 中
6. 需要注意的是默认规划器settings的name是规划器名称，规划器的名称是 `规划组名称[规划器名称]`


`setPlannerConfigurations`函数就是一个检查的作用，他会对前面`loadPlannerConfigurations()`函数返回的`PlannerConfigurationMap`参数和当前的规划组进行检查，为没有配置规划器的规划组配置空配置。
保证后续调用规划器时，系统不会因为找不到配置名而直接崩溃。
步骤比较简单：
- 赋值配置参数
- 对规划组遍历，查看配置参数中是否存在，不存在为规划组设置空配置
- 调用`PlanningContextManager`成员变量，`setPlannerConfigurations`
