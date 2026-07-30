# MoveIt 概览（ROS1）

## 结论

MoveIt 是机械臂运动规划框架，不是电机驱动器。它结合机器人模型、当前状态、环境碰撞信息和规划器生成轨迹；真正让机器人运动的是底层控制器和硬件驱动。

## 核心链路

```text
URDF + SRDF + joint limits + kinematics
                ↓
Planning Scene ← joint_states / TF / 碰撞物
                ↓
move_group → Planning Pipeline / OMPL → RobotTrajectory
                ↓
Trajectory Execution → FollowJointTrajectory action → 控制器 / 机器人
```

## 学习顺序

1. 先完成 [[moveit_tutorials|教程记录]] 中的模型加载、RViz 与规划练习。
2. 阅读 [[../Trajectory Execution|轨迹执行]]，分清规划成功与真实执行成功。
3. 阅读 [[../OMPL源码阅读/move group Node|move_group 节点]]，理解请求如何进入规划管道。
4. 再进入 [[../OMPL源码阅读/Move group|MoveIt-OMPL 源码阅读]] 与 [[motion_planning/README|运动规划算法]]。

## 使用前检查

- `robot_description`、SRDF、规划组、关节限位和运动学配置一致。
- `/joint_states` 与 TF 持续有效，当前状态不是未知或过期值。
- 控制器提供与配置一致的 `FollowJointTrajectory` action。
- 先在 fake execution 或仿真中验证，再连接真机；真机仍需独立的限位、急停和驱动安全策略。
