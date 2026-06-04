

```cpp
#include <ompl/base/Planner.h>
#include <ompl/geometric/PathGeometric.h>
#include <ompl/base/goals/GoalSampleableRegion.h>

namespace ompl
{
namespace geometric
{

class MyCustomPlanner : public base::Planner
{
public:
    // ====================================================================
    // 1. 构造函数：写“简历”，并用 declareParam 暴露参数
    // ====================================================================
    MyCustomPlanner(const base::SpaceInformationPtr &si) : base::Planner(si, "MyCustomPlanner")
    {
        // 填写规划器能力说明书 (specs_)
        specs_.approximateSolutions = false; // 不提供近似解
        specs_.directed = true;              // 假设路径是有向的
        
        // 暴露你的自定义调优参数给 MoveIt/yaml
        declareParam<double>("step_size", this, &MyCustomPlanner::setStepSize, &MyCustomPlanner::getStepSize, "0.01:10.0");
    }

    ~MyCustomPlanner() override
    {
        // 释放你自己在求解过程中 new 出来的节点内存
        freeMemory();
    }

    // ====================================================================
    // 初始化与重置
    // ====================================================================
    void setup() override
    {
        base::Planner::setup(); // 必须调用基类的 setup
        // 在这里初始化你的内部数据结构（比如 KD-Tree, 搜索图等）
    }

    void clear() override
    {
        base::Planner::clear(); // 必须调用基类的 clear
        // 清空你的树或图，准备迎接下一次全新的规划请求
        freeMemory(); 
    }

    // ====================================================================
    // 2. 核心求解函数：拿起点、查碰撞、写循环
    // ====================================================================
    base::PlannerStatus solve(const base::PlannerTerminationCondition &ptc) override
    {
        // 检查环境是否已经 setup，起止点是否有效
        checkValidity();

        // [发牌官登场] 获取有效的起点 (可能存在多个起点)
        const base::State *start_state = pis_.nextStart();
        if (!start_state)
            return base::PlannerStatus::INVALID_START;

        // [发牌官登场] 获取目标 (此处假设目标是一个可采样的区域)
        auto goal = pdef_->getGoal().get();
        auto goal_region = pdef_->getGoal()->as<base::GoalSampleableRegion>();

        // 分配一个临时状态，用来存放你在空间中随机采样的点
        base::State *random_state = si_->allocState();

        bool solved = false;

        // 【核心算法循环】：只要没超时 (ptc)，就一直算
        OMPL_INFORM("%s: 开始搜索!", getName().c_str());
        while (!ptc)
        {
            // (1) 产生一个随机状态 (这里只是示意，通常需要专门的 Sampler)
            // si_->getStateSpace()->sampleUniform(random_state);

            // (2) 碰撞检测：问 SpaceInformation 这个点能走吗？
            if (si_->isValid(random_state))
            {
                // (3) 把点加入你的搜索树/图... (省略具体算法逻辑)

                // (4) 检查是否到达目标
                if (goal->isSatisfied(random_state))
                {
                    solved = true;
                    break;
                }
            }
        }

        si_->freeState(random_state); // 释放临时内存

        // ====================================================================
        // 3. 把结果塞回 ProblemDefinition (pdef_)
        // ====================================================================
        if (solved)
        {
            // 创建一条几何路径
            auto path(std::make_shared<PathGeometric>(si_));
            
            // 将你找到的路径点依次塞入 path (这里只是假装塞入起点和终点)
            path->append(start_state);
            // ... 塞入中间节点 ...
            // path->append(goal_state);

            // 塞回 pdef_，告诉 MoveIt 任务完成！
            pdef_->addSolutionPath(path, false, 0.0, getName());
            
            return base::PlannerStatus::EXACT_SOLUTION;
        }

        return base::PlannerStatus::TIMEOUT; // 时间到了还没找到
    }

    // ====================================================================
    // 自定义参数的 Setter 和 Getter
    // ====================================================================
    void setStepSize(double step)
    {
        step_size_ = step;
    }

    double getStepSize() const
    {
        return step_size_;
    }

private:
    void freeMemory() { /* 释放内存逻辑 */ }

    double step_size_{0.1}; // 你的自定义参数
};

} // namespace geometric
} // namespace ompl
```

