---
type: script
module: utils
dependencies:
  - numpy
description: 高效正向运动学计算器
status: 🟢 测试通过
---
```python
Usage:
        fast_fk = FastFK(device='cuda')
        ee_pos = fast_fk.forward(joint_angles)  # (B, 3)
```