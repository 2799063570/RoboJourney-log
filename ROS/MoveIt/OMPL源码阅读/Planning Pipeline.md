# Planning Pipeline：请求适配器与规划器串联（ROS1 MoveIt）

## 结论

Planning Pipeline 接收一次 `MotionPlanRequest`，按配置运行请求适配器，再调用规划器插件，最后对结果进行时间参数化或有效性处理。适配器顺序会改变请求和返回轨迹，因此 YAML 配置本身是规划行为的一部分。

## 阅读提示

本页示例是 ROS1 launch/参数服务器写法。MoveIt2 的参数加载与 launch 形式不同，但“请求适配器 + 规划器插件”的职责划分仍然适用。

负责**配置 MoveIt! 的核心规划引擎——OMPL (Open Motion Planning Library)**。

💡告诉 MoveIt：“请使用 OMPL 算法来规划路径，并在规划前后做一些预处理和后处理工作。”

```xml
<launch>

  <!-- OMPL Plugin for MoveIt! 设置算法库的插件名称 -->
  <arg name="planning_plugin" value="ompl_interface/OMPLPlanner" />

  <!-- define capabilites that are loaded on start (space seperated) -->
  <arg name="capabilities" default=""/>

  <!-- inhibit capabilites (space seperated) -->
  <arg name="disable_capabilities" default=""/>

  <!-- The request adapters (plugins) used when planning with OMPL.
       ORDER MATTERS 请求适配器 前后处理的插件名称 -->
  <arg name="planning_adapters" value="default_planner_request_adapters/AddTimeParameterization
				       default_planner_request_adapters/FixWorkspaceBounds
				       default_planner_request_adapters/FixStartStateBounds
				       default_planner_request_adapters/FixStartStateCollision
				       default_planner_request_adapters/FixStartStatePathConstraints" />

  <arg name="start_state_max_bounds_error" value="0.1" /><!-- 误差容忍程度 -->

  <param name="planning_plugin" value="$(arg planning_plugin)" />
  <param name="request_adapters" value="$(arg planning_adapters)" />
  <param name="start_state_max_bounds_error" value="$(arg start_state_max_bounds_error)" />
  <param name="capabilities" value="$(arg capabilities)" />
  <param name="disable_capabilities" value="$(arg disable_capabilities)" />

  <rosparam command="load" file="$(find robot_moveit_config)/config/ompl_planning.yaml"/>

</launch>
```

可以发现通过该launch文件，向**ROS参数服务器**中设置了路径规划算法的相关参数

- **planning_plugin**(**规划器插件**)：算法库对应的插件名称
- **request_adapters**(**请求适配器链**)：预处理/后处理过滤器
- **start_state_max_bounds_error**(**误差容忍度**)：始状态的最大边界误差容忍度
- **capabilities**(**额外加载的功能插件**)：特殊的插件（比如“倒水专用规划器”或“视觉伺服控制”）
- **disable_capabilities**(**禁用的默认功能**)：MoveIt 就不会加载它，从而节省内存和 CPU

但是这只是一些相关的变量配置，那么应该如何实现具体的功能呢？
这里主要借助于**ROS 的 [[ROS pluginlib]] 机制** 和 **C++ 的多态（Polymorphism）特性**

---

🚀在上述的launch文件中，我们静态设置了算法库名称(通过加载`ompl_planning.yaml`得到`ompl_interface/OMPLPlanner`)
算法的指定主要分为两种方式：
- **静态**(设置`ompl_planning.yaml`)
	- 定义在moveit_config/config下
	- 定义了从算法ID到具体算法类的映射
- **动态**(程序中或者Rviz中)

📝 我们先来分析一下**静态**设置的`.yaml`文件
```yaml
planning_plugin: ompl_interface/OMPLPlanner # 插件名称
request_adapters: >-   # 预处理/后处理过滤器
	default_planner_request_adapters/AddTimeOptimalParameterization
	default_planner_request_adapters/FixWorkspaceBounds
	default_planner_request_adapters/FixStartStateBounds
	default_planner_request_adapters/FixStartStateCollision
	default_planner_request_adapters/FixStartStatePathConstraints
	
planner_configs:
	# === 这里定义了具体的算法 ===
    RRTConfigDefault:
	    type: geometric::RRT   # <-- 关键：指定了 C++ 类名
	    range: 0.0  # 步长参数
	RRTConnectkConfigDefault:
	    type: geometric::RRTConnect
	    range: 0.0
	# ... 其他算法 ...	
manipulator: # 你的规划组名称
	default_planner_config: RRTConnectkConfigDefault  # <-- 指定该组的默认算法
    planner_configs:
      - RRTkConfigDefault
      - RRTConnectkConfigDefault
      - ...
	projection_evaluator: joints(shoulder_joint,upperArm_joint) # <-- 投影 选择对空间位置影响最大的关节
	longest_valid_segment_fraction: 0.005 # <-- 最长有效段分数
```
 ⚠️  可见重点在于两部分`planner_configs`和`manipulator`， 分别指定了算法的配置和规划组所对应的算法。
 - `planner_configs`就是一个规划器配置池
	 - 文件中为每个算法设置了默认参数（很多都带有注释说明），开发者可以通过微调这些参数来优化规划效果
	 - `type`指定OMPL库中对应的算法类（例如 `geometric::RRTConnect`）
	 - `range`: 每次算法在空间中生长的最大步长。如果设为 `0.0`，通常意味着让OMPL在初始化时自动推断一个合适的值。
	- `goal_bias`: “目标偏好”。例如 `0.05` 表示算法在随机采样时，有 5% 的概率直接向着终点尝试迈进，这有助于加快寻路速度。
	- `num_samples` / `max_nearest_neighbors`: 决定了采样的密度和连接的邻居数量，通常影响路径的质量和计算时间。
 - `manipulator` 对应每个特定规划组配置
	 - `default_planner_config` 指出机器人
	 - `planner_configs` 规定了该规划组可以调用的算法
	 - `projection_evaluator` 高维向低维投影是参考的关节
	 - `longest_valid_segment_fraction` 路径碰撞检测的分辨率

📝 **动态**则可以通过**规划组来指定具体的算法**
```python
group = moveit_commander.MoveGroupCommander("manipulator")

# 明确指定算法 ID (必须是 yaml 里定义的那些名字)
group.set_planner_id("RRTConnectkConfigDefault") 
	
# 或者指定你的新算法
# group.set_planner_id("NeuralRRT") 
	
group.go()
```

```cpp
moveit::planning_interface::MoveGroupInterface group("manipulator");
group.setPlannerId("RRTstar"); // 如果 yaml 里有配 RRTstar 的话
```
⚠️  可见都是通过规划组设置规划算法借助于`set_planner_id`、 `setPlannerId`
同时也可以在Rviz中指定算法

---

🛡️ 在moveit程序初始化的过程中，会从参数服务器中读取这些名称，但是这个时候还只是字符串

🚀 最关键的一步：MoveIt 使用 `pluginlib::ClassLoader` 来根据名字找到对应的 C++ 类
- **机制**：`pluginlib` 利用 C++ 的 `RTTI`（运行时类型识别）和系统动态链接库（Linux 下的 `.so` 文件）。
- **过程**：
	- MoveIt 创建一个加载器，告诉它：“我要加载所有类型为 `planning_interface::PlannerManager` 的插件”
	- 加载器会去扫描 ROS 系统中所有注册的插件 XML 文件（[[plugin description]]）
	- 它发现字符串 `"ompl_interface/OMPLPlanner"` 对应于 `moveit_planners_ompl` 包里的某个编译好的 `.so` 库文件
	
	⚠️ 可以发现一条继承链路：`moveit_core`下`planning_interface::PlannerManager`  -> 各种库得manager(例 `OMPLPlanner`)
	- 当找到对应的库之后，加载器会将这个库加载到内存，并创建具体的对象实例（`ompl_interface/OMPLPlanner`）。
	
```CPP
// 1. 定义加载器，指定基类类型 (PlannerManager)
pluginlib::ClassLoader<planning_interface::PlannerManager> loader(
    "moveit_core", 
    "planning_interface::PlannerManager"
);

try {
    // 2. 关键时刻！利用字符串名字创建具体的对象实例
    // 这一步之后，planner_instance 就变成了一个活生生的 OMPL 对象
    boost::shared_ptr<planning_interface::PlannerManager> planner_instance = 
        loader.createInstance(planner_plugin_name);

    // 3. 初始化 (加载 ompl_planning.yaml 里的参数)
    planner_instance->initialize(robot_model, node_handle);

} catch(pluginlib::PluginlibException& ex) {
    ROS_ERROR("插件加载失败: %s", ex.what());
}
```

🚀 多态调用：不管是OMPL还是CHOMP, STOMP，都继承于同一个基类 [[PlannerManager]], [[PlanningContext]]并实现相同的虚函数

```cpp
// move_group 不关心底层是 OMPL 还是其他，它只管调用标准接口
planning_interface::MotionPlanResponse response;

// 这里的 context 包含了起点、终点等请求信息
// 实际上调用的是 OMPLPlanner::solve() 或者 CHOMPPlanner::solve()
planner_instance->getPlanningContext(planning_scene, request, error_code)->solve(response);
```

🚀规划适配器：对于那长长的一串适配器列表（如 `FixStartStateBounds` 等）
	MoveIt 使用的是**责任链模式 (Chain of Responsibility)**。
	
1. **读取列表**：读取 `planning_adapters` 参数，得到一个字符串数组。
2. **循环加载**：使用 `pluginlib` 循环实例化每一个适配器
3. **像串珠子一样连接**： MoveIt 会把这些适配器串联起来
	- 规划请求 -> [`FixBounds Adapter`] -> [`FixCollision Adapter`] -> [`OMPL Planner`] -> [T`imeParameterization Adapter`] -> 结果
	- 每一个适配器处理完数据后，传递给下一个，最后才传给真正的规划器（或从规划器返回）。
