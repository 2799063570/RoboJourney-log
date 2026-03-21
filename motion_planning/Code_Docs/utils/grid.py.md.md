---
type: script
module: utils
dependencies:
  - numpy
  - PyTorch
status: 🟢 测试通过
description: 将物理世界的浮点坐标转化为神经网络能直接处理的三维 0/1 张量矩阵
---
整个程序就是为了维护一个三维 0/1 张量矩阵（96，96，64）来表示障碍物以及采样过的点
所以程序无非也就是根据障碍物信息得到一个矩阵
根据采样点信息得到一个矩阵

将信息打印出来，画出来

实现的功能：
    1）初始化网格：
        init_grid_with_obstacles：根据`obs_params[batch, N, 6]`初始化`envs_grid`;  障碍物
        update_grid_with_point：根据目前的栅格和新增点(单个点)`[batch, 6]`初始化`self_grid`;
        update_grid_with_points：上一个函数的复用版本, 区别在于是多个点`[batch, N, 6]`
        gpu_rasterizer：同时初始化`envs_grid`和`self_grid`, 根据`obs_params[batch, N, 6]`和`points[batch, N, 6]`
    2）绘图(将加载好的栅格绘制出来)
        visualize_voxel_grid：输入栅格即可
    3）投影信息(计算将障碍物或是路径点投影到栅格中 对应的栅格id以及栅格数目(尺寸))
        obs_params：障碍物信息`obs_params[N, 6]`
        print_path_inf：路径点`points[N, 6]`
    需要注意的是所有的函数都用到了栅格的分辨率(grid_res)和工作区域(bounds) 分别都设置了对应的默认值`(96, 96, 64)`、`[(-1, 1), (-1, 1), (-0.05, 1.2)]`
    同时呢, 前两种实现的函数需要带batch的, 关于路径点的需要正向运动学, 需要对应的运动链


  
## 🏗️ 核心功能模块

  
### 1. 通用环境光栅化 (`init_grid_with_obstacles`)

纯 GPU 加速的碰撞体素生成器，避免了传统的 `for` 循环迭代。

- **用途**：将几何体表示的障碍物集合转化为体素矩阵（0/1 占用图）。
- **核心逻辑**：基于物理边界区间 `bounds` 和给定分辨率生成密致的坐标场（`Meshgrid`）。利用点云的距离不等式同时对 Box、Sphere、Cylinder 形状进行体内判定。
- **输入**：`obs_params` Tensor `(Batch, NumObs, 7)`。
	- `obs_params`: `Tensor (B, NumObs, 7)`。障碍物的批次和参数集合。`7` 个维度分别是：类型 (0=Box, 1=Sphere, 2=Cylinder)，物理坐标 $(x, y, z)$，尺寸参数 $(d_1, d_2, d_3)$。
	- `grid_res`: `tuple (nx, ny, nz)`。网格的划分密度，默认为  `(96, 96, 64)` 分辨率。
	- `bounds`: `list [(x_min, x_max), (y_min, y_max), ...]` 。物理世界盒子的真实尺寸边界，默认为 $[-1, 1], [-1, 1], [-0.05, 1.2]$。
	- `device`: `str`。使用的计算设备（默认 `'cuda'`）。
- **输出**：单通道栅格张量 Tensor `(Batch, 1, nx, ny, nz)`。

### 2. 单一姿态投影 (`update_grid_with_point`)

- **用途**：在已有的环境底图上，将机器人的**当前状态**投影为一个体素点。
- **核心逻辑**：接收 $6$ 自由度的关节角数组，通过正向运动学（`FastFK`）极速解算出末端在世界坐标系下 $x,y,z$ 的标量，利用归一化线性映射直接找到其对应的栅格 `index`，并将对应体素点亮设为 $1.0$。
- **输入参数**：
	- **arm_chain**: 运动学链对象
	- **grid**:` (B, C, nx, ny, nz) `网格
	- **current_joint**: `(B, 6) `关节角
	- **bounds**: 必须传入！格式` [(-1, 1), (-1, 1), (-0.05, 1.2)] `务必与 `init_grid` 保持一致
	- **device**: 'cuda' 或 'cpu'
        需要注意的是保持gird和current_joint的batch是一致的
- **输出**：单通道栅格张量 Tensor `(Batch, 1, nx, ny, nz)`。

### 3. 时间序列投影 (`update_grid_with_points`)

- **用途**：`update_grid_with_point` 的序列化/批处理扩展。
- **核心逻辑**：一次送入多个时刻的关节角度组成的路径（例如长度为 $T$ 的路径历史残影），批量投影到三维网格中，生成轨迹连线效果。

### 4. 神经网络专用高射炮 (`gpu_rasterizer`)

数据集 `data_loader.py` 中被最高频调用的核心流水线，专为 3D 模型设计的融合器。

  
- **用途**：一次性产出双通道包含时空信息的感知矩阵。
- **通道分配** （batch，channel，96，96，64）：
  - `Channel 0` (环境感知)：全量映射 `obs_params` 静态障碍物。
  - `Channel 1` (时间与流向感知)：截取并映射由于网络需要分析运动趋势而输入的短历史时间序列张量 `history_paths` 中的纯位置信息流（前 6 维）。

- **输入**：包含 `ARM_CHAIN` (运动学引擎)、`obs_params` (障碍物)、`history_paths` (历史动作帧)。
- **输出**：双通道特征张量 Tensor `(Batch, 2, nx, ny, nz)` 供 3D-CNN 直接吞吐推理。

---
## 🎨 调试与可视化组件

### 1. PyVista 3D 渲染器 (`visualize_voxel_grid`)

- **用途**：交互式的 3D 体素网格查看器，用于数据纠错。
- **特性**：
  - 放弃 Matplotlib，采用基于 OpenGL 底层的 `pyvista` 组件提速渲染百万级网格块。
  - **颜色编码**：
    - 🔴 **红色半透明**：环境障碍物 (`Channel 0`)。
    - 🔵 **蓝色实心**：机器人运动轨迹 / 历史点云 (`Channel 1`)。
    - 🟡 **黄色高亮**：**碰撞交集区**，当蓝色点重叠进红色块区域时高亮报警（使用按位与运算逻辑实现）。
  - **防呆设计**：内部对 `.glyph()` 调用加入了 `orient=False` 防止非矢量标量数据抛出 UserWarning 污染控制台。

### 2. 坐标转换质检员 (`print_obs_inf` 与 `print_path_inf`)

  
- **用途**：精准拦截物理世界坐标系越过虚拟盒子边界导致 Tensor 索引越界的灾难性问题。
- **功能**：终端对比印制详细表格，输出从标量长度映射到体素分辨率步长缩放比例后的大小尺寸，遇到不在界线范围的立刻高亮抛出 `❌ OUT` 诊断。

---

## ⚠️ 开发与维护注意事项

  
1. **坐标系对齐**：物理坐标系的 `bounds` 与网络的分辨率（目前为 `96,96,64`）在本项目中严格挂钩（例如，X 轴从 $-1 \sim 1$ 米跨越了 $96$ 个像素格子）。
2. **尺寸裁剪**：在 `gpu_rasterizer` 输入 `history_joints` 时，注意代码内部硬编码了 `[:, :, :6]` 的切片动作 —— 无论送进框架的历史张量是否携带速度或加速度信息（如 $12$ 维或 $14$ 维），最终仅使用前 $6$ 个关节位置交由运动学引擎渲染物理位置。


## 💡 待优化的方面

感觉将很多的路径点变为栅格 使用for循环的方式不太高效