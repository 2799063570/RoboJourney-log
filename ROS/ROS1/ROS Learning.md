# ROS1 概览与 ROS2 迁移

## 结论

ROS1 的核心价值是把机器人系统拆成通过标准接口协作的节点。学习时先掌握节点、topic、service、parameter、TF 和 launch 的关系，再进入 MoveIt、驱动或规划器源码；ROS2 继承了这种系统分解方式，但构建、通信发现、QoS 和 C++ API 都不同。

## ROS1 图由什么组成

| 组件 | 作用 | 适合的数据 |
| --- | --- | --- |
| Node | 承担单一职责的进程 | 例如相机驱动、机械臂驱动、规划、可视化 |
| Topic | 发布/订阅的连续数据流 | 图像、点云、关节状态、目标位姿 |
| Service | 短时请求—响应 | 查询、切换模式、一次性配置 |
| Action | 可取消、可反馈的长任务 | 轨迹执行、导航、抓取 |
| Parameter | 启动配置和低频调参 | 限位、控制器名、规划器参数 |
| TF | 坐标系之间带时间戳的变换 | `base_link`、`tool0`、相机坐标系 |

在 ROS1 中，Master 负责名称注册和发布/订阅者匹配；连接建立后，普通 topic 数据由节点之间直接传输。不要把 Master 误解为所有数据的中转站。

## 机械臂的最小系统

```text
机械臂驱动 → /joint_states ─┐
TF / robot_state_publisher ─┼→ MoveIt / RViz → FollowJointTrajectory action → 控制器
规划场景 / 传感器数据 ───────┘
```

先验证 `/joint_states`、TF 和控制器 action，再排查 MoveIt。没有可信的机器人状态和坐标变换，规划结果不能直接用于执行。

## 从 ROS1 迁移到 ROS2 时要改什么

| 主题 | ROS1 | ROS2 |
| --- | --- | --- |
| 构建 | `catkin`、`catkin_make` | `ament_cmake`、`colcon build` |
| 启动 | `roslaunch` XML | `ros2 launch`，通常使用 Python launch |
| C++ | `roscpp`、`ros::NodeHandle` | `rclcpp`、`rclcpp::Node` |
| 发现 | Master 协调名称注册 | 基于中间件的分布式发现 |
| 通信策略 | 常用 TCPROS，队列参数较简单 | QoS 会影响发布/订阅是否兼容 |
| 坐标变换 | `tf` / `tf2` | `tf2_ros` |

迁移时不要只改命令名：需要重新检查参数声明、QoS、executor、launch 文件和依赖声明。通信细节见 [[../ROS2/通信协议-DDS-TCP-UDP]]。

## 建议学习顺序

1. [[ROS常用指令]]：能独立检查节点、话题、参数和 bag。
2. [[tf坐标变换]]：理解坐标树、时间戳与变换查询。
3. [[pluginlib插件机制]]：理解 MoveIt 等框架如何加载扩展。
4. [[aubo]]：把抽象概念对应到真实机械臂驱动。
5. [[../MoveIt/README|MoveIt]]：进入规划、执行与源码阅读。

## 常见误区

- “看到 topic 就说明数据可用”：还要检查频率、消息内容、时间戳和坐标系。
- “能在 RViz 动就能控制真机”：RViz 只验证可视化；执行还依赖控制器、限位和安全逻辑。
- “ROS2 只是 ROS1 的新命令”：两者的通信和构建模型不同，代码需要按版本重写。
