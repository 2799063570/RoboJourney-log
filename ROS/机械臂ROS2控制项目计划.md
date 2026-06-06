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

- [ ] 使用 MoveIt Setup Assistant 或手动方式创建 `simple_arm_moveit_config`
- [ ] 配置 planning group，例如 `arm`
- [ ] 配置 `joint_limits.yaml`
- [ ] 配置 `kinematics.yaml`
- [ ] 配置 `ompl_planning.yaml`
- [ ] 启动 MoveIt2 demo launch
- [ ] 在 RViz2 中完成 3 次不同目标的规划
- [ ] 修改 planner，例如 RRTConnect、RRTstar，对比规划现象
- [ ] 写一个 `moveit_cpp_demo.cpp`，使用 `MoveGroupInterface` 设置关节目标并调用 `plan()`

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

- [ ] 在 URDF/xacro 中加入 `ros2_control` 标签
- [ ] 写 `ros2_controllers.yaml`
- [ ] 启动 `controller_manager`
- [ ] 加载 `joint_state_broadcaster`
- [ ] 加载 `joint_trajectory_controller`
- [ ] 用命令行发送一条简单轨迹
- [ ] 尝试让 MoveIt2 Execute 接到 controller

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

- [ ] 建立 `simple_arm_bringup`
- [ ] 统一 launch 文件
- [ ] 整理 config 文件
- [ ] 写系统架构图
- [ ] 写运行脚本
- [ ] 写 README 完整复现步骤
- [ ] 写简历项目描述
- [ ] 写 3 分钟面试讲解稿

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

- [ ] 学习 `map -> odom -> base_link` TF 树
- [ ] 学习 AMCL 输入输出
- [ ] 学习 global planner 和 local controller 的区别
- [ ] 学习 costmap 的 obstacle layer、inflation layer
- [ ] 整理 Nav2 和 MoveIt2 的对比

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

- [ ] 重构 `robot_control_cpp`，检查命名、const、传参
- [ ] 把 `std::bind` 示例改写一个 lambda 版本
- [ ] 为 `RobotState`、`SafetyLimiter`、`TrajectoryBuffer` 写更清楚的接口注释
- [ ] 实现一个 A* 或 Dijkstra 小 demo
- [ ] 整理 `C++ 面向对象复习` 与项目代码的对应关系

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
