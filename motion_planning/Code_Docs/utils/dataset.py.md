---
type: script
status: 🔴 存在Bug
module: utils
dependencies:
  - PyTorch
description: 加载数据集数据，并进行相关的处理
---
本文件主要实现了一个类MotionDataset 用于读取数据集文件(path、obs)csv文件, 可以**通过索引访问到要训练的参数(start, goal, short_history, history, obs)** 
```cpp
dataset = MotionDataset(work_dir,
                            env_filename="training_envs.csv",    # 根据实际存在的文件名修改
                            path_filename="training_paths.csv",  # 根据实际存在的文件名修改
                            use_adaptive=True,
                            manual_epsilon=MANUAL_EPSILON,
                            manual_max_step=MANUAL_MAX_STEP,
                            adaptive_eps_ratio=ADAPTIVE_EPS_RATIO,
                            adaptive_step_ratio=ADAPTIVE_STEP_RATIO,
                            hard_limit_step=HARD_LIMIT_STEP,
                            skip_rdp=True)
```

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

```python
get_trajectories_and_obsParms
return trj{shape: (N, 6), 轨迹完整路径点数组}, obs_data{shape: (max_obs, 7), 场景的障碍物信息}
```

那么我们在main也提出了对该程序的验证
这里也是输入我们的数据集下的根目标（数据集类构造函数实现了对根目录下的文件搜索拼接的功能）
rdp相关的参数使用默认的就行，这里我们选择跳过类中的rdp，希望数据集直接输出未删减的路径
在我们设置的`path_index`, 设置了要绘制的路径id列表
然后会通过`get_trajectories_and_obsParms`获取路径点集合
然后回计算rdp相关的参数（自适应参数，手动限制参数）调用rdp获取删减后的路径
然会绘制图像 一图六子图


![[dataset.png]]
可以看到有的路径 上面的点基本就没有删除过