# move_group 节点

## 结论

`move_group` 是 MoveIt 的协调节点：它接收运动规划请求，维护机器人和场景状态，调用规划管道生成轨迹，并把可执行轨迹交给控制器。它本身通常不实现具体的 RRT、碰撞检测或硬件驱动，而是把这些能力组织起来。

## 一次规划请求的主流程

```text
客户端 / RViz
    → move_group
    → Planning Scene（机器人状态、碰撞物、约束）
    → Planning Pipeline（请求适配器 + 规划器插件）
    → 时间参数化 / 轨迹校验
    → Trajectory Execution Manager
    → ros_control 或机器人控制器
```

1. 客户端通过 `MoveGroupInterface`、动作或服务提交目标位姿、关节目标和约束。
2. `move_group` 从机器人状态监视器和传感器更新中取得当前状态与规划场景。
3. 规划管道先执行请求适配器，例如修正越界起点、检查起点碰撞，再调用 OMPL 等规划器插件。
4. 得到路径后，适配器可添加时间参数；系统检查轨迹是否满足约束。
5. 用户请求执行时，轨迹执行管理器把轨迹发送给配置好的控制器，并反馈执行结果。

## 关键组成与入口笔记

| 组件 | 职责 | 关联笔记 |
| --- | --- | --- |
| Planning Scene | 保存机器人模型、当前状态、碰撞环境与约束 | [[Move group|move_group 架构]] |
| Planning Pipeline | 串联适配器与规划器插件 | [[Planning Pipeline]] |
| PlannerManager | 读取配置并创建具体规划上下文 | [[PlannerManager]] |
| PlanningContext | 针对一次请求执行求解 | [[PlanningContext]] |
| Trajectory Execution | 向控制器发送轨迹、监视执行 | [[ROS/MoveIt/Trajectory Execution]] |
| Sensor Manager | 将传感器数据更新为碰撞环境 | [[../传感器功能与OctoMap|传感器与 OctoMap]] |

## 配置时优先检查

- `robot_description` 与 SRDF 是否正确加载，规划组名称是否和代码一致。
- `planning_plugin` 是否指向可加载的插件，如 `ompl_interface/OMPLPlanner`。
- `ompl_planning.yaml` 中规划组是否配置了可用的 `planner_configs`。
- 控制器名称、关节名和轨迹 action 是否与 MoveIt 配置一致。
- `joint_states` 是否持续更新；没有当前状态时，规划结果不可信。
- 规划场景中的碰撞物、允许碰撞矩阵和坐标系是否符合预期。

## 常见问题定位

| 现象 | 优先检查 |
| --- | --- |
| 找不到规划器插件 | 插件 XML、共享库、`planning_plugin` 和工作空间环境 |
| 一直提示起点无效/碰撞 | `joint_states`、关节限位、碰撞模型与起始姿态 |
| 能规划但不能执行 | 控制器配置、FollowJointTrajectory action、关节名称与时间戳 |
| RViz 中目标可见但规划失败 | planning group、末端执行器 link、TF 和目标约束 |

## 与 ROS2 的区别

本页的源码和配置语境是 ROS1 MoveIt。实际 ROS2 项目应使用 MoveIt 2 的 launch、参数与 `rclcpp` 接口，通常配合 `ros2_control`；不要直接照搬 ROS1 的 `roslaunch`、`NodeHandle`、`ros_control` 或参数服务器写法。

## 关联笔记

- [[Move group]]
- [[Planning Pipeline]]
- [[PlannerManager]]
- [[PlanningContext]]
- [[ROS/ROS1/pluginlib插件机制]]
