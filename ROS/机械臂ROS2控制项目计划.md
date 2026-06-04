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

主线采用 **4 周加速版**。

如果某一周明显吃力，就退回 8 周版节奏，不跳过验收标准。我们的原则是：

- 每天 2 到 3 小时
- 每天必须有一个可运行的小结果
- 每周必须有一个可展示的阶段成果
- 不追求一次写完美，先跑通，再重构，再解释清楚

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

| ROS1 | ROS2 |
| --- | --- |
| `catkin_make` / `catkin build` | `colcon build` |
| `catkin_ws/src` | `ros2_ws/src` |
| `package.xml` + `CMakeLists.txt` | 仍然是 `package.xml` + `CMakeLists.txt` |
| `roscore` | 通常不需要单独启动 master |
| `rosrun pkg node` | `ros2 run pkg node` |
| `roslaunch pkg file.launch` | `ros2 launch pkg file.launch.py` |
| `rosnode list` | `ros2 node list` |
| `rostopic list` | `ros2 topic list` |
| `rostopic echo` | `ros2 topic echo` |
| `rostopic pub` | `ros2 topic pub` |
| `rosparam` | `ros2 param` |
| `ros::NodeHandle` | `rclcpp::Node` |
| `ros::Publisher` | `rclcpp::Publisher<T>::SharedPtr` |
| `ros::Subscriber` | `rclcpp::Subscription<T>::SharedPtr` |
| `ros::Timer` | `rclcpp::TimerBase::SharedPtr` |

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

## 每天执行模板

每天开始前先写：

```markdown
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
```

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
3. README 写清楚
4. RViz2 可视化
5. 更复杂的雅可比和面试扩展

---

## 关联资料

- 原始 8 周任务书：`C:/Users/zlab/Documents/robot-learning/robot_arm_control_ros2_taskbook.md`
- 原始 4 周加速任务书：`C:/Users/zlab/Documents/robot-learning/robot_arm_control_ros2_accelerated_taskbook.md`
