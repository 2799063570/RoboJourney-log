# ROS 入口

这里存放 ROS1、ROS2、MoveIt、机械臂控制项目相关笔记。当前最重要的目标不是把所有概念看一遍，而是把 ROS2 控制链路跑通并能讲清楚。

## 当前主线

- [[机械臂ROS2控制项目计划]]
- [[机械臂控制项目体检与完善清单]]

当前阶段判断：

- 你已经有 ROS1 基础。
- ROS2 目前刚完成“功能包创建”的入门。
- 第 3 周和第 4 周计划需要降速加细，先补 ROS2 基础闭环，再做机械臂可视化与控制集成。

## 第 3 周优先级

本周不要直接追复杂功能，先完成这些最小闭环：

1. 创建 ROS2 C++ 功能包
2. 写 publisher / subscriber
3. 写 timer 控制节点
4. 使用 parameter 控制关节速度、时间步长、限幅
5. 写 launch 文件启动节点
6. 让 `vel_to_pos_node` 完成速度到位置的积分
7. 用命令行检查 topic、node、param、日志输出

验收标准：

- 能解释 ROS2 package、node、topic、message、parameter、launch 的关系。
- 能独立创建一个 `ament_cmake` C++ 包。
- 能写出一个最小 publisher/subscriber。
- 能用参数改变节点行为，而不是改代码重编译。

## 第 4 周优先级

第 4 周重点是把 ROS2 控制结果变成“看得见”的机械臂演示：

1. 写一个简单 2 自由度机械臂 URDF/Xacro
2. 发布 `joint_states`
3. 配置 `robot_state_publisher`
4. 在 RViz2 中显示机械臂
5. 加入 TF2 坐标关系
6. 将第 3 周的控制节点接到关节状态显示上
7. 整理项目 README 和演示脚本

验收标准：

- RViz2 能显示机械臂。
- 改变关节位置时，机械臂能动。
- 能解释 `joint_states`、URDF、TF2、RViz2 之间的关系。

## 知识地图

| 主题 | 笔记 |
|---|---|
| ROS2 通信 | [[ROS2/通信协议-DDS-TCP-UDP]] |
| TF 坐标 | [[基础概念/tf坐标变换]] |
| MoveIt 轨迹执行 | [[MoveIt/Trajectory Execution]] |
| pluginlib | [[pluginlib插件机制]] |
| MoveIt-OMPL 源码阅读 | [[moveit-ompl/Move group]], [[moveit-ompl/Planning Pipeline]], [[moveit-ompl/PlannerManager]] |
| ROS 学习旧资料 | [[ros_learning/ROS Learning]], [[ros_learning/ROS常用指令]] |

## 学习建议

- ROS1 经验可以迁移概念，但不要照搬写法。
- ROS2 先抓住 `ament_cmake`、`rclcpp`、参数、launch、QoS。
- 每天至少留下一个可运行命令和一个现象截图/记录。
- 所有项目笔记都要能回答三个问题：做了什么、怎么运行、为什么这样设计。

