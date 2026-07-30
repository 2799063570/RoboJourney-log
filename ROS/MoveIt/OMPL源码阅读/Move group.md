
# MoveIt `move_group` 架构（ROS1）

## 结论

`move_group` 是 MoveIt 的协调节点：它加载模型和配置，维护规划场景，调用规划管道，并把需要执行的轨迹发送到控制器。它组织能力，而不是单独实现所有规划或驱动逻辑。

## 五个关键职责

1. [[planning context|加载机器人与规划上下文配置]]：提供 URDF、SRDF、关节限位和运动学参数。
2. [[Planning Pipeline|启动规划管道]]：串联请求适配器与 OMPL 等规划器插件。
3. [[Trajectory Execution|执行轨迹]]：连接 MoveIt 与底层 action 控制器。
4. [[../传感器功能与OctoMap|更新感知场景]]：把三维传感器数据转为 Planning Scene 中的障碍物信息。
5. [[move group Node|对外提供协调节点]]：接收 RViz、C++ API 或 action 的规划请求。

## 一次请求的边界

```text
目标与约束 → move_group → Planning Scene + Planning Pipeline → RobotTrajectory
                                                     ↓
                                          控制器 action → 机器人反馈
```

规划、执行和感知相互依赖，但应分别排错：先保证当前状态与 TF 正确，再检查规划器配置，最后检查控制器与硬件反馈。
