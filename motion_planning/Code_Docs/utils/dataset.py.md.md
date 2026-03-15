---
type: script
status: 🔴 存在Bug
module: utils
dependencies:
  - PyTorch
description: 加载数据集数据，并进行相关的处理
---
本文件主要实现了一个类MotionDataset 用于读取数据集文件(path、obs)csv文件, 可以**通过索引访问到要训练的参数(start, goal, short_history, history, obs)** 
在训练过程中我们发现, 路径点之间的间隔太短了, 导致预测的时候不动就可以得到较小的损失。所以我们需要差分路径点, 去除多余的点, 这个过程需要用到RDP算法的实现相关的参数配置有
- 自适应模式：adaptive_eps_ratio(0.01), adaptive_step_ratio(10.0), hard_limit_step(2.0)
- 分别对应 自适应阈值系数、自适应最大步长系数、最大步长限制
- 手动模式: manual_epsilon(0.1), manual_max_step(0.5) 分别对应阈值和最大步长


先查找指定的目录，查看目录下有哪些文件，将这些文件的数据都读取

```python
return {
            'obs_params': torch.from_numpy(obs_data),       # 【MAX_OBS, 7】
            'short_history': torch.from_numpy(short_history), # 【history_window, 12】
            'history': torch.from_numpy(hist_data),         # 【max_hist_len, 6】
            'current': torch.from_numpy(curr),              # 【6】
            'goal': torch.from_numpy(goal),                 # 【6】
            'target': torch.from_numpy(target_traj),  # next states 【pred_horizon, 6】
            'start': torch.from_numpy(start_state)          # 【6】
        }
```
