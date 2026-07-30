
# ROS1 tf 坐标变换

> 适用范围：本文示例使用 ROS1 `tf` API。新代码优先使用 `tf2_ros`；ROS2 项目使用 `tf2_ros` 与 `geometry_msgs::msg::TransformStamped`，不要直接复制本页头文件。

## 结论

TF 维护的是一棵带时间戳的坐标系树。每条变换都必须明确父坐标系、子坐标系和采样时间；查询失败时，优先检查树是否连通、时间戳是否可用以及查询方向是否写反。

## 使用前的约定

- 命名遵循机器人模型：常见链路为 `map → odom → base_link → ... → tool0`。
- 动态变换由持续运行的 broadcaster 发布；固定安装关系使用静态变换发布器。
- 同一个 child frame 只能有一个父 frame，否则 TF 树不再是树。

在 ROS 的世界里，`tf`（Transform）系统就像是整个机器人的空间定位系统。无论是机械臂的各个关节、相机，还是环境中的障碍物，它们都有自己的坐标系。`tf` 维护这些坐标系之间的相对关系。
`tf` 相关的操作拆解为四大核心模块：**数据构建、发布（建树）、监听（查询）以及空间运算**。

## 数据构建

众所周知，空间变换主要可以分为平移和旋转，平移使用一个三维向量即可，旋转则使用四元数表示。

- `tf::Vector3` (三维向量) 对应平移
```cpp
tf::Vector3 translation(1.0, 0.5, 0.0); // 创建一个平移向量
translation.setX(2.0);                  // 单独修改X轴
```
- `tf::Quaternion` (四元数) 对应旋转
```cpp
tf::Quaternion q;
// 最常用的方法：通过欧拉角(Roll, Pitch, Yaw)来设置，内部会自动转成四元数
q.setRPY(0, M_PI_2, 0); 
// 将四元数标准化（防止多次矩阵乘法后精度丢失导致畸变）
q.normalize();
```
- `tf::Transform` (变换矩阵) 平移和旋转的整合
```cpp
tf::Transform transform;
transform.setOrigin(tf::Vector3(0.1, 0.2, 0.3)); // 塞入平移
transform.setRotation(q);                        // 塞入旋转
```

## 坐标系的发布

当我们计算出某个东西的位置，我们需要将其发布出去，让其他节点可以看到（挂到TF树上）
- 核心类：`tf::TransformBroadcaster`
- 常用函数：`sendTransform()`
- `tf::StampedTransform`来表示坐标变换(变换矩阵(`transform`), 时间戳, 父坐标系, 子坐标系)

```cpp
#include <tf/transform_broadcaster.h>

// 1. 创建发布者（通常作为类的成员变量）
tf::TransformBroadcaster br;

// 2. 准备上面提到的 tf::Transform 变换数据
tf::Transform transform;
transform.setOrigin(tf::Vector3(0.5, 0.0, 0.0));
tf::Quaternion q;
q.setRPY(0, 0, 0);
transform.setRotation(q);

// 3. 加上时间戳和父子关系，打包成 StampedTransform 并发布
// 参数依次为：变换矩阵、时间戳、父坐标系、子坐标系
br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "world", "camera_link"));
```

## 坐标系的监听与查询 (Listener)

向 TF 树询问“此时此刻，A 相对于 B 在哪？”

- **核心类**：`tf::TransformListener`
- **常用函数**：`waitForTransform()` 和 `lookupTransform()`。**注意：它们通常要成对使用！**

```cpp
#include <tf/transform_listener.h>

tf::TransformListener listener;
tf::StampedTransform transform;

try {
    // 【极其关键的一步：等待】
    // 询问从 "world" 到 "ee_link" 在当前时刻的变换，最多等待 3.0 秒。
    // 如果不加这一行，由于网络延迟，lookup 往往会因为查不到最新数据而报错崩溃。
    listener.waitForTransform("world", "ee_link", ros::Time(0), ros::Duration(3.0));

    // 【执行查询】
    listener.lookupTransform("world", "ee_link", ros::Time(0), transform);
    
    ROS_INFO("获取成功！X坐标: %.2f", transform.getOrigin().x());
} catch (tf::TransformException &ex) {
    ROS_ERROR("TF查询报错: %s", ex.what());
}
```

💡 避坑指南：`ros::Time(0)` vs `ros::Time::now()`
	在查询（lookup）时，极力推荐使用 `ros::Time(0)`，它代表“获取当前 TF 缓存树中能拿到的最新数据”。如果你用 `ros::Time::now()`，系统会严格去查极其精确的“此刻”的数据，但由于计算和通信必然存在几毫秒的延迟，往往会抛出 `Extrapolation Exception`（外推异常）报错。

## 强大的空间数学运算

重载了 C++ 的运算符，让三维空间的矩阵乘法变得像做算术题一样简单。
- **坐标系连乘推导** (使用 `*` 乘号)
假设你知道 A 相对世界 (World) 的坐标，又知道 B 相对 A 的坐标，你想求 B 相对世界的坐标，只需要把它们乘起来：
```cpp
tf::Transform world_to_A = ...;
tf::Transform A_to_B = ...;

// 直接相乘！注意顺序：父坐标系在左边，子坐标系在右边
tf::Transform world_to_B = world_to_A * A_to_B;
```
- **求逆矩阵** (使用 `.inverse()`)
假设 TF 树里只发布了“相机(camera) 到 目标(object)”的变换（比如 `camera_to_obj`）。但你的逻辑需要知道“站在目标物体的视角看，相机在哪？”：
```cpp
tf::Transform camera_to_obj = ...;
// 一键求逆，瞬间得到 obj_to_camera
tf::Transform obj_to_camera = camera_to_obj.inverse();
```
- **对空间点进行变换** (使用 `*` 乘号)
假设相机画面里有一个点 `p_camera (1.0, 0, 0)`，你想知道这个点在世界坐标系下的坐标：
```cpp
tf::Vector3 p_camera(1.0, 0.0, 0.0);
tf::Transform world_to_camera = ...; // 已知相机在世界中的位姿

// 直接将点与变换矩阵相乘！
tf::Vector3 p_world = world_to_camera * p_camera;
```

## 命令行调试神器

- **打印树状图（检查树是否断开）**：`rosrun tf view_frames`
- **实时查看两个坐标系的关系**：`rosrun tf tf_echo world ee_link`
- **图形化查看连通性**：`rosrun rqt_tf_tree rqt_tf_tree`

## 常见错误

| 现象 | 优先检查 |
| --- | --- |
| `Lookup would require extrapolation` | 查询时刻不在缓存范围内；先试 `ros::Time(0)`，再检查发布频率和系统时间。 |
| 找不到 transform | 父子 frame 名是否一致、是否启动 `robot_state_publisher`、树是否连通。 |
| 变换方向错误 | 明确“目标坐标系相对于源坐标系”的含义，不要只凭变量名猜乘法顺序。 |
| RViz 模型不动 | `/joint_states`、URDF joint 名和 `robot_state_publisher` 是否匹配。 |

## 关联笔记

- [[ROS常用指令]]
- [[../MoveIt/README|MoveIt]]
