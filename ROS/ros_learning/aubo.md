# 对aubo源码进行解析

## action server创建

该文件位于aubo_controller包下的joint_trajectory_action.cpp文件
可以看到是的程序的实现主要基于industrial_robot_client命名空间下的joint_trajectory_action的命名空间下的
JointTrajectoryAction的实现

首先先看类的定义：

```cpp
class JointTrajectoryAction
{
public:
    JointTrajectoryAction(std::string controller_name); //constructor
    ~JointTrajectoryAction();//destructor
    void run() { ros::spin(); } // 等待回调函数
    void abort() { ros::shutdown();}

private:
    typedef actionlib::ActionServer<control_msgs::FollowJointTrajectoryAction> JointTractoryActionServer;
    ros::NodeHandle node_;  // 节点句柄
    JointTractoryActionServer action_server_;
    ros::Publisher pub_trajectory_command_;     // Publishes desired trajectory (typically to the robot driver)
    ros::Subscriber sub_trajectory_state_;      // subscribes traj state (robot driver)
    ros::Subscriber sub_robot_status_;          // subscribes robot state (robot driver)
    ros::Subscriber trajectory_execution_subs_; // subscribes traj (user)
    ros::Timer watchdog_timer_;                 // 记录robot driver响应时间
    bool controller_alive_;
    bool has_active_goal_;
    bool has_moved_once_;
    JointTractoryActionServer::GoalHandle active_goal_; // cache of traj goal
    trajectory_msgs::JointTrajectory current_traj_;
    static const double DEFAULT_GOAL_THRESHOLD_;// = 0.01;
    double goal_threshold_;
    std::vector<std::string> joint_names_;
    control_msgs::FollowJointTrajectoryFeedbackConstPtr last_trajectory_state_; // 轨迹信息（各关节角度、期望角度、误差等）
    industrial_msgs::RobotStatusConstPtr last_robot_status_;        // 机器人的状态（执行、停止、故障等）
    ros::Time time_to_check_;       // 设置延迟检查目标完成状态的时间点，用于避免在轨迹执行初期就过早判断目标是否完成
    static const double WATCHDOG_PERIOD_;// = 1.0;  看门狗的定时周期

    void watchdog(const ros::TimerEvent &e);
    void goalCB(const JointTractoryActionServer::GoalHandle &gh);
    void cancelCB(const JointTractoryActionServer::GoalHandle &gh);
    void controllerStateCB(const control_msgs::FollowJointTrajectoryFeedbackConstPtr &msg);
    void robotStatusCB(const industrial_msgs::RobotStatusConstPtr &msg);
    void trajectoryExecutionCallback(const std_msgs::String::ConstPtr &msg);
    void abortGoal();
    bool withinGoalConstraints(const control_msgs::FollowJointTrajectoryFeedbackConstPtr &msg,
                             const trajectory_msgs::JointTrajectory & traj);
};
```

### actionlib类

通过按照ROS中提供的actionserver模板来实现自己的actionserver类的实现
actionlib::ActionServer<control_msgs::FollowJointTrajectoryAction> 起别名为 JointTractoryActionServer

actionlib::ActionServer 为ROS中的actionlib库中定义的模板类，用于实现动作服务器（Action Server），负责接收客户端的动作目标（Goal）、处理目标、反馈执行状态，并返回结果。
模板参数为control_msgs::FollowJointTrajectoryAction，FollowJointTrajectoryAction 是 ROS 标准消息（control_msgs包）中定义的关节轨迹跟随动作包含以下信息：

- 目标（Goal）：FollowJointTrajectoryGoal，包含机器人需要执行的关节轨迹（关节名称、位置点、时间信息等）。
- 反馈（Feedback）：FollowJointTrajectoryFeedback，实时反馈当前关节位置、速度等执行状态。
- 结果（Result）：FollowJointTrajectoryResult，返回动作执行的最终状态（成功、失败及错误码等）。

对于JointTractoryActionServer的初始化输入参数有

- node_（节点句柄）
- controller_name（控制器名称 action的客户端会根据这个名称连接）
- boost::bind(&JointTrajectoryAction::goalCB, this, _1) 目标回调函数（传入回调函数地址及对应的对象地址，_1对应占位符）
- boost::bind(&JointTrajectoryAction::cancelCB, this, _1) 取消回调函数
- false （自动启动标志位，若为false则需手动调用对象.start()启动服务器，ture则在构造函数后直接启动）
                   
那么boost::bind是什么呢？
Boost库的bind函数可以生成一个回调函数处理器，用于将类的成员函数goalCB绑定到特定的对象实例（this），并指定参数传递方式。指定this对象地址后，方便回调函数调用类中的其他功能函数。

### 节点句柄

私有节点句柄： ros::NodeHandle pn("~");
全局节点句柄： ros::NodeHandle node_;

对于私有节点句柄会对应命名空间路径是当前节点名称 如节点名称为aubo_joint_follow_action则一些话题或参数服务器名称前会自动加上这层路径，例如`pn.param("constraints/goal_threshold", goal_threshold_, DEFAULT_GOAL_THRESHOLD_);`，访问的路径为`/aubo_joint_follow_action/constraints/goal_threshold`的参数，若goal_threshold参数未设置，则会使用默认值DEFAULT_GOAL_THRESHOLD_

全局命名空间节点句柄，默认访问根路径（/）下的参数或话题（如 node_.advertise(...) 发布的话题在全局命名空间）
例如`node_.param("constraints/goal_threshold", goal_threshold_, DEFAULT_GOAL_THRESHOLD_);`，访问的路径为`/constraints/goal_threshold`的参数，若goal_threshold参数未设置，则会使用默认值

### getJointNames

获取各个关节的名称，主要使用到industrial_utils 库中的工具函数getJointNames

```cpp
industrial_utils::param::getJointNames(
  "controller_joint_names",  // 参数1：控制器关节名称参数名
  "robot_description",       // 参数2：机器人模型描述参数名
  joint_names_               // 参数3：输出变量（存储关节名称的容器）
)
```

会从参数服务器中获取(controller_joint_numes)变量对应的关节名称，如果参数服务器中没有对应的变量，那么会从robot_description服务器中解析机器人的模型描述文件，来解析URDF模型中各个关节的名称

### action goal

server通过goalCB回调函数接收路径信息，交给各个控制器执行，发布的数据类型为JointTractoryActionServer::GoalHandle

**JointTractoryActionServer::GoalHandle**是动作服务器（Action Server）中用于管理单个轨迹目标生命周期的句柄类。它封装了客户端发送的目标数据，并提供了修改目标状态（如接受、拒绝、完成等）的接口，是动作服务器与客户端之间交互的核心载体。

包含有轨迹数据(trajectory_msgs::JointTrajectory)和目标约束(goal_time_tolerance, goal_tolerance, path_tolerance)
例如通过 active_goal.getGoal()->trajectory; 获取轨迹信息
同时提供一些方法更新目标的执行状态，反馈给客户端：

- setAccepted(): 标记目标为"已接受"（服务器开始处理该目标）
- setRejected(): 标记目标为"已拒绝"（目标无效，如关节名称不匹配）
- setSucceeded()：标记目标为"已成功完成"（轨迹执行完毕且满足约束）
- setAborted()：标记目标为"已中止"（执行中出现错误，如急停）
- setCanceled()：标记目标为"已取消"（客户端请求取消或被新目标打断）

### trajectory_msgs::JointTrajectory

trajectory_msgs::JointTrajectory 是 ROS（Robot Operating System）中用于描述机器人关节运动轨迹的标准消息类型，定义在 trajectory_msgs 功能包中。主要用于在机器人系统中传递一系列关节在不同时间点的目标状态（位置、速度等），是轨迹规划与执行之间的核心数据载体。

```cpp
// 标准消息头（包含时间戳和坐标系信息）
std_msgs/Header header

// 轨迹涉及的关节名称列表（字符串数组）
string[] joint_names

// 轨迹点列表（每个点包含该时刻的关节状态）
trajectory_msgs/JointTrajectoryPoint[] points
    time_from_start//（duration 类型）：从轨迹开始到该点的时间（核心时间信息，用于控制运动节奏）。
    positions//（float64[]）：各关节在该时间点的目标位置（单位通常为弧度，与 joint_names 顺序对应）。
    velocities//（float64[]，可选）：各关节在该时间点的目标速度。
    accelerations//（float64[]，可选）：各关节在该时间点的目标加速度。
    efforts//（float64[]，可选）：各关节在该时间点的目标用力（力矩）。
```

### 看门狗定时器

看门狗定时器(watchdog timer) 用于监控机器人控制器的活性（是否正常发送状态反馈），防止控制器无响应时程序陷入异常状态。

```cpp
watchdog_timer_ = node_.createTimer(
  ros::Duration(WATCHDOG_PERIOD_),  // 定时器周期
  &JointTrajectoryAction::watchdog, // 定时器触发时的回调函数
  this,                             // 回调函数的所属对象（类实例指针）
  true                              // 是否为“一次性”定时器（oneshot）
);
```

会按照定时器周期，每个周期定时的检测一次（进入定时器的回调函数），若为一次性的定时器需要在每次触发后手动开始定时器(start())

### control_msgs::FollowJointTrajectoryFeedbackConstPtr

ROS（Robot Operating System）中用于传递关节轨迹跟踪反馈信息的常量智能指针类型，通常用于回调函数中安全地接收和处理控制器的实时状态反馈。
消息格式主要如下：

```cpp
std_msgs/Header header          // 消息头（包含时间戳和坐标系）
string[] joint_names            // 关节名称列表（与轨迹中的关节对应）
trajectory_msgs/JointTrajectoryPoint actual  // 实际关节状态（位置、速度、加速度等）
trajectory_msgs/JointTrajectoryPoint desired // 期望关节状态（轨迹规划的目标状态）
trajectory_msgs/JointTrajectoryPoint error   // 误差（实际与期望的差值）
```

### industrial_msgs::RobotStatusConstPtr

ROS 中用于传递工业机器人状态信息的常量智能指针类型，主要用于安全接收和缓存机器人的实时运行状态（如运动状态、错误信息、紧急停止状态等），是工业机器人控制逻辑中判断机器人状态的核心数据来源。
主要包括一下几个变量：

- **in_motion**: 类型为industrial_msgs::TriState，来表示机器人是否在运动，取值包括TriState::TRUE（运动中）、TriState::FALSE（已停止）、TriState::UNKNOWN（状态未知）。
- **e_stopped**：类型为 industrial_msgs::TriState，表示机器人是否处于紧急停止状态
- **in_error**：类型为 industrial_msgs::TriState，表示机器人是否处于错误 / 保护停止状态