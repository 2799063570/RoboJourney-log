---
type: script
module: debug
dependencies:
  - matplotlib
status: 🔴 存在Bug
description: 可视化机械臂碰撞现场 (基于 CollisionEngine 实现)
---
该程序主要实现了一个函数`visualize_collision_scene`

```python
def visualize_collision_scene(
        engine, # (CollisionEngine) 
        pred_joints, # (Tensor[B, 6])
        obs_params, # (B, N_obs, 7)
        title="...",  # (str)
        show_frames=None,  # (str|list|None)
)
```
- `CollisionEngine`：已初始化好的碰撞引擎实例。引擎内部包含了运动学链、表面点云和安全距离配置。
- `pred_joints` ：待检测的机器人关节角度（弧度），通常来自网络预测。
- `obs_params`: 当前场景的障碍物参数 `(B, N_obs, 7)`
- `title`: 弹出的 3D 窗口标题
- `show_frames`: 是否绘制各连杆坐标系。'all' -> 全部绘制; list  -> 仅绘制名单中的 link；None  -> 不绘制。

 
执行流程如下：
1. 正向运动学解算(夹爪关节设置默认值0), 求解各个点云的当前位置
2. 利用engine去求解机器人点云的碰撞情况
3. 根据点云碰撞情况去判断哪些link发生碰撞
4. 建立一个3D画板，去绘制每一个link，绘制碰撞点

BUG：自碰撞检测的结果有误