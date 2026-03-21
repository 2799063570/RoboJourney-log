---
type: script
status: 🟢 测试通过
module: debug
dependencies:
  - dataset
  - collision
  - visual_collision
description: 从数据集中读取轨迹数据，进行碰撞检测可视化
---
就是一个测试程序
测试数据集中的数据怎么个情况
这里主要是调用visual_collision中的程序去绘制

1. 因此第一步就是要构建dataset对象去读取数据集
2. 随机设置一个索引去读取数据集中的一条轨迹信息（start，obs）
3. 读取机器人模型文件，设置运动学求解链
4. 设置collision的碰撞检测对象
5. 调用visual_collision函数