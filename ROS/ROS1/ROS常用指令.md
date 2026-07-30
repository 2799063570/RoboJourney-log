# ROS1 常用命令与排错流程

> 适用范围：ROS1（`roscore`、`catkin`、`roslaunch`）。ROS2 请使用 `ros2 ...`、`colcon` 与 `ros2 launch`，不要直接照搬本页命令。

## 先建立正确的工作习惯

每次打开新终端，先加载 ROS 环境；在工作空间编译后，再加载工作空间环境：

```bash
source /opt/ros/noetic/setup.bash
source ~/catkin_ws/devel/setup.bash
```

若命令找不到包、节点或消息类型，优先检查这两行是否已执行，以及 `ROS_PACKAGE_PATH` 是否包含工作空间。

## 启动与包管理

| 目的 | 命令 | 说明 |
| --- | --- | --- |
| 启动 Master | `roscore` | 每个 ROS1 图通常需要一个 Master。Master 负责名称注册与连接信息匹配，不转发普通 topic 数据。 |
| 查找包位置 | `rospack find <package>` | 确认包是否被当前环境发现。 |
| 进入包目录 | `roscd <package>` | 依赖 `rosbash`。 |
| 构建工作空间 | `catkin_make` | 在工作空间根目录执行。 |
| 启动节点 | `rosrun <package> <executable>` | 适合单节点调试。 |
| 启动系统 | `roslaunch <package> <file.launch>` | 适合一组节点、参数和 remap。 |

## 节点、话题、服务与参数

### 节点

```bash
rosnode list
rosnode info /node_name
rosnode ping /node_name
```

先用 `rosnode list` 确认节点存在，再用 `rosnode info` 查看它订阅、发布和连接的接口。

### 话题

```bash
rostopic list
rostopic info /topic_name
rostopic echo /topic_name
rostopic hz /topic_name
rostopic type /topic_name
```

常用诊断顺序：`list` 确认名称 → `info` 确认发布/订阅双方 → `echo` 检查数据内容 → `hz` 检查频率。临时发布测试消息时，先用 `rostopic type` 确认消息类型，再按类型构造命令。

### 服务与参数

```bash
rosservice list
rosservice type /service_name
rosservice call /service_name <request>

rosparam list
rosparam get /parameter_name
rosparam set /parameter_name <value>
rosparam load config.yaml /namespace
```

服务用于短时请求—响应；持续更新的状态、传感器和控制量通常应使用 topic。参数用于启动配置，不适合高频控制数据。

## 记录与回放

```bash
rosbag record -O run_01.bag /joint_states /tf /tf_static
rosbag info run_01.bag
rosbag play --clock run_01.bag
```

录制时只选择排错需要的话题，并记录 TF 与时间信息。回放仿真数据时，系统中需要正确设置 `/use_sim_time` 并有 `/clock` 来源。

## TF 与可视化

```bash
rosrun tf view_frames
rosrun tf tf_echo base_link tool0
rosrun rqt_graph rqt_graph
```

`view_frames` 用于检查 TF 树是否断开；`tf_echo` 用于检查两个坐标系之间是否有持续更新的变换；`rqt_graph` 用于看节点连接关系。详细说明见 [[tf坐标变换]]。

## 网络与多机问题

多机 ROS1 需要所有机器能够互相访问，且每台机器的 `ROS_MASTER_URI` 指向同一个 Master；同时为各机器正确设置可达的 `ROS_IP` 或 `ROS_HOSTNAME`。出现“能看到节点但收不到数据”时，先检查网络、主机名解析和防火墙，再检查 topic 名称与消息类型。

## 最小排错清单

1. 已执行正确的 `setup.bash`，包能被 `rospack find` 找到。
2. `roscore` 与目标节点正在运行。
3. 节点名、topic 名、namespace 和 remap 与预期一致。
4. 发布者和订阅者使用相同消息类型，且发布频率正常。
5. 涉及坐标时，TF 树完整、时间戳有效、父子坐标系方向正确。
6. 涉及硬件时，先确认底层驱动和 `/joint_states`，再排查 MoveIt 或上层控制。
