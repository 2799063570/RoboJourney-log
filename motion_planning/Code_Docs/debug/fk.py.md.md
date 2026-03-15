---
type: script
status: 🟢 测试通过
module: debug
dependencies:
  - numpy
  - PyTorch
description: 正运动学计算模块，用于验证机械臂各个关节末端位姿
---

# fk.py 核心逻辑
这里可以记下你推导 DH 参数时的矩阵草稿，或者写下：
> **注意**：这个文件里的正解主要是为了辅助 `collision.py` 做碰撞检测可视化，不要在实际控制循环里调用它，嫌慢的话去调 `fast_fk.py`。