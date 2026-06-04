有个开关
fake_execution

对应的插件是：
- `moveit_fake_controller_manager/MoveItFakeControllerManager`
- `moveit_simple_controller_manager/MoveItSimpleControllerManager`

一个“伪造”的控制器管理器。它的核心作用是在没有真实硬件和复杂物理引擎的情况下，让 MoveIt 认为它正在控制一个机器人。
```yaml
controller_list:
  - name: fake_arm_controller
    type: $(arg fake_execution_type)
    joints:
      - shoulder_joint
      - upperArm_joint
      - foreArm_joint
      - wrist1_joint
      - wrist2_joint
      - wrist3_joint
  - name: fake_gripper_controller
    type: $(arg fake_execution_type)
    joints:
      - joint1
      - joint2
initial:  # Define initial robot poses.
  - group: arm
    pose: zero
  - group: gripper
    pose: open
```
- **主要用途**：
    - **快速原型开发与可视化**：在 RViz 中纯视觉验证运动学模型（URDF）、碰撞检测（SRDF）和路径规划算法。
    - **离线调试**：在开发初期，开发者不需要连接真实的机器人，甚至不需要启动类似 Gazebo 这样的重型物理仿真器。
    - **持续集成 (CI)**：在自动化测试中快速验证 MoveIt 配置的正确性。
        
- **工作原理**： 当 MoveIt 下发一条规划好的轨迹（Trajectory）时，Fake Controller Manager 并不会将任何指令发送给外部系统。相反，它会在内部“假装”执行了这条轨迹。它会根据轨迹点的时间戳，直接更新并发布机器人的虚拟关节状态（Joint States），从而在 RViz 中产生机器人正在运动的动画效果。当轨迹的持续时间结束后，它会向 MoveIt 返回一个“执行成功”的信号。
    
- **优势**： 极其轻量级，启动速度极快，配置简单（不需要配置 PID 参数或底层硬件接口），非常适合算法验证和 UI 演示。


这是 MoveIt 中**最标准、最常用**的控制器管理器实现。当你需要让真实的机器人动起来，或者在拥有真实物理动力学的仿真器（如 Gazebo、Isaac Sim）中控制机器人时，必须使用它。

- **主要用途**：
    
    - 连接**真实机器人硬件**。
    - 连接**物理仿真器**（通常配合 `ros_control` 或 `ros2_control` 使用）。
        
- **工作原理**： Simple Controller Manager 充当了一个 ROS Action Client（动作客户端）的角色。它会读取特定的配置文件（通常是 `ros_controllers.yaml` 或 `controllers.yaml`），从而知道底层系统暴露了哪些控制器。 当 MoveIt 需要执行轨迹时，它会将路径点打包，通过 ROS 标准的 Action 接口（例如 `control_msgs/FollowJointTrajectory` 或 `control_msgs/GripperCommand`）发送给底层的控制器服务器（Action Server）。在发送后，它会持续监控底层返回的反馈（Feedback）和最终状态（Result），以判断轨迹是否成功执行、是否被中断或是否发生误差过大的故障。
    
- **核心特性**：
    
    - **支持多种接口**：默认支持 `FollowJointTrajectory`（用于机械臂关节运动）和 `GripperCommand`（用于简单的两指夹爪）等标准接口。
    - **真实的闭环监控**：它依赖于底层控制器（如 `JointTrajectoryController`）来处理实际的插补和电机闭环控制，并通过 Action 机制监控真实进度。



```yaml
controller_manager_ns: controller_manager
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