---
created: 2026-06-04
tags:
  - ROS2
  - robot-arm
  - C++
  - learning-plan
status: active
---

# 机械臂 ROS2 控制项目计划

## 一句话目标

用 4 周做出一个可以写进简历的项目：

> 基于 ROS2 的机械臂速度-位置控制与轨迹平滑系统

最终不是只“学会语法”，而是形成一套能解释、能运行、能展示的工程：

1. 纯 C++ 机械臂控制基础库
2. ROS2 速度转位置控制节点
3. 简单机械臂 URDF / RViz2 / TF2 可视化
4. README、运行说明、面试讲解稿

---

## 当前策略

主线从 **4 周加速版** 升级为 **基础闭环 + 进阶扩展版**。

原因：

- 你已经会一些 C++ 和 ROS1。
- ROS2 功能包创建、基础节点学习进度比原计划更快。
- 当前只用约 1 周就进入第 3 周末，说明可以把项目从“ROS2 入门控制演示”升级为“机器人系统综合项目”。

新的策略不是简单加快，而是分层推进：

| 阶段 | 定位 | 目标 |
|---|---|---|
| 第 1-4 周 | 基础闭环 | ROS2 控制节点 + URDF + RViz2 能完整跑通 |
| 第 5-7 周 | 机械臂进阶 | MoveIt2 + ros2_control + trajectory controller |
| 第 8 周 | 移动机器人扩展 | Nav2 定位导航基础，理解移动机器人系统 |
| 第 9 周 | 视觉与深度学习扩展 | YOLO 经验迁移到 ROS2 感知链路，补 PyTorch / LibTorch |
| 第 10 周 | C++ 工程强化 | 现代 C++、数据结构算法、编码风格、项目重构 |

我们的原则是：

- 每天 2 到 3 小时
- 每天必须有一个可运行的小结果
- 每周必须有一个可展示的阶段成果
- 不追求一次写完美，先跑通，再重构，再解释清楚
- 进阶内容要服务项目展示，不做散乱学习
- 学过的知识必须能写进 README、简历和面试讲解

---

## 当前进度判断

当前真实状态：

- C++ 有一定基础，但还需要补现代 C++、工程风格、数据结构算法。
- ROS1 有基础，迁移 ROS2 时重点关注 `rclcpp`、参数、launch、QoS、生命周期和组件化。
- ROS2 基础推进较快，已经接近第 3 周末。
- 后续不应该只做 2 自由度 RViz 演示，而应加入 MoveIt2、ros2_control 和机器人系统扩展。

当前最合适的目标升级为：

> 做一个“ROS2 机械臂控制与规划综合项目”：包含自写控制节点、URDF/RViz2 可视化、MoveIt2 规划、ros2_control 控制器接口，并扩展理解移动机器人导航和视觉感知链路。

---

## 项目最终结构

```text
robot_arm_control_ros2_practice/
├── robot_control_cpp/
│   ├── CMakeLists.txt
│   ├── include/
│   │   ├── robot_state.hpp
│   │   ├── safety_limiter.hpp
│   │   ├── cubic_interpolator.hpp
│   │   ├── trajectory_buffer.hpp
│   │   └── jacobian_solver.hpp
│   ├── src/
│   │   ├── robot_state.cpp
│   │   ├── safety_limiter.cpp
│   │   ├── cubic_interpolator.cpp
│   │   ├── trajectory_buffer.cpp
│   │   └── jacobian_solver.cpp
│   └── examples/
│       ├── test_robot_state.cpp
│       ├── test_safety_limiter.cpp
│       ├── test_cubic_interpolator.cpp
│       ├── test_trajectory_buffer.cpp
│       ├── control_loop_demo.cpp
│       └── test_jacobian_solver.cpp
├── robot_control_ros2/
│   ├── src/
│   │   ├── joint_velocity_publisher.cpp
│   │   ├── joint_velocity_subscriber.cpp
│   │   ├── timer_control_node.cpp
│   │   └── vel_to_pos_node.cpp
│   ├── config/
│   │   └── vel_to_pos.yaml
│   ├── launch/
│   │   └── vel_to_pos.launch.py
│   └── CMakeLists.txt
├── simple_arm_description/
│   ├── urdf/
│   │   └── simple_2dof_arm.urdf.xacro
│   ├── launch/
│   │   └── display.launch.py
│   └── rviz/
│       └── simple_arm.rviz
├── simple_arm_moveit_config/
│   ├── config/
│   │   ├── joint_limits.yaml
│   │   ├── kinematics.yaml
│   │   ├── ompl_planning.yaml
│   │   └── ros2_controllers.yaml
│   ├── launch/
│   │   ├── demo.launch.py
│   │   └── move_group.launch.py
│   └── srdf/
│       └── simple_arm.srdf
├── simple_arm_bringup/
│   ├── launch/
│   │   ├── sim_control.launch.py
│   │   ├── moveit_control.launch.py
│   │   └── full_demo.launch.py
│   └── config/
│       └── controllers.yaml
├── robot_vision_learning/
│   ├── scripts/
│   │   ├── yolo_ros2_node.py
│   │   └── image_subscriber.py
│   └── notes/
│       ├── pytorch_review.md
│       └── libtorch_notes.md
├── mobile_robot_learning/
│   ├── notes/
│   │   ├── nav2_overview.md
│   │   ├── localization_amcl.md
│   │   └── slam_toolbox.md
│   └── launch/
│       └── nav2_demo_notes.md
└── README.md
```

---

## 第 1 周：纯 C++ 控制库

### 本周目标

完成 4 个核心模块：

- [ ] `RobotState`
- [ ] `SafetyLimiter`
- [ ] `CubicInterpolator`
- [ ] `TrajectoryBuffer`

本周重点不是 C++ 语法本身，而是：

- 模块接口怎么设计
- 类之间怎么组合
- 测试程序怎么写
- CMake 怎么组织多个目标

### Day 1：RobotState + SafetyLimiter

- [ ] 创建 `robot_control_cpp/` 项目结构
- [ ] 实现 `RobotState`
- [ ] 实现 `SafetyLimiter`
- [ ] 写 `test_robot_state.cpp`
- [ ] 写 `test_safety_limiter.cpp`

验收：

- [ ] 能运行 `./test_robot_state`
- [ ] 能运行 `./test_safety_limiter`
- [ ] 能解释为什么状态管理和安全限幅要拆成两个类

### Day 2：CubicInterpolator

- [ ] 实现三次插值轨迹生成
- [ ] 支持任意自由度
- [ ] 检查 `duration > 0`
- [ ] 检查 `dt > 0`
- [ ] 保证最后一个轨迹点时间等于 `duration`

验收：

- [ ] 打印轨迹点数量
- [ ] 打印第一个点、中间点、最后一个点
- [ ] 打印最大速度
- [ ] 起点速度接近 0
- [ ] 终点速度接近 0

### Day 3：TrajectoryBuffer + 控制循环 Demo

- [ ] 实现 `TrajectoryBuffer`
- [ ] 用 `std::deque<TrajectoryPoint>` 保存轨迹点
- [ ] 支持 push / pop / size / clear
- [ ] 空队列 pop 时抛异常
- [ ] 写 `control_loop_demo.cpp`

控制循环流程：

```text
设定当前关节角
设定目标关节角
生成三次插值轨迹
推入轨迹缓存
循环取点
位置限幅
速度限幅
更新 RobotState
打印状态
```

验收：

- [ ] 能运行 `./test_trajectory_buffer`
- [ ] 能运行 `./control_loop_demo`
- [ ] 能看到机械臂状态从起点逐步变化到目标点

### Day 4：重构 CMake + 简单断言测试

- [ ] 所有模块编译进 `robot_control_cpp` library
- [ ] 每个 example 单独生成可执行文件
- [ ] 打开 warning：`-Wall -Wextra -Wpedantic`
- [ ] 给测试程序加入 `assert`
- [ ] 浮点数比较使用 `near(a, b, eps)`

验收：

- [ ] 所有 examples 都能编译
- [ ] 所有 examples 都能运行
- [ ] 不只靠肉眼看输出，至少有一部分自动检查

### Day 5：第一版 README

- [ ] 说明这个库解决什么问题
- [ ] 说明每个模块负责什么
- [ ] 写清楚如何编译
- [ ] 写清楚如何运行测试
- [ ] 写清楚控制循环 demo 的流程

验收：

- [ ] 别人只看 README，也能知道怎么编译和运行纯 C++ 控制库

---

## 第 2 周：Eigen、雅可比求解与项目化

### 本周目标

把纯 C++ 控制库从“轨迹和状态管理”推进到“简单运动学计算”。

- [ ] 引入 Eigen
- [ ] 实现 `JacobianSolver`
- [ ] 完成一个 2 自由度平面机械臂的雅可比速度映射
- [ ] 整理控制库接口和 README

### 核心任务

- [ ] 安装或配置 Eigen
- [ ] 写 `jacobian_solver.hpp`
- [ ] 写 `jacobian_solver.cpp`
- [ ] 写 `test_jacobian_solver.cpp`
- [ ] 输入关节速度，计算末端速度
- [ ] 输入期望末端速度，尝试求解关节速度

验收：

- [ ] 能解释雅可比矩阵在速度控制中的作用
- [ ] 能解释奇异位形为什么危险
- [ ] 能运行所有 C++ examples
- [ ] README 能说明控制库整体架构

---

## 第 3 周：ROS2 入门迁移 + vel_to_pos_node

### 本周定位

你现在会 ROS1，也已经知道 ROS2 功能包怎么创建，所以第 3 周不要直接硬写完整控制节点。先把 ROS2 的基本套路吃透，再把纯 C++ 控制逻辑迁移进去。

本周的真实目标是：

- [ ] 能说清楚 ROS1 和 ROS2 在包、节点、构建、参数、launch 上的差异
- [ ] 能独立创建并编译一个 ROS2 C++ package
- [ ] 能写最小 publisher / subscriber / timer
- [ ] 能用 parameter 和 launch 管理节点
- [ ] 完成 `vel_to_pos_node.cpp` 的第一版

### ROS1 到 ROS2 对照表

先把这张表背熟，后面写代码会顺很多：

| ROS1                             | ROS2                                 |
| -------------------------------- | ------------------------------------ |
| `catkin_make` / `catkin build`   | `colcon build`                       |
| `catkin_ws/src`                  | `ros2_ws/src`                        |
| `package.xml` + `CMakeLists.txt` | 仍然是 `package.xml` + `CMakeLists.txt` |
| `roscore`                        | 通常不需要单独启动 master                     |
| `rosrun pkg node`                | `ros2 run pkg node`                  |
| `roslaunch pkg file.launch`      | `ros2 launch pkg file.launch.py`     |
| `rosnode list`                   | `ros2 node list`                     |
| `rostopic list`                  | `ros2 topic list`                    |
| `rostopic echo`                  | `ros2 topic echo`                    |
| `rostopic pub`                   | `ros2 topic pub`                     |
| `rosparam`                       | `ros2 param`                         |
| `ros::NodeHandle`                | `rclcpp::Node`                       |
| `ros::Publisher`                 | `rclcpp::Publisher<T>::SharedPtr`    |
| `ros::Subscriber`                | `rclcpp::Subscription<T>::SharedPtr` |
| `ros::Timer`                     | `rclcpp::TimerBase::SharedPtr`       |

---

### Day 1：确认 ROS2 工作区和包结构

目标：只做一件事，把 ROS2 工程结构弄清楚。

- [ ] 创建工作区：`ros2_ws/src`
- [ ] 创建包：`robot_control_ros2`
- [ ] 依赖先只加 `rclcpp` 和 `std_msgs`
- [ ] 看懂 `package.xml`
- [ ] 看懂 `CMakeLists.txt`
- [ ] 用 `colcon build` 编译
- [ ] source 环境：`source install/setup.bash`

建议命令：

```bash
mkdir -p ~/ros2_ws/src
cd ~/ros2_ws/src
ros2 pkg create robot_control_ros2 --build-type ament_cmake --dependencies rclcpp std_msgs
cd ~/ros2_ws
colcon build
source install/setup.bash
ros2 pkg list | grep robot_control_ros2
```

今天必须理解：

- [ ] `src/` 下面放 ROS2 package
- [ ] `colcon build` 是在工作区根目录运行
- [ ] `install/setup.bash` 的作用是把新包加入当前终端环境
- [ ] 每开一个新终端都要重新 source

验收：

- [ ] `ros2 pkg list` 能看到 `robot_control_ros2`
- [ ] 能说清楚 package、node、executable 不是同一个东西

---

### Day 2：最小 Publisher 节点

目标：写一个节点定时发布关节速度命令。

文件：

```text
robot_control_ros2/src/joint_velocity_publisher.cpp
```

节点功能：

- 节点名：`joint_velocity_publisher`
- topic：`/joint_velocity_cmd`
- 消息类型：先用 `std_msgs/msg/Float64MultiArray`
- 每 100 ms 发布一次 6 维速度

核心概念：

- [ ] `rclcpp::init`
- [ ] `std::make_shared<NodeClass>()`
- [ ] `rclcpp::spin`
- [ ] `rclcpp::shutdown`
- [ ] `create_publisher`
- [ ] `create_wall_timer`

验收命令：

```bash
ros2 run robot_control_ros2 joint_velocity_publisher
ros2 topic list
ros2 topic echo /joint_velocity_cmd
ros2 topic hz /joint_velocity_cmd
```

验收：

- [ ] 能看到 `/joint_velocity_cmd`
- [ ] `echo` 能看到数组数据
- [ ] `hz` 接近 10 Hz
- [ ] 能解释 timer callback 为什么会周期执行

---

### Day 3：最小 Subscriber 节点

目标：写一个订阅节点，能收到 Day 2 发布的速度命令。

文件：

```text
robot_control_ros2/src/joint_velocity_subscriber.cpp
```

节点功能：

- 节点名：`joint_velocity_subscriber`
- 订阅：`/joint_velocity_cmd`
- 收到后打印 6 个关节速度

核心概念：

- [ ] `create_subscription`
- [ ] callback 函数参数
- [ ] `SharedPtr`
- [ ] QoS 队列深度
- [ ] `RCLCPP_INFO`

验收命令：

```bash
ros2 run robot_control_ros2 joint_velocity_subscriber
ros2 run robot_control_ros2 joint_velocity_publisher
```

也可以手动发布：

```bash
ros2 topic pub /joint_velocity_cmd std_msgs/msg/Float64MultiArray "{data: [0.1, 0.0, 0.0, 0.0, 0.0, 0.0]}"
```

验收：

- [ ] subscriber 能打印收到的数据
- [ ] 能解释 publisher 和 subscriber 不需要互相知道对方存在
- [ ] 能解释 topic 名字和消息类型必须匹配

---

### Day 4：Timer 控制循环节点

目标：理解 ROS2 控制节点通常不是“收到一次就算一次”，而是有一个固定频率控制循环。

文件：

```text
robot_control_ros2/src/timer_control_node.cpp
```

节点功能：

- 节点名：`timer_control_node`
- 内部保存当前关节位置 `q`
- 每 10 ms 执行一次 timer callback
- 每次让 `q[0] += 0.001`
- 发布 `/joint_position_cmd`

核心概念：

- [ ] 控制频率
- [ ] `dt`
- [ ] 节点内部状态变量
- [ ] timer callback 和 subscriber callback 的区别

验收命令：

```bash
ros2 run robot_control_ros2 timer_control_node
ros2 topic echo /joint_position_cmd
ros2 topic hz /joint_position_cmd
```

验收：

- [ ] 能看到位置连续变化
- [ ] 频率接近 100 Hz
- [ ] 能解释为什么控制循环通常用 timer，而不是只依赖 subscriber callback

---

### Day 5：参数和 YAML 配置

目标：把硬编码的控制参数改成 ROS2 parameter。

新增文件：

```text
robot_control_ros2/config/vel_to_pos.yaml
```

建议参数：

```yaml
vel_to_pos_node:
  ros__parameters:
    dof: 6
    control_rate: 100.0
    max_velocity: 0.5
    lower_limits: [-3.14, -1.57, -3.14, -3.14, -2.0, -6.28]
    upper_limits: [3.14, 1.57, 3.14, 3.14, 2.0, 6.28]
```

今天任务：

- [ ] 学会 `declare_parameter`
- [ ] 学会 `get_parameter`
- [ ] 学会 `ros2 param list`
- [ ] 学会 `ros2 param get`
- [ ] 学会从 YAML 启动节点

验收命令：

```bash
ros2 run robot_control_ros2 timer_control_node --ros-args --params-file src/robot_control_ros2/config/vel_to_pos.yaml
ros2 param list
ros2 param get /timer_control_node control_rate
```

验收：

- [ ] 节点能读取 YAML 参数
- [ ] 能用命令行查看参数
- [ ] 能解释为什么参数不应该写死在代码里

---

### Day 6：完成 vel_to_pos_node 第一版

目标：把前几天的 publisher、subscriber、timer、参数合成一个真正的控制节点。

文件：

```text
robot_control_ros2/src/vel_to_pos_node.cpp
```

输入：

- `/joint_velocity_cmd`

输出：

- `/joint_position_cmd`

内部状态：

- `q_current`
- `dq_cmd`
- `lower_limits`
- `upper_limits`
- `max_velocity`
- `dt`

核心逻辑：

```text
subscriber callback:
  保存最新速度命令 dq_cmd

timer callback:
  dq_cmd 限幅
  q_current = q_current + dq_cmd * dt
  q_current 限幅
  发布 q_current
```

今天先不追求完美：

- [ ] 可以先用 `std::vector<double>` 写内部逻辑
- [ ] 可以先不用接第 1、2 周的纯 C++ library
- [ ] 先保证 ROS2 通信链路跑通
- [ ] 跑通后再考虑把 `SafetyLimiter` 接回来

验收命令：

```bash
ros2 run robot_control_ros2 vel_to_pos_node
ros2 topic pub /joint_velocity_cmd std_msgs/msg/Float64MultiArray "{data: [0.1, 0.0, 0.0, 0.0, 0.0, 0.0]}"
ros2 topic echo /joint_position_cmd
```

验收：

- [ ] 输入正速度时，位置逐渐增加
- [ ] 输入 0 速度时，位置保持
- [ ] 输入超过 `max_velocity` 的速度时，会被限制
- [ ] 超过位置上下限时，会被限制

---

### Day 7：Launch 文件和本周复盘

目标：用 launch 一次启动节点，整理第 3 周 README。

新增文件：

```text
robot_control_ros2/launch/vel_to_pos.launch.py
```

launch 目标：

- [ ] 启动 `vel_to_pos_node`
- [ ] 加载 `vel_to_pos.yaml`
- [ ] 后续可以选择同时启动测试 publisher

验收命令：

```bash
ros2 launch robot_control_ros2 vel_to_pos.launch.py
ros2 topic pub /joint_velocity_cmd std_msgs/msg/Float64MultiArray "{data: [0.1, 0.0, 0.0, 0.0, 0.0, 0.0]}"
ros2 topic echo /joint_position_cmd
```

本周复盘要写进 README：

- [ ] 如何创建包
- [ ] 如何编译
- [ ] 如何 source
- [ ] 如何启动每个节点
- [ ] topic 输入输出是什么
- [ ] 参数文件怎么改
- [ ] `vel_to_pos_node` 的控制逻辑图

第 3 周总验收：

- [ ] 能独立写出一个 ROS2 C++ publisher
- [ ] 能独立写出一个 ROS2 C++ subscriber
- [ ] 能独立写出一个 timer 控制循环
- [ ] 能用 YAML 参数配置节点
- [ ] 能用 launch 启动节点
- [ ] 能解释 `vel_to_pos_node` 的数据流

---

## 第 4 周：URDF / RViz2 / robot_state_publisher + 项目整理

### 本周定位

第 4 周不要一上来就追求复杂机械臂。先做一个最小 2 自由度机械臂，只要能在 RViz2 里看到模型，并且能通过 joint state 改变姿态，就已经完成核心目标。

本周目标：

- [ ] 理解 URDF 描述的是机器人结构
- [ ] 理解 `joint_states` 描述的是关节当前状态
- [ ] 理解 `robot_state_publisher` 根据 URDF 和 joint state 发布 TF
- [ ] RViz2 中能看到机械臂
- [ ] 把第 3 周的 `/joint_position_cmd` 转成 `/joint_states`
- [ ] 整理 README、简历描述、面试讲解稿

---

### Day 1：先理解可视化链路

今天先不要写太多代码，先把链路搞明白：

```text
URDF / xacro
  描述 link 和 joint 的结构

/joint_states
  给出每个 joint 当前角度

robot_state_publisher
  读取 URDF 和 /joint_states
  发布 TF

RViz2
  显示 robot model 和 TF
```

必须理解：

- [ ] `link` 是刚体
- [ ] `joint` 是 link 之间的连接
- [ ] revolute joint 需要 axis、limit、origin
- [ ] RViz2 不负责计算运动学，它只是显示 TF 和模型
- [ ] `robot_state_publisher` 才是把 joint angle 转成 TF 的关键节点

验收：

- [ ] 能画出上面的数据流
- [ ] 能解释 URDF、joint state、TF、RViz2 各自负责什么

---

### Day 2：创建 simple_arm_description 包

目标：建立机械臂描述包。

建议命令：

```bash
cd ~/ros2_ws/src
ros2 pkg create simple_arm_description --build-type ament_cmake
```

目录结构：

```text
simple_arm_description/
├── urdf/
│   └── simple_2dof_arm.urdf.xacro
├── launch/
│   └── display.launch.py
├── rviz/
│   └── simple_arm.rviz
├── package.xml
└── CMakeLists.txt
```

今天任务：

- [ ] 创建 `urdf/`
- [ ] 创建 `launch/`
- [ ] 创建 `rviz/`
- [ ] 修改 `CMakeLists.txt`，安装这些目录
- [ ] 检查 `package.xml` 是否需要加入 `xacro`、`robot_state_publisher`、`rviz2`

验收：

- [ ] `colcon build` 成功
- [ ] `ros2 pkg list | grep simple_arm_description` 能找到包

---

### Day 3：写最小 2 自由度 URDF / xacro

目标：先让 RViz2 能显示一个简单机械臂，不追求外观。

模型结构：

```text
base_link
  |
joint1: revolute, 绕 z 轴
  |
link1
  |
joint2: revolute, 绕 z 轴
  |
link2
```

建议尺寸：

- `base_link`：小盒子
- `link1`：长度 1.0
- `link2`：长度 0.8
- `joint1` limit：`-3.14` 到 `3.14`
- `joint2` limit：`-1.57` 到 `1.57`

今天任务：

- [ ] 写 `simple_2dof_arm.urdf.xacro`
- [ ] 每个 link 都有 visual
- [ ] 每个 revolute joint 都有 axis
- [ ] 每个 revolute joint 都有 limit
- [ ] 用 `xacro` 检查能否展开

验收命令：

```bash
ros2 run xacro xacro src/simple_arm_description/urdf/simple_2dof_arm.urdf.xacro
```

验收：

- [ ] xacro 能正常输出 robot XML
- [ ] 没有 XML 语法错误
- [ ] 能说清楚 `origin xyz rpy` 是父子 link 的相对位姿

---

### Day 4：启动 robot_state_publisher + RViz2

目标：在 RViz2 里看到静态机械臂。

新增文件：

```text
simple_arm_description/launch/display.launch.py
```

launch 启动：

- [ ] `robot_state_publisher`
- [ ] `joint_state_publisher_gui` 或后面自己写的 joint state 节点
- [ ] `rviz2`

如果先用 GUI：

```bash
sudo apt install ros-$ROS_DISTRO-joint-state-publisher-gui
```

启动命令：

```bash
ros2 launch simple_arm_description display.launch.py
```

验收：

- [ ] RViz2 中 Fixed Frame 设为 `base_link`
- [ ] 能看到 RobotModel
- [ ] 拖动 joint slider 后机械臂姿态变化
- [ ] TF tree 没有明显断裂

今天重点：

- [ ] 不纠结模型好不好看
- [ ] 先保证 link / joint / TF 链路通

---

### Day 5：把 /joint_position_cmd 转成 /joint_states

目标：让第 3 周的控制输出驱动 RViz2 里的机械臂。

新增节点：

```text
robot_control_ros2/src/position_to_joint_state_node.cpp
```

输入：

- `/joint_position_cmd`

输出：

- `/joint_states`

消息类型：

- 输入仍用 `std_msgs/msg/Float64MultiArray`
- 输出用 `sensor_msgs/msg/JointState`

节点逻辑：

```text
订阅 /joint_position_cmd
取前 2 个关节角
填入 JointState:
  name = ["joint1", "joint2"]
  position = [q0, q1]
  header.stamp = now()
发布 /joint_states
```

今天任务：

- [ ] 给 `robot_control_ros2` 增加 `sensor_msgs` 依赖
- [ ] 写 `position_to_joint_state_node.cpp`
- [ ] 修改 `CMakeLists.txt`
- [ ] 编译通过
- [ ] 手动发布 `/joint_position_cmd` 测试 RViz2 姿态变化

验收命令：

```bash
ros2 run robot_control_ros2 position_to_joint_state_node
ros2 topic pub /joint_position_cmd std_msgs/msg/Float64MultiArray "{data: [0.5, 0.3]}"
ros2 topic echo /joint_states
```

验收：

- [ ] `/joint_states` 能看到 joint1、joint2
- [ ] RViz2 中机械臂会随输入姿态变化
- [ ] 能解释为什么 joint name 必须和 URDF 里的 joint 名字一致

---

### Day 6：完整联调

目标：把整条链路跑起来。

完整链路：

```text
/joint_velocity_cmd
  -> vel_to_pos_node
  -> /joint_position_cmd
  -> position_to_joint_state_node
  -> /joint_states
  -> robot_state_publisher
  -> /tf
  -> RViz2
```

今天任务：

- [ ] 启动 `vel_to_pos_node`
- [ ] 启动 `position_to_joint_state_node`
- [ ] 启动 `display.launch.py`
- [ ] 手动发布 `/joint_velocity_cmd`
- [ ] 观察 `/joint_position_cmd`
- [ ] 观察 `/joint_states`
- [ ] 观察 RViz2 姿态变化

验收命令示例：

```bash
ros2 launch simple_arm_description display.launch.py
ros2 run robot_control_ros2 vel_to_pos_node
ros2 run robot_control_ros2 position_to_joint_state_node
ros2 topic pub /joint_velocity_cmd std_msgs/msg/Float64MultiArray "{data: [0.1, 0.1, 0.0, 0.0, 0.0, 0.0]}"
```

验收：

- [ ] 发布速度后，位置会积分变化
- [ ] joint state 会更新
- [ ] RViz2 里的机械臂会动
- [ ] 能画出完整数据流

---

### Day 7：README、简历描述和面试讲解稿

目标：把项目变成能展示的东西。

README 必须包含：

- [ ] 项目目标
- [ ] 系统架构图
- [ ] package 说明
- [ ] topic 说明
- [ ] parameter 说明
- [ ] build 命令
- [ ] run 命令
- [ ] RViz2 展示说明
- [ ] 已知限制

简历描述初稿：

```text
实现了一个基于 ROS2 的简化机械臂速度-位置控制与可视化系统。项目包含纯 C++ 控制库、ROS2 速度积分位置控制节点、URDF 机械臂模型和 RViz2 可视化链路。通过 timer 控制循环订阅关节速度命令，进行限幅和积分后发布关节位置，并转换为 JointState 驱动 robot_state_publisher 生成 TF，实现机械臂姿态实时显示。
```

面试讲解顺序：

1. 为什么先做纯 C++ 控制库：控制逻辑不依赖 ROS，方便测试。
2. ROS2 节点怎么设计：速度输入、位置输出、timer 控制循环。
3. 为什么要限幅：防止速度和位置命令超过安全范围。
4. URDF 和 RViz2 怎么接：位置命令转 JointState，再由 robot_state_publisher 发布 TF。
5. 项目还能怎么扩展：接真实控制器、接 MoveIt2、加入轨迹跟踪和雅可比逆解。

第 4 周总验收：

- [ ] RViz2 中能看到简单 2 自由度机械臂
- [ ] 能通过 `/joint_position_cmd` 改变机械臂姿态
- [ ] 能通过 `/joint_velocity_cmd` 驱动完整链路
- [ ] README 可以让别人复现项目
- [ ] 简历描述能在 3 到 5 句话内讲清楚

---

## 第 5 周：MoveIt2 机械臂运动规划

### 本周定位

第 5 周开始从“自己写控制链路”进入“工业常用规划框架”。目标不是一口气掌握 MoveIt2 源码，而是先把一个机械臂模型接入 MoveIt2，能在 RViz2 中完成规划和执行。

本周目标：

- [ ] 理解 MoveIt2 的核心组成：MoveGroup、PlanningScene、PlanningPipeline、Planner、TrajectoryExecution
- [ ] 为 `simple_arm_description` 生成 MoveIt2 配置包
- [ ] 能在 RViz2 MotionPlanning 插件中拖动目标位姿或关节目标
- [ ] 能使用 OMPL 完成一次规划
- [ ] 能解释 MoveIt2 和自己写的 `vel_to_pos_node` 的区别

### MoveIt2 核心链路

```text
RobotModel / SRDF
  -> PlanningScene
  -> PlanningPipeline
  -> OMPL Planner
  -> RobotTrajectory
  -> TrajectoryExecution
  -> ros2_control controller
```

今天先理解这些概念：

- [ ] `move_group` 节点负责什么
- [ ] URDF 和 SRDF 的区别
- [ ] Planning Group 是什么
- [ ] Planning Scene 为什么要维护碰撞环境
- [ ] OMPL planner 在 MoveIt2 中处于哪一层

### 本周实作任务

### Day 1：MoveIt2 概念和安装检查

今天先不要急着配置机械臂，先确认环境和概念。

- [ ] 确认 ROS2 发行版和 MoveIt2 是否匹配
- [ ] 安装或检查 `moveit`、`moveit_setup_assistant`
- [ ] 跑一个官方 MoveIt2 demo 或 tutorial
- [ ] 记录 `move_group`、RViz2 MotionPlanning、PlanningScene 的作用
- [ ] 在笔记中画出 MoveIt2 核心链路

验收：

- [ ] 能启动一个 MoveIt2 示例
- [ ] 能说清楚 MoveIt2 主要负责规划，不直接等同于底层控制器
- [ ] 能解释 URDF 和 SRDF 的区别

### Day 2：检查 simple arm 模型是否适合 MoveIt2

今天检查第 4 周做的机械臂模型。

- [ ] 检查 `simple_2dof_arm.urdf.xacro`
- [ ] 确认 joint 类型、axis、limit 是否完整
- [ ] 确认 link / joint 名称清晰
- [ ] 用 `xacro` 展开模型
- [ ] 用 RViz2 显示模型
- [ ] 记录哪些地方需要为了 MoveIt2 修改

验收：

- [ ] URDF 能被正常展开
- [ ] 每个 revolute joint 都有 limit
- [ ] 能解释为什么 MoveIt2 需要 joint limits

### Day 3：创建 simple_arm_moveit_config

目标是生成或手写 MoveIt2 配置包。

- [ ] 使用 MoveIt Setup Assistant 或手动创建 `simple_arm_moveit_config`
- [ ] 配置 planning group：`arm`
- [ ] 配置 planning joints：`joint1`、`joint2`
- [ ] 生成或整理 SRDF
- [ ] 生成 `joint_limits.yaml`
- [ ] 生成 `kinematics.yaml`
- [ ] 生成 `ompl_planning.yaml`

验收：

- [ ] 配置包能被 colcon 编译
- [ ] 能解释 planning group 是什么
- [ ] 能在文件里找到 `arm` 对应哪些 joints

### Day 4：启动 MoveIt2 RViz demo

今天目标是看到 MotionPlanning 面板并能规划。

- [ ] 启动 `demo.launch.py`
- [ ] RViz2 Fixed Frame 设置正确
- [ ] MotionPlanning 面板加载成功
- [ ] 选择 planning group
- [ ] 设置一个关节目标
- [ ] 点击 Plan
- [ ] 观察规划轨迹

验收：

- [ ] RViz2 中能看到机械臂
- [ ] 能规划至少 1 条轨迹
- [ ] 能说出规划失败时先检查哪几个地方：joint limits、planning group、TF、碰撞模型、日志

### Day 5：OMPL planner 对比

今天重点不是调参到完美，而是理解 MoveIt2 通过 OMPL 调用不同规划器。

- [ ] 找到 `ompl_planning.yaml`
- [ ] 尝试 RRTConnect
- [ ] 尝试 RRTstar
- [ ] 尝试改变 planning time
- [ ] 对同一个目标重复规划 3 次
- [ ] 记录规划时间、成功率、路径现象

验收：

- [ ] 能解释 OMPL 是 MoveIt2 的规划插件之一
- [ ] 能说明 RRTConnect 通常适合快速找可行解
- [ ] 能说明 RRTstar 更偏向渐进优化，但可能更慢

### Day 6：MoveGroupInterface C++ demo

今天写一个应用层 C++ 节点调用 MoveIt2。

- [ ] 创建 `moveit_cpp_demo.cpp`
- [ ] 引入 `moveit_ros_planning_interface`
- [ ] 创建 `MoveGroupInterface`
- [ ] 设置 planning group
- [ ] 设置 joint target
- [ ] 调用 `plan()`
- [ ] 打印规划是否成功

验收：

- [ ] 能用 `ros2 run` 启动 demo
- [ ] 能通过代码触发一次规划
- [ ] 能解释 `MoveGroupInterface` 是应用层接口，不是底层 planner 本身

### Day 7：MoveIt2 复盘和 README

今天把 MoveIt2 变成项目资产。

- [ ] 在 README 增加 MoveIt2 运行命令
- [ ] 写 MoveIt2 架构图
- [ ] 写配置包说明
- [ ] 写 planning group 说明
- [ ] 写 OMPL planner 对比记录
- [ ] 写“MoveIt2 与自写控制链路区别”表格

验收：

- [ ] 别人能按 README 启动 MoveIt2 demo
- [ ] 能用 1 分钟解释 MoveIt2 在项目中的位置

验收：

- [ ] MoveIt2 demo 能启动
- [ ] RViz2 中能看到 MotionPlanning 面板
- [ ] 能完成一次 OMPL 规划
- [ ] 能写一个 C++ MoveGroupInterface demo
- [ ] README 增加 MoveIt2 运行说明

---

## 第 6 周：ros2_control 与控制器接口

### 本周定位

第 6 周把机械臂从“RViz2 可视化”和“MoveIt2 规划”推进到“控制器接口”。`ros2_control` 是 ROS2 中连接上层规划和底层硬件/仿真的关键框架。

本周目标：

- [ ] 理解 `ros2_control`、`controller_manager`、`hardware_interface`
- [ ] 配置 `joint_state_broadcaster`
- [ ] 配置 `joint_trajectory_controller`
- [ ] 让 MoveIt2 输出轨迹发送到 controller
- [ ] 能解释 `FollowJointTrajectory` action 的作用

核心概念：

| 概念 | 作用 |
|---|---|
| `ros2_control` | 控制框架 |
| `hardware_interface` | 抽象真实硬件或仿真硬件 |
| `controller_manager` | 管理控制器加载、启动、停止 |
| `joint_state_broadcaster` | 发布关节状态 |
| `joint_trajectory_controller` | 接收轨迹并执行 |
| `FollowJointTrajectory` | MoveIt2 和控制器之间常用 action 接口 |

本周任务：

### Day 1：理解 ros2_control 架构

- [ ] 学习 `ros2_control` 总体结构
- [ ] 区分 hardware、controller、controller_manager
- [ ] 理解 command interface 和 state interface
- [ ] 整理 `position`、`velocity`、`effort` 三类接口
- [ ] 记录 `joint_state_broadcaster` 和 `joint_trajectory_controller` 的职责

验收：

- [ ] 能画出 `MoveIt2 -> controller -> hardware` 的关系
- [ ] 能解释为什么 MoveIt2 不直接控制电机

### Day 2：在 URDF 中加入 ros2_control 标签

- [ ] 修改 `simple_2dof_arm.urdf.xacro`
- [ ] 为 joint1、joint2 添加 command interface
- [ ] 为 joint1、joint2 添加 state interface
- [ ] 先使用 mock 或 fake hardware
- [ ] 用 xacro 检查 XML 是否正确

验收：

- [ ] URDF 能展开
- [ ] 能解释 command interface 是上层写入的命令
- [ ] 能解释 state interface 是底层反馈的状态

### Day 3：配置 controllers.yaml

- [ ] 创建或整理 `ros2_controllers.yaml`
- [ ] 配置 `controller_manager`
- [ ] 配置 `joint_state_broadcaster`
- [ ] 配置 `joint_trajectory_controller`
- [ ] 指定 joints：`joint1`、`joint2`
- [ ] 指定 command interfaces
- [ ] 指定 state interfaces

验收：

- [ ] YAML 结构清楚
- [ ] 能解释 controller 名称、type、joints 各自含义

### Day 4：启动 controller_manager

- [ ] 写或修改 launch 文件
- [ ] 启动 `ros2_control_node`
- [ ] 加载 `robot_description`
- [ ] 检查 controller manager 是否存在
- [ ] 执行 `ros2 control list_hardware_interfaces`
- [ ] 执行 `ros2 control list_controllers`

验收：

- [ ] 能看到 hardware interfaces
- [ ] 能看到 controller 处于 configured 或 active 状态

### Day 5：加载并测试 joint_state_broadcaster

- [ ] 加载 `joint_state_broadcaster`
- [ ] 激活 controller
- [ ] echo `/joint_states`
- [ ] 检查 joint name 是否与 URDF 一致
- [ ] 检查 RViz2 是否能显示状态

验收：

- [ ] `/joint_states` 正常发布
- [ ] RViz2 模型不报 TF / joint state 错误

### Day 6：测试 joint_trajectory_controller

- [ ] 加载 `joint_trajectory_controller`
- [ ] 查看 action 列表
- [ ] 确认 `FollowJointTrajectory` action 存在
- [ ] 用命令行或小脚本发送一条简单轨迹
- [ ] 观察 `/joint_states`
- [ ] 观察 RViz2 中机械臂变化

验收：

- [ ] 能发送一条目标关节轨迹
- [ ] 能解释 trajectory point 里的 positions 和 time_from_start
- [ ] 能解释 action goal、feedback、result

### Day 7：连接 MoveIt2 Execute

- [ ] 修改 MoveIt2 controller 配置
- [ ] 确认 MoveIt2 找到 trajectory controller
- [ ] 在 RViz2 中 Plan
- [ ] 点击 Execute
- [ ] 观察 controller 是否收到轨迹
- [ ] 记录失败原因和解决方式

验收：

- [ ] MoveIt2 Execute 至少能连到 controller 接口
- [ ] 能说明 MoveIt2 规划结果如何通过 action 发给 ros2_control

验收命令方向：

```bash
ros2 control list_controllers
ros2 control list_hardware_interfaces
ros2 action list
ros2 action info /joint_trajectory_controller/follow_joint_trajectory
```

本周验收：

- [ ] 能看到 controller 已 active
- [ ] 能解释 broadcaster 和 controller 的区别
- [ ] 能发送一条 FollowJointTrajectory 测试轨迹
- [ ] 能说明 MoveIt2 规划结果如何进入 ros2_control

---

## 第 7 周：机械臂项目整合与工程化

### 本周定位

第 7 周不再继续堆新概念，而是把前面内容整合成一个完整可展示项目。

最终项目展示链路：

```text
MoveIt2 target
  -> OMPL planning
  -> RobotTrajectory
  -> FollowJointTrajectory
  -> joint_trajectory_controller
  -> joint states
  -> robot_state_publisher
  -> RViz2
```

同时保留自写控制链路：

```text
/joint_velocity_cmd
  -> vel_to_pos_node
  -> /joint_position_cmd
  -> /joint_states
  -> RViz2
```

本周任务：

### Day 1：建立 bringup 包

- [ ] 创建 `simple_arm_bringup`
- [ ] 建立 `launch/`
- [ ] 建立 `config/`
- [ ] 把启动相关逻辑从各包中整理出来
- [ ] 写 `sim_control.launch.py`
- [ ] 写 `moveit_control.launch.py`
- [ ] 写 `full_demo.launch.py` 草稿

验收：

- [ ] bringup 包能编译
- [ ] launch 文件命名清楚
- [ ] 能解释 description、moveit_config、bringup 三个包各自负责什么

### Day 2：整理 launch 启动顺序

- [ ] 确认 robot_description 从哪里加载
- [ ] 确认 robot_state_publisher 启动顺序
- [ ] 确认 controller_manager 启动顺序
- [ ] 确认 MoveIt2 move_group 启动顺序
- [ ] 确认 RViz2 启动配置
- [ ] 给 launch 文件加入必要参数

验收：

- [ ] 能单独启动可视化链路
- [ ] 能单独启动 ros2_control 链路
- [ ] 能单独启动 MoveIt2 链路

### Day 3：统一配置文件

- [ ] 检查 `joint_limits.yaml`
- [ ] 检查 `kinematics.yaml`
- [ ] 检查 `ompl_planning.yaml`
- [ ] 检查 `ros2_controllers.yaml`
- [ ] 删除重复或无效配置
- [ ] 为每个配置文件写一句说明

验收：

- [ ] 重要配置都能在 README 中解释
- [ ] 不再依赖“我自己记得这个文件干嘛用”

### Day 4：完整联调

- [ ] 启动 `full_demo.launch.py`
- [ ] 检查 `/tf`
- [ ] 检查 `/joint_states`
- [ ] 检查 controller 状态
- [ ] 在 MoveIt2 中 Plan
- [ ] 尝试 Execute
- [ ] 记录问题清单

验收：

- [ ] 至少有一条完整链路能稳定跑
- [ ] 所有失败点都有记录，而不是只说“跑不起来”

### Day 5：README 完整复现步骤

README 必须包含：

- [ ] 项目目标
- [ ] 系统架构图
- [ ] 包结构说明
- [ ] 依赖安装
- [ ] build 命令
- [ ] source 命令
- [ ] 自写控制链路运行命令
- [ ] MoveIt2 规划运行命令
- [ ] ros2_control 测试命令
- [ ] 常见问题

验收：

- [ ] 只看 README 可以复现基础演示
- [ ] README 中没有“看情况”“自己改一下”这种模糊描述

### Day 6：简历描述和面试讲解稿

- [ ] 写 3 行简历项目描述
- [ ] 写 1 分钟讲解版本
- [ ] 写 3 分钟讲解版本
- [ ] 写“项目难点”3 条
- [ ] 写“如果继续扩展”3 条
- [ ] 写“我负责/我实现了什么”清单

验收：

- [ ] 能不看文档讲 1 分钟
- [ ] 能回答为什么用了 MoveIt2 和 ros2_control

### Day 7：项目复盘和下一阶段计划

- [ ] 列出已完成内容
- [ ] 列出未完成内容
- [ ] 列出 3 个最值得修的 bug
- [ ] 列出 3 个最值得加的功能
- [ ] 决定是否进入 Nav2 / 视觉 / C++ 重构阶段

验收：

- [ ] 项目状态清楚
- [ ] 下一步不是凭感觉，而是根据清单推进

本周验收：

- [ ] 一条 launch 能启动完整演示
- [ ] README 能让别人复现
- [ ] 能讲清楚自写控制、MoveIt2、ros2_control 三者关系
- [ ] 项目可以作为简历核心项目

---

## 第 8 周：移动机器人定位导航 Nav2 入门

### 为什么要学移动机器人导航

你的主线是机械臂，但机器人岗位常常同时看 ROS2 基础、机械臂 MoveIt2、移动机器人 Nav2、TF、URDF、sensor 和 map。因此 Nav2 不需要现在做深，但应该建立系统认知。

本周目标：

- [ ] 理解移动机器人导航栈整体结构
- [ ] 理解 map、odom、base_link、laser_frame 的 TF 关系
- [ ] 理解 AMCL 定位
- [ ] 理解 costmap
- [ ] 理解 planner、controller、behavior tree
- [ ] 跑通一个 Nav2 仿真 demo 或至少完成架构笔记

核心链路：

```text
map
  -> localization / AMCL
  -> global costmap
  -> global planner
  -> local costmap
  -> controller
  -> cmd_vel
  -> mobile base
```

本周任务：

### Day 1：Nav2 总体架构

- [ ] 阅读 Nav2 概览
- [ ] 画出 `map -> odom -> base_link` TF 树
- [ ] 理解 `cmd_vel` 的作用
- [ ] 理解 navigation goal 的输入
- [ ] 记录 Nav2 中 planner、controller、recoveries / behaviors 的职责

验收：

- [ ] 能解释定位、规划、控制三个词在移动机器人里的含义
- [ ] 能说明 Nav2 的输出为什么通常是 `/cmd_vel`

### Day 2：TF 与坐标系

- [ ] 学习 `map` 坐标系
- [ ] 学习 `odom` 坐标系
- [ ] 学习 `base_link` 坐标系
- [ ] 学习 `laser_frame` 或 camera frame
- [ ] 对比机械臂中的 `base_link`、`link1`、`link2`

验收：

- [ ] 能画出移动机器人 TF 树
- [ ] 能解释 `map -> odom` 和 `odom -> base_link` 的区别

### Day 3：定位 AMCL

- [ ] 学习 AMCL 的输入：map、laser scan、odom、initial pose
- [ ] 学习 AMCL 的输出：`map -> odom`
- [ ] 理解粒子滤波的大概思想
- [ ] 在笔记中写 AMCL 解决什么问题

验收：

- [ ] 能解释“定位”不是“导航”
- [ ] 能说明为什么机器人需要初始位姿

### Day 4：Costmap

- [ ] 学习 global costmap
- [ ] 学习 local costmap
- [ ] 学习 obstacle layer
- [ ] 学习 inflation layer
- [ ] 画出障碍物膨胀示意图

验收：

- [ ] 能解释为什么机器人不能贴着障碍物规划
- [ ] 能解释 global costmap 和 local costmap 的区别

### Day 5：Planner 和 Controller

- [ ] 学习 global planner 的输入输出
- [ ] 学习 local controller 的输入输出
- [ ] 理解路径 path 和速度 command 的区别
- [ ] 记录 Nav2 中 planner/controller 和 MoveIt2 planner/controller 的异同

验收：

- [ ] 能说明 planner 输出 path，不是直接输出电机控制
- [ ] 能说明 controller 负责跟踪路径并输出 `cmd_vel`

### Day 6：跑一个 Nav2 demo 或做完整架构笔记

如果环境允许：

- [ ] 启动 TurtleBot3 或 Nav2 demo
- [ ] 加载地图
- [ ] 设置 initial pose
- [ ] 设置 navigation goal
- [ ] 观察 global path 和 local behavior

如果环境暂时不允许：

- [ ] 完成 Nav2 架构笔记
- [ ] 整理常用 topic
- [ ] 整理常用 launch
- [ ] 整理常见问题

验收：

- [ ] 至少完成 demo 或一篇完整 Nav2 架构笔记

### Day 7：Nav2 和机械臂项目对比

- [ ] 写 Nav2 和 MoveIt2 对比表
- [ ] 写移动机器人和机械臂控制对比
- [ ] 写哪些知识可复用：TF、launch、参数、RViz2、action
- [ ] 写哪些知识不同：底盘速度控制、地图、定位、costmap

验收：

- [ ] 能用 1 分钟解释为什么机械臂项目也值得了解 Nav2

验收：

- [ ] 能画出 Nav2 架构图
- [ ] 能解释定位和导航的区别
- [ ] 能解释 `cmd_vel` 在移动机器人中的作用
- [ ] 能说明机械臂控制和移动机器人控制的共同点与区别

---

## 第 9 周：YOLO、PyTorch 与 LibTorch 机器人感知扩展

### 是否需要学习 PyTorch / LibTorch

需要，但学习目标要明确：

- 你做过 YOLO，说明视觉项目经验可以成为优势。
- 对机器人方向来说，更重要的是“如何把视觉结果接入 ROS2 系统”。
- PyTorch 用于训练和 Python 推理。
- LibTorch 用于 C++ 部署和 ROS2 C++ 节点结合。

本周目标：

- [ ] 复习 PyTorch tensor、model、checkpoint、inference
- [ ] 理解 YOLO 输出：bbox、class、confidence
- [ ] 写一个 ROS2 Python image subscriber
- [ ] 写一个 YOLO ROS2 detection node 设计草图
- [ ] 初步了解 LibTorch C++ 推理流程
- [ ] 思考视觉检测如何和机械臂抓取任务连接

学习重点：

| 内容 | 学到什么程度 |
|---|---|
| Python | 会写 ROS2 Python 节点、图像订阅、结果发布 |
| PyTorch | 会加载模型、前处理、推理、后处理 |
| YOLO | 会解释输入输出和检测结果 |
| LibTorch | 了解 C++ 加载 TorchScript 模型的流程 |
| OpenCV | 会做图像读取、显示、坐标绘制 |
| ROS2 image pipeline | 理解 `sensor_msgs/Image`、`cv_bridge` |

和机械臂项目的连接：

```text
camera image
  -> YOLO detection
  -> object bbox
  -> target center
  -> camera coordinate estimate
  -> TF transform to robot base
  -> MoveIt2 grasp planning
```

本周验收：

- [ ] 能解释 PyTorch 和 LibTorch 的区别
- [ ] 能说明 YOLO 检测结果如何发布成 ROS2 topic
- [ ] 能写出视觉到机械臂抓取的系统框图
- [ ] 能判断哪些部分现在做，哪些部分以后再做

### Day 1：复盘已有 YOLO 项目

- [ ] 写清楚自己做过的 YOLO 项目输入是什么
- [ ] 写清楚输出是什么：bbox、class、confidence
- [ ] 写清楚训练、验证、推理流程
- [ ] 整理模型文件、数据集、评价指标
- [ ] 写“这个经验如何迁移到机器人”

验收：

- [ ] 能用 1 分钟讲清楚自己的 YOLO 项目
- [ ] 能说明检测结果如何变成机器人可用的信息

### Day 2：PyTorch 推理复习

- [ ] 复习 tensor shape
- [ ] 复习 `model.eval()`
- [ ] 复习 `torch.no_grad()`
- [ ] 复习 checkpoint 加载
- [ ] 复习图像前处理：resize、normalize、to tensor
- [ ] 复习后处理：NMS、阈值过滤

验收：

- [ ] 能写一个最小 PyTorch inference 脚本
- [ ] 能解释训练模式和推理模式区别

### Day 3：ROS2 Python 图像节点

- [ ] 创建或设计 `robot_vision_learning`
- [ ] 写 `image_subscriber.py`
- [ ] 订阅 `sensor_msgs/msg/Image`
- [ ] 使用 `cv_bridge` 转 OpenCV 图像
- [ ] 打印图像尺寸
- [ ] 可选：显示图像窗口

验收：

- [ ] 能订阅图像 topic
- [ ] 能解释 ROS image message 和 OpenCV image 的区别

### Day 4：YOLO ROS2 detection node 草稿

- [ ] 设计 `yolo_ros2_node.py`
- [ ] 订阅 image
- [ ] 前处理图像
- [ ] 调用 YOLO 模型推理
- [ ] 后处理得到 bbox
- [ ] 发布检测结果
- [ ] 可选：发布带框图像

输出 topic 设计：

```text
/detections
/debug_image
```

验收：

- [ ] 能画出 image -> detection 的节点图
- [ ] 能说明检测结果应该用什么消息表达：自定义 msg、数组、vision_msgs

### Day 5：视觉结果如何接机械臂

- [ ] 理解 bbox 中心点只是图像坐标
- [ ] 学习相机内参概念
- [ ] 学习深度信息或单目估计的限制
- [ ] 学习 camera frame 到 robot base 的 TF
- [ ] 设计“检测目标 -> MoveIt2 抓取目标”的流程

验收：

- [ ] 能说明 2D bbox 不能直接等于机械臂抓取位姿
- [ ] 能画出 camera -> base_link -> end_effector 的转换链路

### Day 6：LibTorch 入门了解

- [ ] 理解 PyTorch 和 LibTorch 的关系
- [ ] 理解 TorchScript
- [ ] 学习 `torch.jit.trace` 或 `torch.jit.script`
- [ ] 了解 C++ 加载 `.pt` 模型的流程
- [ ] 记录 LibTorch 部署难点：依赖、CMake、CUDA、版本匹配

验收：

- [ ] 能说清楚什么时候用 Python 推理，什么时候考虑 LibTorch
- [ ] 不要求本周完整部署 LibTorch

### Day 7：感知扩展复盘

- [ ] 写 PyTorch / YOLO / LibTorch 对比表
- [ ] 写 ROS2 视觉节点 topic 设计
- [ ] 写机械臂视觉抓取扩展路线
- [ ] 决定是否把视觉部分作为项目加分项，而不是主线

验收：

- [ ] 能把 YOLO 经验和 ROS2 机械臂项目连起来讲
- [ ] 能明确当前阶段不被视觉部分拖慢主线

---

## 第 10 周：C++ 数据结构算法、现代 C++ 与编码风格

### 是否需要学数据结构和算法

需要，但不需要一开始刷很难的算法题。机器人项目中最常用的是：

- `vector`
- `deque`
- `queue`
- `map` / `unordered_map`
- `priority_queue`
- 图搜索
- BFS / DFS
- Dijkstra / A*
- 简单排序和查找
- 数值计算中的矩阵、向量、插值

对机器人方向，优先级最高的是：

1. 能写清楚数据结构的用途
2. 能读懂算法复杂度
3. 能实现 A*、Dijkstra、RRT 的基础版本
4. 能在项目里解释为什么选这个结构

### 现代 C++ 必学内容

| 内容 | 项目用途 |
|---|---|
| `auto` | 简化复杂类型 |
| range-for | 遍历容器 |
| `nullptr` | 替代 `NULL` |
| `enum class` | 更安全的枚举 |
| `std::unique_ptr` | 独占资源 |
| `std::shared_ptr` | ROS2 中广泛使用 |
| lambda | 替代部分 `std::bind` |
| `std::function` | 保存回调 |
| move semantics | 避免不必要拷贝 |
| `const` correctness | 接口设计 |
| RAII | 资源生命周期管理 |
| `optional` / `variant` | 表达可能为空或多类型结果 |

### C++ 编写风格

项目中应逐步建立这些习惯：

- [ ] 类名使用 `PascalCase`
- [ ] 函数名使用 `snake_case` 或项目统一风格
- [ ] 成员变量使用 `_` 后缀，例如 `timer_`
- [ ] 头文件只放声明，源文件放实现
- [ ] public 接口少而清晰
- [ ] 查询函数加 `const`
- [ ] 大对象参数用 `const T&`
- [ ] 避免裸 `new/delete`，优先智能指针和标准容器
- [ ] CMake 用 target 风格
- [ ] 每个模块有最小测试
- [ ] README 写清楚运行方式

本周任务：

### Day 1：项目代码风格体检

- [ ] 检查类名是否统一
- [ ] 检查函数名是否统一
- [ ] 检查成员变量是否使用 `_` 后缀
- [ ] 检查头文件 include guard 或 `#pragma once`
- [ ] 检查 public / private 是否合理
- [ ] 检查是否有过长函数
- [ ] 列出需要重构的点

验收：

- [ ] 得到一份 C++ 风格问题清单
- [ ] 能说出自己项目采用的命名规范

### Day 2：const correctness 和参数传递

- [ ] 检查查询函数是否加 `const`
- [ ] 检查大对象参数是否用 `const T&`
- [ ] 检查需要修改的参数是否用 `T&`
- [ ] 检查返回值是否有悬空引用风险
- [ ] 修改 `RobotState`、`SafetyLimiter`、`TrajectoryBuffer` 中不合理接口

验收：

- [ ] 能解释为什么 `positions() const` 很重要
- [ ] 能解释 `const T&` 和 `T` 传参区别

### Day 3：现代 C++ 回调写法

- [ ] 复习 `std::bind`
- [ ] 复习 lambda
- [ ] 把一个 timer callback 改写成 lambda
- [ ] 把一个 subscriber callback 改写成 lambda
- [ ] 对比可读性
- [ ] 记录 ROS2 中 `SharedPtr` 的常见写法

验收：

- [ ] 能写出 `std::bind(&Class::func, this, _1)`
- [ ] 能写出等价 lambda
- [ ] 能解释 lambda 捕获 `[this]`

### Day 4：智能指针和 RAII

- [ ] 复习 `unique_ptr`
- [ ] 复习 `shared_ptr`
- [ ] 复习 `weak_ptr`
- [ ] 检查项目中是否有裸 `new/delete`
- [ ] 用标准容器或智能指针替代不必要的手动内存管理
- [ ] 记录 ROS2 为什么大量使用 `SharedPtr`

验收：

- [ ] 能解释所有权
- [ ] 能解释 `unique_ptr` 和 `shared_ptr` 的区别
- [ ] 能解释 RAII 为什么能减少资源泄漏

### Day 5：数据结构和控制项目对应

- [ ] 复习 `vector`
- [ ] 复习 `deque`
- [ ] 复习 `queue`
- [ ] 复习 `map` / `unordered_map`
- [ ] 复习 `priority_queue`
- [ ] 写一张表：每种结构在机器人项目中适合放什么

示例：

| 数据结构 | 项目用途 |
|---|---|
| `vector` | 关节位置、速度、轨迹点数组 |
| `deque` | 轨迹缓存 |
| `queue` | 消息队列、任务队列 |
| `unordered_map` | joint name 到 index 的映射 |
| `priority_queue` | A* open set |

验收：

- [ ] 能解释为什么 `TrajectoryBuffer` 适合用 `deque`
- [ ] 能解释 joint name 映射为什么适合用 map

### Day 6：算法小 demo：A* 或 Dijkstra

- [ ] 选择 A* 或 Dijkstra
- [ ] 写一个 2D grid map
- [ ] 设置起点、终点、障碍物
- [ ] 输出路径
- [ ] 使用 `priority_queue`
- [ ] 记录算法复杂度和适用场景

验收：

- [ ] demo 能运行
- [ ] 能解释 open set、closed set
- [ ] 能说明 A* 和 Dijkstra 的区别
- [ ] 能把它和 Nav2 global planner 联系起来

### Day 7：C++ 工程复盘和重构提交

- [ ] 整理本周 C++ 修改点
- [ ] 更新 README 或代码注释
- [ ] 整理 C++ 面试问题与项目对应表
- [ ] 提交一次代码重构
- [ ] 写“我的 C++ 编码规范”小节

验收：

- [ ] 项目代码比重构前更清楚
- [ ] 能从项目里举例说明现代 C++ 用法

验收：

- [ ] 能解释 `shared_ptr` 为什么在 ROS2 中常见
- [ ] 能解释 lambda 和 `std::bind` 的关系
- [ ] 能解释 `vector`、`deque`、`queue` 在项目中分别适合哪里
- [ ] 能说出自己的 C++ 编码规范

---

## 进阶内容优先级

如果时间有限，按这个顺序推进：

1. MoveIt2 基础规划
2. ros2_control + joint_trajectory_controller
3. 现代 C++ 和项目重构
4. Nav2 架构理解
5. PyTorch / YOLO ROS2 感知链路
6. LibTorch C++ 推理
7. 更深入的数据结构算法

原因：

- MoveIt2 和 ros2_control 最贴近机械臂控制项目。
- 现代 C++ 会直接提升代码质量。
- Nav2 和视觉是横向扩展，适合做机器人系统认知。
- LibTorch 价值高，但部署成本也高，不应压过主线。

---

## 学习资源选择

每个方向只选“一个主线资源 + 官方文档 + 项目实作”，避免资料过载。

| 方向 | 学习方式 |
|---|---|
| ROS2 基础 | 官方 tutorials + 自己写节点 |
| MoveIt2 | MoveIt2 tutorials + 自己的 simple arm 配置 |
| ros2_control | 官方 demos + 自己的 joint trajectory controller |
| Nav2 | Nav2 getting started + 架构笔记 |
| PyTorch / YOLO | 复用已有 YOLO 经验，补 ROS2 图像节点 |
| LibTorch | 先了解 TorchScript C++ 推理，不急着完整部署 |
| 现代 C++ | Effective C++ / 侯捷课程 + 项目重构 |
| 数据结构算法 | 机器人常用算法优先：A*、Dijkstra、RRT、队列、图 |

不建议现在做的事：

- [ ] 不要同时开多个大型项目
- [ ] 不要一开始就啃完整 MoveIt2 源码
- [ ] 不要把 Nav2 做成第二个主线
- [ ] 不要为了学 LibTorch 牺牲 MoveIt2 和 ros2_control
- [ ] 不要随机刷 C++ 面试题，要和项目代码绑定

---

## 新版简历项目定位

项目名称可以升级为：

> 基于 ROS2、MoveIt2 与 ros2_control 的机械臂控制与规划系统

简历描述方向：

```text
实现了一个基于 ROS2 的机械臂控制与规划项目，包含纯 C++ 控制库、ROS2 控制节点、URDF/RViz2 可视化、MoveIt2 运动规划和 ros2_control 控制器接口。项目中完成了速度到位置的控制闭环、关节状态发布、TF 可视化、OMPL 规划配置以及 FollowJointTrajectory 控制链路，并进一步调研 Nav2 移动机器人导航和 YOLO 视觉感知接入 ROS2 的扩展方案。
```

面试讲解升级版：

1. 我先用纯 C++ 写控制库，保证核心逻辑可测试。
2. 然后用 ROS2 节点把控制逻辑接入 topic、timer、parameter 和 launch。
3. 再用 URDF、JointState、robot_state_publisher 和 RViz2 做可视化闭环。
4. 接着加入 MoveIt2，让系统具备规划能力。
5. 再接 ros2_control，让规划轨迹能进入标准控制器接口。
6. 最后扩展学习 Nav2 和 YOLO 感知链路，形成对机器人系统的整体认识。

---

## 每天执行模板

每天开始前先写：

~~~markdown
## 2026-xx-xx

### 今日目标

- [ ] 

### 今日实现

- [ ] 

### 今日运行命令

```bash

```

### 今日问题

- 

### 明天第一步

- 
~~~

每天结束时必须留下三样东西：

1. 今天跑通了什么命令
2. 今天遇到的一个问题
3. 明天打开电脑后第一步做什么

---

## 项目解释主线

面试或复盘时按这条线讲：

1. 我先把控制逻辑从 ROS2 中解耦出来，做成纯 C++ 控制库。
2. 控制库里分了状态管理、安全限幅、轨迹插值、轨迹缓存和雅可比求解几个模块。
3. 然后我把纯 C++ 控制逻辑迁移成 ROS2 节点，实现速度命令到位置命令的转换。
4. 最后用 URDF、RViz2 和 TF2 做简单机械臂可视化，形成一个完整的控制链路。
5. 在基础闭环跑通后，加入 MoveIt2，让机械臂具备运动规划能力。
6. 再接入 ros2_control 和 joint trajectory controller，让规划轨迹进入标准控制器接口。
7. 最后横向补 Nav2、YOLO/PyTorch/LibTorch 和现代 C++，把项目扩展成机器人系统能力展示。

---

## 风险与调整

### 如果 C++ 卡住

不要补整本语法书，只补当前模块需要的知识：

- `class`
- `std::vector`
- `std::deque`
- `const &`
- exception
- CMake target

### 如果 ROS2 环境卡住

先继续完成前 2 周纯 C++ 部分。纯 C++ 控制库不依赖 ROS2，不能因为环境问题停掉主线。

### 如果时间不够

优先级如下：

1. 纯 C++ 控制库跑通
2. `vel_to_pos_node` 跑通
3. RViz2 可视化
4. README 写清楚
5. MoveIt2 基础规划
6. ros2_control 控制器接口
7. 现代 C++ 和项目重构
8. Nav2 / 视觉 / LibTorch 扩展

---

## 关联资料

- 原始 8 周任务书：`C:/Users/zlab/Documents/robot-learning/robot_arm_control_ros2_taskbook.md`
- 原始 4 周加速任务书：`C:/Users/zlab/Documents/robot-learning/robot_arm_control_ros2_accelerated_taskbook.md`
