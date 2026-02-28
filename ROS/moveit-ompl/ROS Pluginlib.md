
💡是什么？
	它就像 **“USB 接口”** 或者 **“手机 App”**
    ROS对OMPL预留的接口，基于基本的类实现，我们进行多态的构建

🚀实现这个插件，必须包含以下五点：

| **步骤**    | **文件位置**      | **动作 (Action)** | **代码/关键点**                                                  |
| --------- | ------------- | --------------- | ----------------------------------------------------------- |
| **1. 基类** | `.h`          | 定义接口            | 纯虚函数，如 `virtual void plan() = 0;`                           |
| **2. 子类** | `.h / .cpp`   | 具体实现            | `class MyPlanner : public BasePlanner`                      |
| **3. 注册** | `.cpp` (末尾)   | **打标签** (最容易忘)  | `PLUGINLIB_EXPORT_CLASS(MyPlanner, BasePlanner)`            |
| **4. 描述** | `plugin.xml`  | 写身份证            | 告诉系统：我的库文件在哪，我是谁，我的基类是谁。                                    |
| **5. 导出** | `package.xml` | 办签证             | `<export><my_pkg plugin="${prefix}/plugin.xml" /></export>` |

📝以我们的自定义的算法(`LERP`)为例，加入到OMPL库中，通过moveit来调用

`CLASS_LOADER_REGISTER_CLASS(lerp_interface::LERPPlannerManager, planning_interface::PlannerManager);`

MoveIt 的规划接口设计采用了**工厂模式**。主要有两种角色：经理(Manager)和厨师(Context)

- [[PlannerManager]](餐厅经理)：不负责具体算法的实现，只负责接客与配置。
	- **基类**：`planning_interface::PlannerManager`
	- **MoveIt 会问它**：“你能处理在这个机器人上做规划吗？”“请给我分配一个规划器实例。”
	- **核心功能**：`initialize`(初始化)和`getPlanningContext`(获取规划上下文)
	
- [[PlanningContext]] (主厨/具体算法)：**真正干活**的地方，具体算法的实现。
	- **基类**：`planning_interface::PlanningContext`
	- 按照OMPL中的算法的结构实现相关的算法
	- **流程**：`PlannerManager` 会 `new` 一个 `PlanningContext`，然后调用 `context->solve()`

- [[PlanningInterface]] (副经理 / 资源统筹)：持有着所有重要的资源（比如机器人模型 `RobotModel`，参数配置等）。
	- 协调者和枢纽者：链接moveit（接收请求）和OMPL（配置信息）的枢纽
	- 根据moveit的请求(`MotionPlanRequest`)，去构建实际的算法的执行者

- [[PlanningContextManager]](车间主任 / 工厂)：创建各种不同的规划器（RRT, EST, KPIECE）
	- **仓库管理**，负责高效存取规划器实例

🥊 其实可以发现这四个是链式串联的结构

[[PlannerManager]] 负责和moveit进行交互，具体的任务会交给 [[PlanningInterface]]  去完成， [[PlanningInterface]] 负责根据任务的需求去组件项目组的成员，项目组的成员