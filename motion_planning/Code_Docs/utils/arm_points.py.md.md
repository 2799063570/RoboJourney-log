---
type: script
status: 🟡 开发/调试中
module: utils
dependencies:
  - numpy
  - PyTorch
description: 碰撞检测工具【基于 PyTorch 张量操作实现的向量化碰撞检测引擎，支持 GPU 加速环境碰撞、自碰撞检测及可微的碰撞惩罚 Loss 计算】
---
URDFPointCloudBuilder： **读取描述机器人物理形状的文件（URDF / Xacro 和关联的 3D Mesh 网格），并在其表面“撒点”，从而生成机器人的点云（Point Cloud）表示。**

具体实现过程如下：

1. **模型采样撒点/生成表面点**： **(`_parse_joints, parse_origin`)** 它使用 `xml.etree.ElementTree` 解析 URDF 文件，找出所有连杆（Link）以及它们发生固定连接的关节（Fixed Joint），推算出它们之间的静态几何坐标偏移（齐次变换矩阵）。
2. **模型采样撒点/生成表面点**: **(`sample_mesh`, `generate_primitive_points`)** 对于机器人的每一个 Link，它会去读 `<collision>` 标签里的几何形状
	- 标准几何体
	- Mesh网格（`.STL` / `.DAE` 文件）

主要函数：

1. 构造函数：URDF文件路径、对应包的路径
2. build函数：主要的求点云的函数，输入参数（默认每个link的采样个数、各个link的采样点数的配置字典、缓存路径）
	输出一个字典形式：key（link name），value（N, 3）对应在连杆自身局部坐标系下表面离散点的三维坐标
3. 点云可视化：
	 这里主要通过创建一个URDFPointCloudBuilder对象，获取点云字典
	 借助于正向运动学计算每个连杆的坐标
	 然后通过矩阵运算计算出每个点云的坐标绘制在图上

`target_angles = [0.0, -0.5, 0.5, 0.0, 1.57, 0.0,  0.0, 0.0]`设置一组关节角度值
![[arm_points1.png#img_center]]
