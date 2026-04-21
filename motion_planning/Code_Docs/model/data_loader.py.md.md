---
type: script
status: 🟢 测试通过
dependencies:
  - numpy
  - dataset
description: 读取数据集，对数据进行相关的处理，方便后续调用
module: model
---

`data_loader.py` 是位于原生 `dataset.py` 和神经网络模型大关口之间的一个**过滤与适配层**。

由于 `dataset.py` 的设计不仅要负责读取原始文件，还要处理动态 RDP 简化、提取相对历史等庞杂的基础数据工作，`data_loader.py` 从中剥离出了面向模型端的最通用需求：**将数据从 Dataset 中安全取出，清理“死数据”（例如不存在的凑数障碍物），并直接包装成带 batch 维度的 PyTorch Tensor，送入模型喂料**。

就是输入数据集的地址和所需要的（路径|点）id
根据地址构造一个dataset类

## 🏗️ 核心功能模块

### 1. 基于轨迹维度的提取 (`load_data_with_validation`)

面向类似 RNN 或者全轨迹规划的算法设计。一次性抽出一条完整的运动轨迹线上所有的点。

- **用途**：通过提供全局轨迹的索引 `traj_idx`，把一整条长长的大路径（以及对应的环境）全部拿出来。
- **获取对象**：调用 `dataset.get_trajectories_and_obsParms(traj_idx)` 方法获取 numpy 格式的全局长数据。
- **验证清理（Validation）**：
  在原始采集中，由于每个批次的障碍物数量要求对齐（比如最大 20 个），环境文件使用了大量全为 `0` 的行来充当占位符（或者说这根数据并不存在有效的碰撞体尺寸）。
  - **规则**：检测障碍物参数的第 4 维 (长度 $D_1$)。如果 $D_1 > 1e-6$ 则认为是真实存在的碰撞体；否则，说明这是一个无效的填充物，全部剔除（Mask 掉）。
- **返回值**：
  - `points`: 张量 `(1, N, 6)`。一条轨迹由 `N` 个关节角组成的连续点列。
  - `obs_params`: 张量 `(1, Valid_N, 7)`。经过除脏操作后，只保留真实存在的有效环境碰撞体数据。
  >*如果环境彻底为空，则返回极为空旷的 `(1, 0, 7)`，防止后续的 PyTorch 图计算层因 None 报错。*

### 2. 基于单点维度的提取 (`load_data_points`)

面向类似 MPNet 这类基于当前单点状态预测“下一个单点状态”的序列网络设计。这也是训练模型最核心的加载方式。

- **用途**：给定一个庞大数据集中的随机采样的索引 `point_idx`，精确挖出这一刻“我所在的快照”，以及“我即将去哪里的快照”。
- **获取对象**：调用 `dataset[point_idx]` 原生魔法函数获取。由于底层 `dataset.py` 已经对数据集做了展平映射，返回的是一个打包好的结构化信息字典。
- **包含特征**：
  - **`short_history`**: `Tensor (T, 12)`。带有速度特征 $V$ 的历史残影。
  - **`target`**: `Tensor (6,)`。模型**真正要去预测试图优化的下一个时刻的位置目标**（Ground Truth）。
  - **`current`**: 当前的绝对状态。
  - **`goal` / `start`**: 整个子集路径的终点和起点宏观指引。
- **验证清理（Validation）**：执行与上方轨迹方法一模一样的占位符“剔除除脏”操作。
- **返回值**：目前代码简化为仅返回增加了维度 `unsqueeze(0)` 以适配 Batch 的 `points` 历史快照特征，以及净化过的 `obs_params` 碰撞体特征。

---

## ⚠️ 开发与维护注意事项

1. **测试文件名称挂接**：在代码内部手动实例化 `MotionDataset` 时，使用的是关键字传参：
   `dataset = MotionDataset(data_root=root_dir, env_filename='generated_env.csv', path_filename='generated_path.csv')`。
   如果日后使用真实数据集，或者训练脚本改用其他诸如 `training_envs.csv` 取代这套命名，请注意在此处或者从入参暴露出可配置的接口给文件命名。
2. **设备指派 (Device Assignment)**：这些适配器函数在执行张量转换的过程中 `torch.tensor(..., device=device)` 已经直接指定了显卡/CPU设备。这意味着从这里加载出的数据直接跳过了数据迁移的过程，可以无缝送到基于相同 `device` 的网络上做推理，速度极快。
