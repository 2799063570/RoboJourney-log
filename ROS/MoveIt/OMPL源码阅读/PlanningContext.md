# PlanningContext：一次规划请求的执行对象

## 结论

每个 `PlanningContext` 对应一次具体的规划任务。它接收规划场景、起点/目标/约束等请求数据，调用算法求解，并把结果写回 `MotionPlanResponse`。Context 不应承担全局插件加载或长期配置管理，那是 `PlannerManager` 的职责。

## 实现算法

```cpp
#include <moveit/planning_interface/planning_interface.h>

class MyAlgoContext : public planning_interface::PlanningContext {
public:
    MyAlgoContext(const string& name, const string& group) 
        : planning_interface::PlanningContext(name, group) {}

    // 【核心】这里是 MoveIt 调用你算法的唯一入口！
    // 你的 OMPL 魔改版、深度学习推理，全都要包装在这个函数里
    void solve(planning_interface::MotionPlanResponse &res) override {
        ROS_INFO("我的算法开始计算路径了...");
        
        // 1. 获取起点和终点
        // 2. 运行你的算法 (Run Your Algorithm)
        // 3. 把结果填入 res
        
        res.error_code_.val = moveit_msgs::MoveItErrorCodes::SUCCESS;
    }

    void clear() override {}
    bool terminate() override { return true; }
};
```

查看以上的程序模版，我们可以发现主要实现一个继承于`planning_interface::PlanningContext`的子类
实现自己算法的独特功能，但是大致的框架依然规定好了

首先我们看构造函数
`MyAlgoContext(const string& name, const string& group)  : planning_interface::PlanningContext(name, group)`

这里输入了两个参数： 规划器的名称和规划组的名称，同时呢这也是父类构造所需要的参数
当然我们可以属于一些其他参数，例如机器人模型、神经网络模型



主要分为以下几个方面：

- solve(核心求解)：
```cpp
 bool solve (planning_interface::MotionPlanResponse &res) override;
```

	- 
- clear
- terminate


具体的任务肯定是要交给planner去实现的，那么planner如何实现呢
这里主要是纯数学的方面，要交给OMPL中的规划器
而我们要实现算法时，需要继承 OMPL 中的规划器基类。自定义实现骨架见 [[motion_planning/OMPL/自定义Planner骨架|自定义 Planner 骨架]]。
