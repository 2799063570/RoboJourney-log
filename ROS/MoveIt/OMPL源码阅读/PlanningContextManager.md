# PlanningContextManager：规划上下文缓存与创建

## 结论

构建状态空间、约束采样器和 OMPL 规划上下文可能开销较大。`PlanningContextManager` 负责按规划组与配置复用或创建合适的上下文；复用前仍需用当前 `PlanningScene` 和 `MotionPlanRequest` 更新请求数据，不能把上一条轨迹状态当作当前状态。

💥 创建一个规划器实例（比如 RRT* 算法的一个实例）并为它配置好机器人所在的数学空间是非常耗时的
💡 如果机器人总是用同一个算法在同一个规划组（planning group）中规划，我们就不应该重复创建这些东西

🥊 因此 PlanningContextManager的作用在于两方面：工厂和仓库
- 工厂：负责建造context
- 仓库：负责存储context

🚀 工作流程：
- interface 给他配置需求，让PlanningContextManager提供一个context
- PlanningContextManager 会先在自己的仓库中进行查找
- 复用或者创建
	- 如果有的话直接拿出来使用
	- 如果没有创建一个新的存储在仓库中，并拿出来使用

主要依赖

- **`RobotModel`**：它需要机器人的模型信息来构建正确的状态空间。
- **`ConstraintSamplerManager`**：用于处理路径约束，它会将约束采样器管理器传递给它创建的每个规划上下文。

当我们要解决一个路径规划问题，我们需要一个完整的"规划上下文"（机器人规划的必要信息）
- 状态空间
- 有效状态检测器
- 规划器
- 目标
- 其他设置

维护的最重要的变量: `CachedContexts`
```cpp
struct PlanningContextManager::CachedContexts
{
  std::map<std::pair<std::string, std::string>, std::vector<ModelBasedPlanningContextPtr> > contexts_;
  std::mutex lock_;
};
```

那么我先需要了解`ModelBasedPlanningContextPtr` 

在ROS中，`ModelBasedPlanningContextPtr` 往往是`ModelBasedPlanningContext`智能指针
该智能指针是通过[C++生存指南] #栈展开 来实现的

`ModelBasedPlanningContext`继承于 `planning_interface::PlanningContext`

#### 构造函数
```cpp
PlanningContextManager(moveit::core::RobotModelConstPtr robot_model,
                       constraint_samplers::ConstraintSamplerManagerPtr csm);
```
**必须传入两个核心依赖**：

1. **`RobotModel`**：机器人运动学模型（来自 URDF/SRDF），所有规划都基于机器人结构；
2. **`ConstraintSamplerManager`**：约束采样管理器（用于路径约束、目标约束的采样）。


