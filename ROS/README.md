# ROS 入口

本目录以 ROS1 与 MoveIt 学习资料为主，ROS2 内容用于记录通信机制与机械臂控制项目扩展。

## 推荐阅读路径

1. [[ROS1/README|ROS1 基础]]：理解节点、话题、常用命令、TF 与机器人驱动。
2. [[MoveIt/README|MoveIt]]：从概念和教程进入轨迹执行、控制器与 OMPL 源码阅读。
3. [[../motion_planning/README|运动规划]]：单独理解 RRT、OMPL 接口与自定义规划器。
4. [[导航/SLAM导航与多机器人编队|SLAM、导航与多机器人编队]]：对应移动机器人项目复习。
5. [[ROS2/README|ROS2]]：需要迁移或继续机械臂控制项目时再进入。

## 目录说明

| 目录 | 内容 |
|---|---|
| [[ROS1/README\|ROS1]] | ROS 概览、常用指令、TF、AUBO 源码阅读与 pluginlib |
| [[MoveIt/README\|MoveIt]] | MoveIt 教程、轨迹执行、控制器和 MoveIt-OMPL 源码阅读 |
| [[ROS2/README\|ROS2]] | DDS 通信笔记与 ROS2 机械臂项目资料 |
| [[导航/SLAM导航与多机器人编队\|导航]] | Gmapping、AMCL、move_base、探索与主从编队 |

## 整理原则

- ROS1 与 ROS2 笔记按实际 API 和运行环境分类，不因主题相似而混放。
- MoveIt 的应用笔记放在 `MoveIt/`；纯算法和自定义规划实验放在 `motion_planning/`。
- 继续项目时以代码、构建日志和演示结果确认实际进度，不把历史计划当成完成记录。
