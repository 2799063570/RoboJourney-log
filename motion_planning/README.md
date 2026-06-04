# 运动规划入口

这里存放运动规划算法、OMPL、RRT 修改、自写规划代码和代码文档。

## 当前用途

这个目录暂时不是 ROS2 机械臂控制项目的第一优先级，但它会成为后续 MoveIt/OMPL 深入理解的基础。

推荐顺序：

1. 先完成 [[ROS/README|ROS2 控制主线]]
2. 再阅读 OMPL 与 MoveIt 的接口关系
3. 最后整理自定义 Planner 和 RRT 修改实验

## 主要笔记

| 主题 | 笔记 |
|---|---|
| 自定义 OMPL Planner | [[OMPL/自定义Planner骨架]] |
| RRT 修改 | [[modifyRRT]] |
| 自写运动规划算法 | [[motion planning algorithm self]] |
| 数据生成 | [[数据生成程序]] |
| 代码文档索引 | [[Code_Docs/README_MAP.md]] |

## 和 ROS/MoveIt 的关系

运动规划这部分需要分清三层：

| 层级 | 关注点 |
|---|---|
| 算法层 | 采样、碰撞检测、路径搜索、路径平滑 |
| OMPL 层 | Planner 接口、SpaceInformation、ProblemDefinition |
| MoveIt 层 | Planning Pipeline、PlanningContext、插件加载、轨迹执行 |

不要一开始就把三层混在一起。先用纯算法理解路径生成，再看 OMPL 接口，最后接 MoveIt。

## 待整理问题

- `Code_Docs` 下有不少 `.md.md` 文件名，后续应统一重命名。
- 部分文档是代码说明，部分是学习笔记，建议拆成 `算法笔记` 与 `代码文档` 两类。
- OMPL 与 MoveIt-OMPL 的内容目前分散在 `motion_planning` 和 `ROS/moveit-ompl`，后续可以建立交叉索引。
