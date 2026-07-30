# MoveIt 轨迹执行与控制器管理器（ROS1）

## 结论

规划成功只说明得到了一条满足规划约束的轨迹；执行成功还要求 MoveIt 能找到覆盖对应关节的控制器，并通过 action 收到有效反馈与结果。`move_group` 是控制器 action 的客户端，不提供电机闭环本身。

## 执行链路

```text
RobotTrajectory
    → Trajectory Execution Manager
    → MoveIt Controller Manager
    → FollowJointTrajectory action client
    → ros_control / 机器人驱动 action server
    → 硬件与 joint_states 反馈
```

## 常见控制器管理器

| 管理器 | 适用场景 | 关键点 |
| --- | --- | --- |
| `MoveItFakeControllerManager` | RViz 演示、配置验证、离线规划 | 只模拟执行，不发送硬件命令。 |
| `MoveItSimpleControllerManager` | 已有 action 控制器的真机或仿真 | 将轨迹发送到 `FollowJointTrajectory` 或夹爪 action。 |

> 上表是 ROS1 MoveIt 的常见配置。MoveIt2 通常与 `ros2_control` 和 `joint_trajectory_controller` 配合，配置文件和启动方式不同。

## 配置示例

```yaml
controller_list:
  - name: aubo_i5/arm_joint_controller
    action_ns: follow_joint_trajectory
    type: FollowJointTrajectory
    default: true
    joints:
      - shoulder_joint
      - upperArm_joint
      - foreArm_joint
      - wrist1_joint
      - wrist2_joint
      - wrist3_joint
```

控制器名、`action_ns`、关节顺序和 `JointTrajectory` 的关节名必须与底层 action server 一致。

## 排错顺序

1. 能否在 RViz 中规划：检查模型、规划组、当前状态和碰撞环境。
2. 规划后能否看到控制器：检查 MoveIt controller 配置是否加载。
3. action server 是否存在：确认控制器启动且 action 名正确。
4. 关节名、数量和顺序是否匹配；关节状态是否持续回传。
5. 真机前先验证限位、速度/加速度缩放、停止策略和恢复流程。

## 关联笔记

- [[OMPL源码阅读/move group Node|move_group 节点]]
- [[传感器功能与OctoMap]]
- [[../ROS1/aubo|AUBO 驱动源码阅读]]
