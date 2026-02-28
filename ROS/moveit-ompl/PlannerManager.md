#实现算法导入 #OMPL算法管理

PlannerManager就是经理，负责将我们写的算法介绍给moveit
它负责读取`ompl_planning.yaml` 配置文件，根据你选的算法（`RRTConnect`, `PRM`...），配置好参数。

```cpp
#include <moveit/planning_interface/planning_interface.h>
#include <class_loader/class_loader.hpp> // 或者是 pluginlib

class LERPPlannerManager : public planning_interface::PlannerManager {
    // 1. 存模型 (成员变量)
    std::shared_ptr<NeuralNetwork> model_; 

public:
    // 2. 初始化：只跑一次，加载大模型
    bool initialize(...) override {
        model_ = std::make_shared<NeuralNetwork>("model.pt");
        return true;
    }

    // 3. 资格查验：能不能接单？
    bool canServiceRequest(...) const override {
        return true; 
    }

    // 4. 工厂方法：创建干活的 Context，并把模型传给它
    planning_interface::PlanningContextPtr getPlanningContext(...) const override {
        // 创建 Context，把 model_ 指针传进去复用
        auto context = std::make_shared<LERPPlanningContext>(..., model_);
        // 填充数据
        context->setPlanningScene(planning_scene);
        context->setMotionPlanRequest(req);
        return context;
    }

    // 5. 清理
    void terminate() const override { }
};
// 【这就是你看到的那个宏】
// 注册 MyAlgoManager，而不是 Context
CLASS_LOADER_REGISTER_CLASS(LERPPlannerManager, planning_interface::PlannerManager);
```

三个主要注意的点：
- 继承的类 （planning_interface::PlannerManager）
- 需要实现的虚函数
- 最后的宏声明 （CLASS_LOADER_REGISTER_CLASS）

查看模版实现，我们可以发现我们写的自己算法的经理(`MyAlgoManager`)继承于`planning_interface::PlannerManager`
按照父类的规定实现必须要实现的几个纯虚数
同时能可以发现最后有一个宏`CLASS_LOADER_REGISTER_CLASS(MyAlgoManager, planning_interface::PlannerManager);`

那么需要实现的子函数有那些呢？

- `initialize`(初始化)：`bool initialize(const moveit::core::RobotModelConstPtr& model, const std::string& ns)`
	- 功能：初始启动一次，加载那些耗时、占用内存且整个生命周期资源
	- 例如可以读取机器人模型、加载我们的算法模型、读取规划相关的参数; 创建OMPL_Interface
	- 我们去查看`moveit planner`下的 `ompl interface`的源码可以看到, 不仅有创建OMPL_Interface 还有 创建了动态调参机制`OMPLDynamicReconfigureConfig`, 详细过程可以查看 [C++生存指南] #动态调参机制
	- 算法模型则是通过OMPL_Interface来获取规划器的配置`getPlannerConfigurations`, 详见[PlanningInterface]

- `canServiceRequest` (资格审查)：`bool canServiceRequest(const planning_interface::MotionPlanRequest& req) const`
	- **调用时机**: 当 MoveIt 收到一个规划请求，但还没决定把任务交给谁时
	- **核心功能**: **“检查菜单”**。判断当前的规划器是否有能力处理这个请求。
	- 例如，判断是否是机器人模型是否和我们算法的所用的模型一直
	
- `getPlanningContext` (创建厨师)：
	`planning_interface::PlanningContextPtr getPlanningContext( const planning_scene::PlanningSceneConstPtr& planning_scene, const planning_interface::MotionPlanRequest& req, moveit_msgs::MoveItErrorCodes& error_code) const`
	- **调用时机**: 当 MoveIt 决定使用你的规划器，并且需要开始执行具体的规划任务时
	- **核心功能**: **“指派厨师”**。你需要实例化一个 `LERPPlanningContext` 对象，并将它返回给 MoveIt。
	- **配置模型**：
		- 加载好的 **PyTorch 模型指针**、配置参数传递给 Context
		- 将 `planning_scene` (包含障碍物信息) 和 `req` (包含起点终点) 塞给 Context
		- **返回指针**: 返回 `std::shared_ptr<LERPPlanningContext>`

- `terminate` (终止/清理)：`void terminate() const`
	- **调用时机**: MoveIt 关闭或插件卸载时。
	- **核心功能**: **“打烊”**。释放资源。