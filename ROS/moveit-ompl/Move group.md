
四个主要功能：

- 加载机器人参数（[[planning context]]）： 将机器人的URDF和SRDF加载到参数服务器
- 启动规划管道 ([[Planning Pipeline]])：默认使用的是OMPL的Planning Pipeline，在参数服务器设置规划算法相关的参数
- 轨迹执行控制器([[Trajectory Execution]])：负责连接 MoveIt 和底层硬件控制器
- 传感器管理 ([[Sensors Functionality]])：配置 3D 传感器，用于生成 **Octomap**（八叉树地图）进行动态避障。
- 核心节点 ([[move group Node]])：
	- 