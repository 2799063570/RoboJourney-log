---
reviewed: 2026-07-30
tags:
  - vault
  - obsidian
  - maintenance
status: active
---

# Vault 文档结构与整理原则

## 当前结论

本库以 C++ 与 ROS1 为主要学习线，MoveIt 与运动规划作为机器人应用延伸；ROS2 内容独立保存，避免与 ROS1 的 API、命令和项目状态混在一起。

## 当前结构

```text
CPP/                    C++、STL、数据结构与工程实践
ROS/
├── ROS1/               ROS1 基础、TF、AUBO 与 pluginlib
├── MoveIt/             教程、轨迹执行和 MoveIt-OMPL 源码阅读
└── ROS2/               通信机制与机械臂 ROS2 项目资料
motion_planning/        RRT、OMPL、自定义规划与代码文档
Git/                    Git 笔记
Inbox/                  临时材料
```

## 本轮整理记录

- 根目录只保留知识库入口页，`C++现代用法.md` 已归入 `CPP/`。
- 原 `ros_learning/` 已明确为 `ROS1/`；ROS1 `tf` 与 pluginlib 笔记一并归档。
- MoveIt 教程、轨迹执行和 MoveIt-OMPL 源码阅读统一放入 `ROS/MoveIt/`。
- ROS2 的通信与项目资料归入 `ROS/ROS2/`。
- `motion_planning/Code_Docs` 的 `.md.md` 重复后缀已统一为 `.md`。
- 原空白的传感器笔记已替换为 MoveIt 与 OctoMap 的主题入口。

## 维护规则

- 新笔记先确定其运行环境和主题：C++、ROS1、MoveIt、ROS2 或运动规划。
- 项目计划和体检记录必须注明适用时间；实际进度以代码和可复现实验为准。
- 图片与笔记放在同一主题目录，移动笔记时同步检查嵌入路径。
- 不在 Vault 中提交构建产物、IDE 缓存或 Obsidian 本地工作区状态。
- `Inbox/待整理` 只作临时缓冲区；完成归类后清空。
