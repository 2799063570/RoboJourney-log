
```dataview
TABLE WITHOUT ID
    file.link AS "📜 脚本名称",
    module AS "所属目录", 
    status AS "当前状态", 
    description AS "功能描述",
    dependencies AS "核心依赖",
    dateformat(file.mtime, "MM-dd HH:mm") AS "最后修改"
FROM "motion_planning/Code_Docs" 
WHERE type = "script"
SORT module ASC, file.name ASC
```

## 📚 文档导航

### 根目录

- **(`config/`)** - 机器人的一些参数配置（控制参数、关节限位等）
- **(`data/`)** - 训练所需的训练数据
- **(`docs/`)** - 项目的解释文档
- **(`include/`)** - src中文件依赖的头文件
- **(`config/`)** - launch项目所需启动的程序文件
- **(`meshes/`)** - 机器人的一些模型文件
- **(`motion_planning/`)** - 模型代码程序
- **(`result/`)** - 存放一些程序的结果
- **(`runs/`)** - 存放程序运动过程中日志文件
- **(`scripts/`)** - 模型代码直接运动程序
- **(`src/`)** - 直接调用moveit
- **(`test/`)** - 测试数据集数据
- **(`urdf/`)** - 机器人模型
- **(`weights/`)** - 模型训练的模型权重

### 模型开发

- **(`config/`)** - 一些模型训练、运行相关的参数配置
- **(`debug/`)** - 模型撰写过程对一些数据可视化的程序
	- **[check_paths](./debug/check_paths.py.md)** - 检查数据集中各个关节状态的变化情况，绘制成图
	- **[debug_collision](../motion_planning/debug/debug_collision.py)** - 可视化碰撞检测的结果：从数据集中读取轨迹数据，进行碰撞检测可视化
	- **[fk](fk.py.md.md)** - 借助机器人模型计算机器人的正向运动学【手动指定关节角度，求得对应的末端位姿】

  - **[plot_path](../motion_planning/debug/plot_path.py)** - 生成不同形态（如直线、螺旋）的虚拟测试轨迹数据并保存为 CSV
- [[utils]]   - 一些模型训练、运行相关的工具函数