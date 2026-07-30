# ROS2 基础开发闭环

## 结论

学习 ROS2 最有效的方式不是先读完所有概念，而是完成一个可验证的闭环：一个 C++ 功能包发布消息，另一个节点订阅并按参数运行，通过 launch 启动，并能用命令行观察节点、topic、参数和日志。

## 最小系统

```text
launch
  ├─ publisher node  →  /joint_velocity_cmd
  └─ controller node →  /joint_position_cmd

终端检查：node / topic / param / log
```

## 核心概念

| 概念 | 作用 | 不应混淆为 |
| --- | --- | --- |
| Package | 可构建、可安装的功能单元 | 单个 `.cpp` 文件 |
| Node | 一个运行中的计算单元 | 必须独立进程的唯一形式 |
| Topic | 持续数据流 | 一次性请求—响应 |
| Service | 短时请求—响应 | 长时间可取消任务 |
| Action | 可反馈、可取消的长任务 | 普通 topic |
| Parameter | 节点的低频配置 | 高频控制指令 |
| QoS | 发布者与订阅者的通信约束 | 仅仅是队列长度 |

## C++ 包的基本结构

```text
my_robot_ws/
└─ src/my_control_pkg/
   ├─ package.xml
   ├─ CMakeLists.txt
   ├─ src/controller_node.cpp
   ├─ launch/control.launch.py
   └─ config/controller.yaml
```

构建与运行的最小流程：

```bash
cd ~/my_robot_ws
colcon build --packages-select my_control_pkg
source install/setup.bash
ros2 launch my_control_pkg control.launch.py
```

## 必做的四个练习

1. **发布/订阅**：发布一个明确类型的速度或位置消息，订阅节点打印并保存最新值。
2. **定时器**：用固定周期读取最新命令，避免在订阅回调中直接承担所有控制逻辑。
3. **参数**：将控制周期、限幅和 topic 名声明为参数；修改参数后能观察节点行为变化。
4. **launch**：一次启动多个节点与 YAML 配置，而不是依赖手工开多个终端。

## QoS 的实用选择

- 传感器流通常更关注新鲜度，可考虑较小队列与 `Best Effort`，但发布端与订阅端必须兼容。
- 控制目标或状态确认通常更重视送达，可考虑 `Reliable`，同时评估重传带来的延迟。
- 先使用系统提供的默认 QoS 跑通；出现“节点和 topic 都存在但收不到数据”时，再检查 reliability、durability、history 和深度设置。

## 排错清单

```bash
ros2 pkg executables my_control_pkg
ros2 node list
ros2 node info /controller_node
ros2 topic list -t
ros2 topic echo /joint_velocity_cmd
ros2 topic hz /joint_velocity_cmd
ros2 param list /controller_node
ros2 param get /controller_node control_period
```

1. 当前终端是否已 `source install/setup.bash`。
2. 可执行文件、launch 文件和安装规则是否正确。
3. 发布者、订阅者的消息类型和 QoS 是否兼容。
4. 参数是否已声明，并被 YAML/launch 正确覆盖。
5. 涉及可视化时，`joint_states`、URDF、TF2 与 RViz2 是否处于同一时间与坐标体系。

## 关联笔记

- [[通信协议-DDS-TCP-UDP]]
- [[项目/机械臂ROS2控制项目计划]]
- [[../ROS1/ROS Learning|ROS1 与 ROS2 的差异]]
