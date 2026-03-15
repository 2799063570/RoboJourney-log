---
type: script
status: 🟡 开发/调试中
module: utils
dependencies:
  - OMPL
  - arm_points
description: 基于点云和简化包围盒的碰撞检测逻辑
---
主要为一个碰撞检测的类 CollisionEngine



主要函数

`parse_srdf_collision_pairs`： 解析 SRDF 文件, 提取其中的link对
构造函数：传入（机器人正向运动学链、一些参数配置）
	初始化正向运动学链、解析忽略碰撞检测link对、加载机器人点云参数、
	`all_local_points`:  存放所有的点云坐标（【N， 3】）
	`link_ranges`：存放`link`的点云索引范围（例：base_link: (0, 10))
	`link_id_tensor`:  和all_local_points对齐，表示每个点对应的序号
	`idx_a_list`, `idx_b_list`：存储要进行自碰撞检测的点云序号序列
`get_robot_points_flat` : 计算输入N组关节空间的值 得到各个link的坐标，计算出机器人点云的坐标（N，机器人点云个数，3）
`_check_env_collision_vectorized` : 输入经过坐标变化后的机器人点云 和 障碍物参数 返回【N，机器人点云个数(bool)】
`_check_self_collision_vectorized` : 返回【N，1(bool)】
`check_collision` : 调用上述三个函数，先求得点云坐标，调用环境、自碰撞检测 将所有点做与运算求得是否发生了碰撞
`_compute_env_collision_loss`: 计算各个点云点到障碍物的几何距离，利用ReLU函数计算离障碍物表面的距离，在所有障碍物 (`dim=2`) 和所有点云 (`dim=1`) 中找到惩罚力度最大的那个极值穿透量，最后 `.mean()` 把多批次平均掉，并吐给 PyTorch 的梯度计算图。
`_compute_self_collision_loss` ： 在这个长长的 `for` 循环中，它遍历了上面预处理好的、可能发生自相交的配对点云列表。 对每一对连杆组合（比如 Link1 与 Link2），它用极快的 `torch.cdist` 算出这两块“三维散点云团”之间的距离大矩阵，并在庞大的矩阵中抽出最近的一个两两元素距离： `min_dist = dists.view(B, -1).min(dim=1)[0]`