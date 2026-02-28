#算法实现
💡真正干活的工作，具体的算法实现的程序
🛡️每一个规划请求（Request）都会生成一个独立的 Context 实例。
🥊核心功能主要围绕着 **“如何把 MoveIt 的数据喂给你的算法”** 以及 **“如何把算出来的路径还给 MoveIt”**。

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

主要分为以下几个方面：

- solve(核心求解)：`bool solve(planning_interface::MotionPlanResponse &res) override;`
	- 
- clear
- terminate

