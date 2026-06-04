---
created: 2026-06-04
tags:
  - vault
  - obsidian
  - project-review
status: active
---

# Vault 文档结构体检与整理方案

## 总体判断

现在这个 Vault 的问题不是“资料少”，而是资料边界不清。

目前混在一起的内容有：

- 长期知识笔记
- 项目计划
- 项目复盘
- 代码文档
- 教程摘录
- 日记式理解
- 图片素材
- Visual Studio / C++ 编译产物

这些内容都可以保留，但需要分层。否则后面查资料时会出现一个问题：文件名看起来都很重要，但打开后不知道它是概念、项目记录、教程、还是临时草稿。

---

## 最严重的问题

### 1. 根目录放了太多主题文档

当前根目录有：

```text
Sensors Functionality.md
Trajectory Execution.md
tf坐标变换.md
通信协议.md
文字记录方法.md
planner.md
README.md
```

问题：

- `tf坐标变换.md` 明显属于 ROS
- `Trajectory Execution.md` 明显属于 MoveIt / ROS 控制执行
- `通信协议.md` 明显属于 ROS2 / DDS / 网络通信
- `Sensors Functionality.md` 是空文件
- `planner.md` 内容像 OMPL 自定义规划器代码草稿，不适合放根目录

建议：

```text
tf坐标变换.md
-> ROS/基础概念/tf坐标变换.md

Trajectory Execution.md
-> ROS/MoveIt/Trajectory Execution.md

通信协议.md
-> ROS/ROS2/通信协议-DDS-TCP-UDP.md

Sensors Functionality.md
-> 删除或移入 Inbox/待整理/

planner.md
-> motion_planning/OMPL/自定义Planner骨架.md
```

---

### 2. ROS 目录内部层级不稳定

当前 ROS 目录大致是：

```text
ROS/
├── moveit-ompl/
├── moveit_tutorials/
├── ros_learning/
├── pluginlib插件机制.md
├── 机械臂ROS2控制项目计划.md
└── 机械臂控制项目体检与完善清单.md
```

问题：

- `ros_learning`、`moveit_tutorials`、`moveit-ompl` 命名风格不统一
- ROS1、ROS2、MoveIt、项目计划混在同一级
- `pluginlib插件机制.md` 应该归入 ROS 基础或 MoveIt 插件机制
- 机械臂项目计划应该放在项目目录，不应和基础知识同级

建议结构：

```text
ROS/
├── 00-索引.md
├── 基础概念/
│   ├── ROS Learning.md
│   ├── ROS常用指令.md
│   ├── tf坐标变换.md
│   └── pluginlib插件机制.md
├── ROS2/
│   ├── 通信协议-DDS-TCP-UDP.md
│   └── ROS1到ROS2迁移.md
├── MoveIt/
│   ├── Trajectory Execution.md
│   ├── moveit_tutorials/
│   └── moveit-ompl/
└── 项目-机械臂ROS2控制/
    ├── 机械臂ROS2控制项目计划.md
    ├── 机械臂控制项目体检与完善清单.md
    ├── 每日记录.md
    └── README草稿.md
```

---

### 3. 编译产物进入了 Vault

扫描发现大量 Visual Studio / C++ 构建产物：

```text
*.exe
*.pdb
*.obj
*.tlog
*.ilk
*.idb
*.recipe
*.lastbuildstate
*.sln
*.vcxproj
*.user
*.filters
*.vsidx
*.ipch
*.suo
```

其中一部分是工程文件，例如 `.sln`、`.vcxproj` 可能有保留价值；但 `.exe`、`.pdb`、`.obj`、`.tlog`、`.ilk`、`.idb`、`.recipe`、`.lastbuildstate`、`.vsidx`、`.ipch`、`.suo` 不适合进 Obsidian Vault。

建议：

- [ ] 保留源码：`.cpp`、`.h`、`.hpp`
- [ ] 视情况保留工程配置：`.sln`、`.vcxproj`
- [ ] 从 Git 跟踪中移除构建产物
- [ ] `.gitignore` 中忽略后续构建产物

已处理：

- [x] `.gitignore` 已加入常见构建产物忽略规则

未处理：

- [ ] 还没有从 Git 索引中移除已经被跟踪的旧构建产物

---

### 4. motion_planning 的 Code_Docs 命名有问题

当前有很多文件名像：

```text
motion_planning/Code_Docs/utils/grid.py.md.md
motion_planning/Code_Docs/utils/fast_fk.py.md.md
motion_planning/Code_Docs/model/data_loader.py.md.md
motion_planning/Code_Docs/debug/check_paths.py.md.md
```

问题：

- `.md.md` 重复后缀不专业
- `Code_Docs` 像自动生成文档，但没有说明来源
- 文件名既像源码文件又像笔记文件，后续查找容易混乱

建议：

```text
motion_planning/
├── 00-索引.md
├── 算法设计/
│   ├── motion planning algorithm self.md
│   └── modifyRRT.md
├── 数据生成/
│   └── 数据生成程序.md
└── 代码文档/
    ├── README_MAP.md
    ├── utils/
    │   ├── grid.py.md
    │   ├── fast_fk.py.md
    │   └── dataset.py.md
    ├── model/
    └── debug/
```

---

### 5. 笔记写法需要从“聊天式”改成“复用式”

很多文档内容并不是错，而是写法更像当时理解过程。例如：

```text
学了这么长时间的ROS了，但是真要问ROS是什么还真说不上来
```

这类句子适合放在学习日志里，不适合作为长期概念笔记开头。

建议把笔记分成两类：

#### 长期知识笔记

结构：

```markdown
# 主题

## 结论

## 使用场景

## 核心机制

## 常用命令 / API

## 易错点

## 与其他概念的关系

## 参考链接 / 关联笔记
```

#### 学习日志

结构：

```markdown
# 日期 - 学习主题

## 今天学了什么

## 卡住的问题

## 解决方式

## 明天继续
```

---

## 具体文件建议

### `通信协议.md`

评价：

- 内容很有价值，适合做 ROS2 通信机制核心笔记。
- 但篇幅较长，混合了 TCP、UDP、DDS、Nodelet、Component、ROS1/ROS2 通信过程。

建议拆成：

```text
ROS/ROS2/通信协议-DDS-TCP-UDP.md
ROS/ROS2/QoS策略.md
ROS/ROS2/Component与零拷贝.md
ROS/基础概念/ROS1通信机制.md
```

### `tf坐标变换.md`

评价：

- 内容偏 ROS1 `tf`，但当前项目是 ROS2，应补 `tf2`。

建议：

```text
ROS/基础概念/tf坐标变换.md
ROS/ROS2/tf2坐标变换.md
```

在原文开头加一句：

```text
注意：本文主要记录 ROS1 tf API。ROS2 项目应使用 tf2_ros。
```

### `Trajectory Execution.md`

评价：

- 内容其实是 MoveIt 控制器管理器：fake controller 与 simple controller。
- 文件名过宽，容易误以为是所有轨迹执行。

建议重命名：

```text
ROS/MoveIt/MoveIt控制器管理器-Fake与Simple.md
```

### `Sensors Functionality.md`

评价：

- 空文件。

建议：

- 如果暂时不用，删除。
- 如果以后写传感器体系，移动到：

```text
ROS/传感器/Sensors Functionality.md
```

### `planner.md`

评价：

- 内容是 OMPL 自定义 Planner 的代码骨架。
- 不应放根目录。

建议：

```text
motion_planning/OMPL/自定义Planner骨架.md
```

---

## 推荐的新 Vault 顶层结构

建议不要一次性大重构，先建立清晰的顶层：

```text
00-Vault管理/
01-项目/
CPP/
ROS/
motion_planning/
Git/
Inbox/
```

说明：

- `00-Vault管理/`：放 Vault 规则、整理方案、索引说明
- `01-项目/`：放真正项目推进材料，例如机械臂 ROS2 控制项目
- `CPP/`：C++ 知识和练习
- `ROS/`：ROS / ROS2 / MoveIt 知识
- `motion_planning/`：运动规划算法、OMPL、RRT、数据生成
- `Git/`：Git 工具学习
- `Inbox/`：暂时不知道放哪里的临时笔记

---

## 建议的整理顺序

不要一次全搬，容易断链接。建议分三轮。

### 第一轮：清理仓库噪声

- [ ] 删除或取消跟踪构建产物
- [ ] 保留源码和真正笔记
- [ ] 确认 Git 状态干净

### 第二轮：移动根目录文件

- [ ] 移动 `tf坐标变换.md`
- [ ] 移动 `Trajectory Execution.md`
- [ ] 移动 `通信协议.md`
- [ ] 移动 `planner.md`
- [ ] 处理空文件 `Sensors Functionality.md`

### 第三轮：重写关键入口页

- [ ] `ROS/00-索引.md`
- [ ] `CPP/00-索引.md`
- [ ] `motion_planning/00-索引.md`
- [ ] `01-项目/机械臂ROS2控制/00-项目首页.md`

---

## 结论

这个 Vault 的知识积累是有价值的，但现在需要从“收集资料”进入“建立知识系统”。

最重要的不是把每篇笔记写长，而是让每篇笔记只有一个明确身份：

- 它是概念笔记？
- 它是项目记录？
- 它是代码文档？
- 它是学习日志？
- 它是临时草稿？

身份清楚后，位置自然就清楚。

