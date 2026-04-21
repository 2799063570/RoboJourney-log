
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
- **(`launch`)** - launch项目所需启动的程序文件
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
- **(`model/`)** - 模型架构文件对应的损失函数、RRT算法
	- **`model_cvae`** : **CVAE+MDN 模型**， 四部分架构：环境编码器、路径编码器、变分推断、解码器
	- **`loss_cvae`** :  **CVAE 专属损失函数**，混合高斯分布的负对数似然（NLL）和 KL 散度的组合计算
	- 
- **(`debug/`)** - 模型撰写过程对一些数据可视化的程序

- [[utils]]   - 一些模型训练、运行相关的工具函数
- [[debug]] 一些调试文件
- [[model]]