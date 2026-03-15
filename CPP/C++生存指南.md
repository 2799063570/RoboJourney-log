#CPP积累 #reset #shared_ptr #unique_ptr

**基础知识部分**分为基本C++语法，包含C语言部分、进阶部分、面向对象和STL。该文档只记录了STL部分（[[STL总结]]），其余重要部分以零散笔记的形式记录在下面中以及[[C++ 关键字字典]]、[[C语言 关键字字典]]。
除此之外还有数据结构和STL相对应（[[data_struct]]）, 基础算法（[[algorithm]]）,一些力扣习题（[[朝花夕拾]]）
于26年设置新的计划
- 三月份：[[C++面向对象]]
- 四月份：[[C++现代用法]]
- 五月份：[[STL总结]]
- 六月份：[[并发编程]]

### 左值、右值与它们的引用
#左值右值及其引用

- **左值 (`Lvalue`)** = **有身份** (Has Identity)。
	- 标识非临时性对象的表达式。
    - 能在内存里找到固定的家（地址）。
    - _比喻_：**“有房产证的居民”**。你可以去他家找他，他会长久住在那里。
        
- **右值 (Rvalue)** = **有内容** (Has Value)，但**无身份**。
	- 临时的、转瞬即逝的、没名字的表达式。
	- 临时性对象、字面量等。
    - _比喻_：**“路过的流浪汉”** 或 **“幽灵”**。你只能看到他身上的东西，但你不知道他住哪，他马上就消失。

##### 如何区分左值右值呢？

**取地址测试 (`&`)**
- **能**取地址 `&x` -> **左值**。
- **不能**取地址 -> **右值**。

| **概念** | **代码示例**               | **判定理由**                                         |
| ------ | ---------------------- | ------------------------------------------------ |
| **左值** | `int a = 10;`          | 可以写 `&a`，能找到它的内存地址。                              |
| **右值** | `10`                   | ❌ 不能写 `&10`。字面量，没地址。                             |
| **右值** | `x + y`                | ❌ 这是一个表达式计算出的**临时结果**，存在寄存器里，没地址。                |
| **右值** | `func()`<br>(返回非引用的函数) | ❌ 返回值是一个**临时对象**，马上销毁。                           |
| **左值** | `string s = "ros";`    | `&s` 合法。                                         |
| **右值** | `"ros"`                | 字面量本身是右值（特例：字符串字面量在 C++ 中比较特殊，但在移动语义语境下通常视为右值源）。 |

##### 左值引用 vs 右值引用 (如何接收？)

引用就是“别名”。但不同的别名只能给不同的人。

**A. 左值引用 (`T&`) —— 传统的引用**

- **规则**：只能绑定到 **左值**。
- **潜台词**：“我要引用一个实实在在存在的人。”

```cpp
int a = 10;
int& ref1 = a;   // ✅ 正确：引用了左值 a
// int& ref2 = 10; // ❌ 错误：不能引用由值 (10 是临时的)
```
_特例_：`const T&` (常引用) 是万能的，它可以绑定右值（因为它是只读的，不会破坏临时对象）。

**B. 右值引用 (`T&&`) —— C++11 的新武器**

- **规则**：只能绑定到 **右值**。
- **潜台词**：“我要抓住一个即将销毁的临时工，准备抢他的资源。”

```cpp
// int&& ref1 = a;  // ❌ 错误：a 是左值，不能用右值引用绑定
int&& ref2 = 10;    // ✅ 正确：绑定了右值 10
ref2 = 20;          // ✅ 甚至可以修改它（延长了临时对象的生命周期）
```

主要的应用就是在构造函数上
**类构造函数重载**，一切为了 **性能 (Move Semantics)**。 编译器通过你用 `&` 还是 `&&`，来决定是 **“复印”** 还是 **“抢劫”**。

```cpp
class BigData {
public:
    // 1. 只有左值引用 (&) -> 只能复印
    // 遇到 BigData b = a; 时调用
    BigData(const BigData& other) {
        std::cout << "深拷贝：申请内存，复制数据..." << std::endl;
    }

    // 2. 有了右值引用 (&&) -> 可以抢劫
    // 遇到 BigData b = std::move(a); 时调用
    BigData(BigData&& other) {
        std::cout << "移动：直接偷走指针，零拷贝！" << std::endl;
    }
};
```

##### ⚠️ 烧脑陷阱：名字本身是左值！⭐⭐⭐

**结论**：一个类型为“右值引用”的变量，它本身是一个“左值”。
```cpp
void process(int&& x) {
    // 参数 x 的类型是：int&& (右值引用)
    // 但是！x 在这个函数里是有名字的！它在函数结束前一直活着！
    // 所以：x 本身是一个【左值】。
    
    // 如果要把它传给下一个只收右值的函数，必须再次 move：
    // next_func(x);            // ❌ 传的是左值，调用的是 copy 版本
    // next_func(std::move(x)); // ✅ 变回右值，调用的是 move 版本
}
```

| **符号**         | **名字**   | **绑定对象**     | **目的**          | **典型场景**                           |
| -------------- | -------- | ------------ | --------------- | ---------------------------------- |
| **`T`**        | 值        | -            | 拷贝副本            | `void f(int a)`                    |
| **`T&`**       | 左值引用     | 左值 (有地址的)    | 修改/观察原对象        | `void f(int& a)` (输出参数)            |
| **`const T&`** | 常左值引用    | 左值 或 右值      | **只读**观察        | `void f(const string& s)` (传参标准写法) |
| **`T&&`**      | **右值引用** | **右值** (临时的) | **窃取资源** (Move) | `BigData(BigData&& other)` (移动构造)  |

**当这两个引用传递给构造函数时，堆内存（Heap）里发生了什么。**

假设我们有一个 `std::vector`，它在栈上有一个“控制头”（存了指针 ptr、大小 size），在堆上有一大块“数据区”（Data）。

**A. 左值引用 (`const T&`) —— 触发“深拷贝” (Deep Copy)**

当你把一个左值引用传给对象 B 的构造函数时：

1. **读取**：B 通过引用，顺藤摸瓜找到了 A 的堆内存数据。
2. **申请**：B 在堆内存里，**新开辟**了一块和 A 一样大的空间。
3. **复制**：CPU 执行 `memcpy` 或循环赋值，把 A 的数据一个字节一个字节地**抄**到 B 的新空间里。
4. **结果**：内存里有两份完全一样的数据。A 和 B 互不干扰。
    
- **内存代价**：极高（申请内存 + 搬运数据）。
- **时间复杂度**：O(N)。
    
**B. 右值引用 (`T&&`) —— 触发“移动” (Move)**

当你把一个右值引用传给对象 B 的构造函数（移动构造）时：

1. **读取**：B 通过引用，找到了 A 的“控制头”。
2. **窃取 (的关键一步)**：
    - B 读取 A 的指针值（比如 `0x999`）。
    - B 把自己的指针指向 `0x999`。
    - **内存操作**：只是简单的整数赋值（8字节），没有动堆上的那一大块数据！
3. **毁灭**：
    - B 把 A 的指针修改为 `nullptr`。
    - **内存操作**：又是简单的赋值。
4. **结果**：堆上的数据纹丝不动，只是**“房产证”上的名字从 A 变成了 B**。A 变成了空壳。
    
- **内存代价**：几乎为零（只改了几个指针的值）。
- **时间复杂度**：O(1)。

⚠️ 这里有一个需要注意的地方，无论是左值引用还是右值引用，都是起别名，都是复制的内存地址。为什么左值引用触发深拷贝呢？

这里的关键在于：**这个引用被用来干什么了？**
如果是**单纯的函数传参**，它**不会**触发拷贝。 但在**构造新对象**（`T b = a`）的场景下，因为它指向的是一个“不能随便动的左值”，为了保证两个对象的**独立性**，接收方被迫进行了深拷贝。

我们以构造一个对象为例来分析一下

```cpp
std::string A = "珍贵数据";
std::string B = A; // 拷贝构造 左值引用
std::string C = std::string("hahaha"); // 移动构造 右值引用
```

- 首先从对象角度分析
	- 左值引用是用一个对象的数据去构造一个新的对象，为保证两个对象的独立，势必要申请新的内存（避免浅拷贝）
	- 右值引用是用一个即将销毁的对象去构造一个新的对象，直接让新对象指向那块内存，达到了构造和销毁的双重目的
- 分析一下两个的调用过程

```cpp
// 1. 签名是引用：为了能看着 A 画画
String(const String& other) {
    // 2. 行为是拷贝：为了画出一幅属于自己的画
    
    // B 自己买画布 (申请内存)
    this->data = new char[other.size]; 
    
    // B 照着 A 画画 (深拷贝数据)
    memcpy(this->data, other.data, other.size); 
}
String(const String&& other) {
    this->data = other.data; // 直接复制地址
    other.data = nullptr; // 避免内存泄漏 
}
```

可以发现在传值方面，都是通过引用传入对象，因此传入方面都是高效的。
不同的在于构造函数的实现方面，移动构造可以直接复制地址，将原对象的指针指向置空。


##### 对于一个简单的例子: `int a = b + c`

按照上述的解释 `b + c`是一个右值，那么会经过 申请内存 -> 存右值 -> 交换地址 -> 置空
但是实际上并不需要这么麻烦

对于 `int` 这种只有 4 个字节的小家伙，CPU 根本不屑于搞什么“指针交换”、“防内存泄漏”。它是直接**复制数值**的。
因为在**计算阶段 (`ALU`)**：

- CPU 把 `b` 的值读进 **寄存器 1** (比如 `EAX`)。
- CPU 把 `c` 的值读进 **寄存器 2** (比如 `EBX`)。
- CPU 执行 `ADD` 指令：寄存器相加，结果存在 **寄存器 1** 里。
- _注意：这一步产生的“和”，就是一个右值。但它通常只活在 CPU 的寄存器里，甚至都不一定会在栈上申请内存。_



---

### 知识点：ROS 中的绝对主角：`shared_ptr` (共享指针)

#智能指针

💥 对于一块内存，**重复释放会造成程序崩溃或堆内存破坏**，如果没**有释放会造成内存泄漏**。
💡 智能指针是一个对象，像一个“保姆”。只要还有人用这个对象，我就留着它；一旦没人用了，我自动帮你删掉。

在 ROS 和 MoveIt 中，99% 的指针都是 `shared_ptr`。
#### ROS 里的 `Ptr` 到底是什么？

你在代码里看到的 `xxxPtr`，其实就是 `shared_ptr` 的缩写（Typedef）。

```cpp
// ROS 源码里是这样定义的（虽然你看不到）：
typedef std::shared_ptr<RobotModel> RobotModelPtr;
typedef std::shared_ptr<PlanningScene> PlanningScenePtr;
```
所以：

- 看到 `RobotModelPtr` $\approx$ `std::shared_ptr<RobotModel>`
- 意思就是：**“这是一个大家都可以共享的、带自动垃圾回收功能的 RobotModel 指针。”**

#### 如何创建呢 (最好借用于make_shared函数)?

**❌ 土办法（旧式/易错）：**
```C++
// 先 new 一个裸指针，再交给智能指针
std::shared_ptr<int> p(new int(10)); 
```
**✅ 推荐办法（高效/现代）：`make_shared`** 这是最高频的写法，像工厂一样直接生产包裹好的智能指针。
```C++
// 语法：std::make_shared<类型>(构造函数参数...);
auto p = std::make_shared<int>(10); 

// ROS 实战例子：
// 创建一个障碍物消息对象
auto object = std::make_shared<moveit_msgs::CollisionObject>();
```
#### 如何使用？（就像用普通指针一样）

智能指针重载了 `->` 和 `*` 操作符，所以用法和普通指针**一模一样**。
```C++
object->id = "box1"; // 使用箭头访问成员
(*object).id = "box1"; // 使用解引用（少用）
```
#### `get()` 是什么？（偶尔用到）

有些古老的 C 语言函数接口需要传入“裸指针”（Raw Pointer）。这时你需要把保姆手里的“真身”拿出来。

```C++
// 假设有个旧函数：void old_c_function(int* p);
std::shared_ptr<int> smart_p = std::make_shared<int>(10);

// old_c_function(smart_p); // ❌ 报错，类型不匹配
old_c_function(smart_p.get()); // ✅ get() 返回内部的 int*
```

_⚠️ 警告：千万不要手动 delete `get()` 出来的指针！保姆会生气的！

### 还有一个配角：`unique_ptr` (独占指针)

虽然在旧 ROS 代码中少见，但在现代 C++（ROS 2）中很常见。

- **含义**：**“这把钥匙是传家宝，天下只有一把。”**
- **特点**：不能复制（Copy），只能移交（Move）。
- **场景**：你拥有这个对象的所有权，绝对不想让别人同时也拥有它。
    
```C++
std::unique_ptr<int> p1(new int(5));
// std::unique_ptr<int> p2 = p1; // ❌ 报错！不能复制
std::unique_ptr<int> p2 = std::move(p1); // ✅ 可以！p1 变空了，所有权转给 p2
```

### 起始参数获取

#### C++程序的输入

标准情况下是`int argc, char** argv`，分别对应：

- **输入参数的个数**(默认有1个，运行路径)
- **输入参数的数组**(第一个参数对应运行路径)

⚡️ 有可能运行程序的时候，并没有参数输入，需要进行判断**是否有值存在，是否满足参数类型**

#### 参数判断

- 第一层判断：个数是否满足 `argc > count+1 ?`
- 第二层判断：异常捕获 `try{}catch{}`

```cpp
int start_index = 0;
if (argc >= 2) {
	try {
		start_index = std::stoi(argv[1]);	
		ROS_INFO("Start Index specified: %d", start_index);		
	} catch (...) {
		ROS_WARN("Invalid argument, starting from 0");
	}
}
```

### Dynamic Reconfigure（动态参数配置）
#动态调参机制
例如: `moveit planner`下的 `ompl interface cfg` 中的 `OMPLDynamicReconfigure.cfg`
调用可以查看 `ompl interface` 中的 `ompl_planner_manager.cpp`

🔍 在我们算法运行过程中如何保证程序持续运行，而可以动态的调整参数
⚡️ 通过Dynamic Reconfigure即可以实现动态调参

理解一下这个过程：
- 普通参数（ROS Param）：在程序运行之前写在`.yaml`文件中的，想修改参数必须要重新运行程序
- Dynamic Reconfigure：提供一些控制的旋钮，在程序运行的过程中动态的调整相关的参数

想要动态的控制，那么我们必要要进行一些设置：
- 设计控制台都有哪些参数
- 建立控制台和程序之间的连接


---
#### 配置控制台

控制台的配置需要`.cfg`文件，该文件定义了“我的界面上要有哪几个按钮？滑动条范围是多少？默认值是多少？”

**💡 闭环逻辑（重点）**： 程序实际调用的是 `OMPLDynamicReconfigureConfig` 类，**不是手写的**，而是根据这个 `.cfg` 文件**自动生成**出来的！
📝 标准的`.cfg`文件模版如下：
```python
#!/usr/bin/env python
# ^ 第一行必须是这个，因为它要作为脚本运行

PACKAGE = "my_robot_planning" # 你的包名

from dynamic_reconfigure.parameter_generator_catkin import *

gen = ParameterGenerator()  # 生成器对象

# -----------------------------------------------------------------------------------
# 格式：gen.add(name变量名, type类型, level等级, description描述, default默认值, min最小值, max最大值)
# -----------------------------------------------------------------------------------

# 1. 浮点数滑动条 (double_t)
gen.add("max_velocity", double_t, 0, "机械臂最大速度", 0.5, 0.0, 1.0)

# 2. 整数滑动条 (int_t)
gen.add("smooth_iterations", int_t, 0, "平滑迭代次数", 50, 10, 200)

# 3. 复选框/布尔值 (bool_t)
gen.add("enable_collision_check", bool_t, 0, "开启碰撞检测", True)

# 4. 字符串 (str_t)
gen.add("planner_id", str_t, 0, "规划器ID", "RRTConnect")

# -----------------------------------------------------------------------------------

# 最后一行是生成指令：
# 参数1: 包名
# 参数2: 节点名 (用于日志)
# 参数3: 生成的类名前缀 (这就是你在 C++ 里看到的那个名字！)
exit(gen.generate(PACKAGE, "my_node_name", "OMPLDynamicReconfigure"))
```

**需要重点注意的坑**

**💥 坑一：必须给可执行权限！** 因为它是脚本，系统必须能运行它。**（Permission denied）**
- **命令**：`chmod +x cfg/MyConfig.cfg`
- **症状**：如果没给权限，编译时会报错，或者生成的头文件是空的

💥 **坑二：必须在 `CMakeLists.txt` 里注册（No such file or directory）**
```CMake
find_package(catkin REQUIRED COMPONENTS 
	roscpp 
	# ... 其他包 
	dynamic_reconfigure # <--- 【必加】 
)
# 重点：在 CMakeLists.txt 中找到这一段
generate_dynamic_reconfigure_options(
  cfg/OMPLDynamicReconfigure.cfg
)

add_executable(my_planner_node src/my_planner_node.cpp)

# 【核心中的核心】
# 语法：add_dependencies(你的可执行文件名 ${PROJECT_NAME}_gencfg)
# ${PROJECT_NAME}_gencfg 是 ROS 自动生成的一个目标，代表“生成配置文件的动作”
add_dependencies(my_planner_node ${PROJECT_NAME}_gencfg)

# 如果你写的是库 (add_library)，同理：
# add_dependencies(my_planner_lib ${PROJECT_NAME}_gencfg)

target_link_libraries(my_planner_node
  ${catkin_LIBRARIES} # dynamic_reconfigure 包含在这里面
)
```
**症状**：编译成功，但在 C++ 里 `#include` 时报错“No such file”

**💥 坑三：依赖关系要写对** 在 `package.xml` 里必须加上（**Undefined reference**）
```xml
<build_depend>dynamic_reconfigure</build_depend>
<exec_depend>dynamic_reconfigure</exec_depend>
```

**放置位置**

🏡 按照ROS 社区的约定俗成来进行文件的放置，方便我们的查找也方面同事之间的交流

```Plaintext
~/catkin_ws/src/my_robot_planning/  <-- 你的包根目录
├── CMakeLists.txt
├── package.xml
├── src/
│   └── main.cpp
├── include/
│   └── my_robot_planning/
│       └── ...
└── cfg/                        <-- 【重点】就在这里！
    └── OMPLDynamicReconfigure.cfg
```
就是在功能包目录下单独建立一个文件夹来存放该文件

**安装控制台**

按照前面的步骤，基本上已经完成了控制台的配置，下面就是如何加载了
```cpp
#include <dynamic_reconfigure/server.h>
#include "moveit_planners_ompl/OMPLDynamicReconfigureConfig.h"

std::unique_ptr<dynamic_reconfigure::Server<OMPLDynamicReconfigureConfig>> dynamic_reconfigure_server_ = 
    std::make_unique<dynamic_reconfigure::Server<OMPLDynamicReconfigureConfig>>(
        ros::NodeHandle(nh_, ompl_ns)
    );
```

主要借助于智能指针`unique_ptr`来构建一个`OMPLDynamicReconfigureConfig`类型的`dynamic_reconfigure::Server`
主要需要关注以下几个部分：
- `dynamic_reconfigure_server_`：这里设置为一个类成员指针变量，通过指针控制控制台的生命周期。
- `dynamic_reconfigure::Server<...>`：标砖的一个服务类
- `ros::NodeHandle(nh_, ompl_ns)`：在父句柄 `nh_` 的基础上，下沉到了 `ompl_ns` 这个命名空间里，用于获取参数服务器中的参数

**执行流程**

🚀 整个流程的流水线

- **工程师 (你)**：编写 `MyConfig.cfg` (Python 脚本)。
- **编译系统 (CMake)**：在编译时，执行 `python MyConfig.cfg`。
- **生成器 (Generator)**：
    - 自动产出 `MyConfig.h` (给 C++ 用)。
    - 自动产出 `MyConfig.py` (给 Python 用)。    
- **你的代码 (C++)**：`#include "my_pkg/MyConfig.h"`，然后创建一个 Server。
- **最终效果 (Rqt)**：用户打开 `rqt_reconfigure`，看到刚才定义的滑动条。

**OMPL参考**
```python
#!/usr/bin/env python
PACKAGE = "moveit_planners_ompl"

from dynamic_reconfigure.parameter_generator_catkin import *

gen = ParameterGenerator()

gen.add("simplify_solutions", bool_t, 0, "Flag indicating whether computed motion plans are also simplified", True)
gen.add("minimum_waypoint_count", int_t, 0, "Set the minimum number of waypoints to include in a motion plan", 2, 2, 10000)
gen.add("maximum_waypoint_distance", double_t, 0, "The maximum distance between consecutive waypoints along the solution path (0.0 means 'ignore')", 0.0, 0.0, 50.0)
gen.add("link_for_exploration_tree", str_t, 0, "Show the exploration tree for a particular link", "")
gen.add("display_random_valid_states", bool_t, 0, "Flag indicating whether random valid states are to be published", False)

exit(gen.generate(PACKAGE, PACKAGE, "OMPLDynamicReconfigure"))
```
```cpp
#include <dynamic_reconfigure/server.h>
#include "moveit_planners_ompl/OMPLDynamicReconfigureConfig.h"

std::unique_ptr<dynamic_reconfigure::Server<OMPLDynamicReconfigureConfig>> dynamic_reconfigure_server_ = 
    std::make_unique<dynamic_reconfigure::Server<OMPLDynamicReconfigureConfig>>(
        ros::NodeHandle(nh_, ompl_ns)
    );
```

---
#### 连接线路

```cpp
dynamic_reconfigure_server_->setCallback(
    [this](auto& config, uint32_t level) { 
        dynamicReconfigureCallback(config, level); 
    }
);
```

主要有以下几个部分组成：
- `setCallback(...)`：当控制台相关的设置改变的时候请执行以下参数
- `[this](auto& config, uint32_t level) { ... }`：
	- 这是一个 **C++ Lambda 表达式（匿名函数）**
	- **`auto& config`**: 这里其实就是 `OMPLDynamicReconfigureConfig&`
	- **`uint32_t level`**: 这是一个掩码。如果在 `.cfg` 文件里定义了不同的 level，你可以通过判断这个数字，知道用户到底修改了哪个参数

---
### static_cast (静态转换)和dynamic_cast (动态转换)

📂 **静态转换**是个讲道理的翻译官。只要你在编译时能证明这两种类型有关系（比如父子关系、数字转换），我就帮你转。完全没关系的东西（比如把‘机械臂’转成‘香蕉’），我会直接报错拦住你。（程序编译时）
📂 **动态转换**是个严谨的安检员。你拿着一个‘基类指针’（比如‘员工卡’）告诉我你是‘经理’，我不信。我要在**程序运行的时候**（Runtime）去查你的档案（`RTTI`）。如果你真的是经理，我放行；如果你只是个普通员工却想冒充经理，我直接给你返回 **空指针 (`nullptr`)**。（程序运行时）

🚀 静态类型转换语法：`目标类型 变量 = static_cast<目标类型>(原始变量);`
🚀 动态类型转换语法： `Derived* d = dynamic_cast<Derived*>(base_ptr);`

🔍 为什么不用**c语言的强制类型转换**？(`(int)a  -> static_cast<int>(a)`)
- C 风格转换太野蛮了，它什么都能转（甚至把指针转成整数），容易出大 bug。`static_cast` 会在编译时检查类型是否“兼容”，不兼容直接报错。
- 在几万行代码里找“哪里做了类型转换”？搜索 `static_cast` 一搜一个准。搜 `(` 括号？你会疯掉

🔍 动态转换相较于静态转换，可以进行试**探性转换** (Attempt-to-Cast)
- `dynamic_cast` 允许你写出这样的逻辑：**“我不知道这个指针具体是什么，试试看它是不是 A？如果不是，那它是不是 B？”**
- 这在 **Pluginlib** 和 **工厂模式** 中极其常见。
```cpp
// 假设 Base 是基类，Derived 是子类
Base* ptr = getSomeObject(); // 获取一个不知道具体类型的对象

// 试图转成 Derived
Derived* d = dynamic_cast<Derived*>(ptr);

if (d) { // ✅ 等同于 if (d != nullptr)
    // 转换成功！说明 ptr 指向的确实是 Derived 对象
    d->specialFunction(); // 可以安全调用子类特有函数
    ROS_INFO("它是 Derived 类型！");
} else {
    // ❌ 转换失败！说明 ptr 指向的不是 Derived (可能是别的子类)
    // 程序不会崩，只是 d 变成了空指针  我们可以再dynamic_cast别的数据类型
    ROS_WARN("它不是 Derived，处理失败");
}
```

| **特性**     | **static_cast (静态转换)**              | **dynamic_cast (动态转换)**  |
| ---------- | ----------------------------------- | ------------------------ |
| **生效时间**   | **编译时** (Compile Time)              | **运行时** (Runtime)        |
| **速度**     | 🚀 **极快** (无额外开销)                   | 🐢 **较慢** (要查 RTTI 类型信息) |
| **安全性**    | **不安全** (转错也不报错，责任在人)               | **安全** (转错会返回 `nullptr`) |
| **主要用途**   | 1. 基础数据转换 (int/float)<br>2. 确定的父子转换 | **不确定**父类指针到底指向哪个子类时     |
| **ROS 场景** | 绝大多数数值计算、确定的指针传递                    | 插件加载后，判断加载的是哪种算法         |
**⚠️ dynamic_cast 必须有虚函数 (Virtual Function)** `dynamic_cast` 只有在**多态**的类体系中才能工作。
- **条件**：基类（父类）中**至少要有一个 `virtual` 函数**（通常是析构函数 `virtual ~Base() {}`）。
- **原因**：C++ 编译器只给有虚函数的类生成“类型信息表”（`RTTI`）。没有这个表，安检员就查不到档案，`dynamic_cast` 就会报错。
🚀 **总结**
- **static_cast** 是 **“我相信”**：我相信它是，直接转，别废话。（快，但风险自负）
- **dynamic_cast** 是 **“我怀疑”**：我不确定它是不是，帮我查一下，如果是就给我，不是就给空。（慢，但绝对安全）

| **场景**                           | **推荐使用**           | **理由**                          |
| -------------------------------- | ------------------ | ------------------------------- |
| **基础数据转换** (`double` -> `int`)   | `static_cast`      | 只有它可以做数值转换。                     |
| **向上转型** (子类 -> 父类)              | `static_cast`      | 100% 安全，没必要浪费性能去查表。             |
| **向下转型** (父类 -> 子类) 且 **非常确定类型** | `static_cast`      | 为了速度（比如在高频控制循环里）。但你要为崩溃负责。      |
| **向下转型** 且 **不确定具体是哪个子类**        | **`dynamic_cast`** | **唯一选择**。利用 `nullptr` 检查来做逻辑分支。 |
| **插件/工厂模式返回的指针**                 | **`dynamic_cast`** | 你永远无法完全信任外部加载进来的插件类型。           |

### std::mutex 互斥锁
#多线程安全

💡 把mutex想象成**卫生间的钥匙**
- 公共卫生间（共享内存）只有一个坑位
- **Mutex** 就是那把唯一的钥匙
- 如果你想进去，必须先拿到钥匙（`lock`）
- 如果钥匙被别人拿走了，你只能在门口排队死等（`block`），直到里面的人出来还钥匙（`unlock`）

🥊 防止 **“竞态条件 (Race Condition)”**。即防止两个线程同时修改同一个变量，导致数据错乱

ROS开发中经常会用到它，因为ROS中往往是多节点多线程的，线程管理是必不可少的
- 📝**场景**：
    1. **回调函数（线程 A）**：不断接收最新的 `CurrentPose`（位置数据）并更新变量。
    2. **主循环/规划线程（线程 B）**：正在读取 `CurrentPose` 用来做路径规划。
- 📝**危险**：如果线程 B 正在读数据的 `x` 坐标，还没读 `y`，此时线程 A 突然冲进来把 `x` 和 `y` 都改了。结果 B 读到的是“旧的 `x` + 新的 `y`”，数据就**撕裂**了，机器人可能会突然抽风。

##### ❌ 黑名单：原始写法（手动挡）

**极度危险**，严禁在生产环境代码中使用。
```cpp
#include <mutex>

std::mutex mtx; // 1. 创建一把锁
int shared_data = 0;

void dangerous_function() {
    mtx.lock();   // 2. 拿钥匙（上锁）
    
    // ... 执行操作 ...
    // ⚠️ 风险点：如果这里抛出了异常，或者你写了个 return 提前退出了...
    // 代码永远执行不到下面这一行！
    
    mtx.unlock(); // 3. 还钥匙（解锁）
}
// 后果：钥匙没还，后续所有线程全部死锁（Deadlock），程序卡死。
```
⚠️ 可以看到手动会有很大的风险，随着程序的增长，遇到的情况也会变多，可能最后没有调用`unlock()`导致线程一直死锁。
##### ✅ 白名单：RAII 写法（自动挡）lock_guard

使用 **`std::lock_guard`** 或 **`std::unique_lock`**。它们利用 C++ 的构造和析构机制，保证“**进门自动上锁，出门自动解锁**”，哪怕报错了也能自动解锁。
```cpp
#include <mutex>

std::mutex mtx; // 全局锁（或者类成员变量）
int shared_data = 0;

void safe_function() {
    // 【关键】创建即上锁。
    // 这里的 lock 变量只是一个“保镖”，它构造时自动调用 mtx.lock()
    std::lock_guard<std::mutex> lock(mtx); 
    
    shared_data++; // 安全修改数据
    
    // ... 随便你怎么 return 或抛异常 ...
    
} // 【关键】函数结束，lock 变量超出作用域被销毁，析构函数自动调用 mtx.unlock()
```
⚠️ 需要注意的是可能函数非常长，而我们又不想让线程一直死锁，我们可以使用大括号 `{}` 来限制 `lock_guard` 的生命周期。
```cpp
void huge_function() {
    // 准备工作（不需要锁）
    prepare();

    { // --- 临界区开始 ---
        std::lock_guard<std::mutex> lock(mtx); // 🔒 自动上锁
        
        // 只有这里修改共享数据
        shared_data++;
        
    } // --- 临界区结束 --- 🔓 lock 变量在这里销毁，自动解锁

    // 耗时计算（不需要锁）
    // 此时别的线程已经可以拿锁了，性能提升！
    heavy_calculation();
}
```
##### ✅ 白名单：RAII 写法（自动挡）unique_lock

- **本质**：它也是一个 RAII 风格的锁管理器，但比 `lock_guard` 多了很多**手动操作**的功能。
- **代价**：因为它内部需要维护一个标志位（记录“我现在到底是锁着还是没锁”），所以性能比 `lock_guard` **微乎其微地慢一点点**（通常可以忽略不计）。
对比一下lock_guard和unique_lock

| **能力**          | **std::lock_guard** | **std::unique_lock**     |
| --------------- | ------------------- | ------------------------ |
| **RAII 自动析构解锁** | ✅ 支持                | ✅ 支持                     |
| **构造时暂时不锁**     | ❌ 不支持 (必须马上锁)       | ✅ 支持 (`std::defer_lock`) |
| **中途手动解锁**      | ❌ 不支持 (死板)          | ✅ 支持 (`lck.unlock()`)    |
| **中途手动上锁**      | ❌ 不支持               | ✅ 支持 (`lck.lock()`)      |
| **配合条件变量**      | ❌ 不支持               | ✅ **必须用它**               |
| **所有权转移**       | ❌ 不可移动              | ✅ 可移动 (Moveable)         |
```cpp
bool data_ready = false;
void complex_job() {
    std::unique_lock<std::mutex> my_lock1(mtx); // 1. 构造时自动上锁
    std::unique_lock<std::mutex> my_lock2(mtx, std::defer_lock); // 先没有锁
    // --- 步骤 1：读取共享数据 (快) ---
    read_shared_data();

    // 2. 【关键】中途手动解锁！
    // 告诉系统：“我先不占着茅坑了，你们用吧”
    my_lock1.unlock(); 

    // --- 步骤 2：处理数据 (耗时 1秒) ---
    // 此时其他线程可以抢锁，并发性能提升！
    heavy_computation();
	
	// 现在同时锁住两个，防止死锁 
	std::lock(my_lock1, my_lock2);
	my_lock1.unlock(); 
    // 3. 【关键】再次手动上锁
    // “我回来了，我要写回去了”
    my_lock1.lock(); 
    // 意思是：如果数据没好，就解锁并在那里死等； 
    // 等到被唤醒了，并且 data_ready 为 true 了，再重新上锁继续往下跑
	cv.wait(my_lock1, []{ return data_ready; });
    // --- 步骤 3：写入结果 (快) ---
    write_shared_data();

} // 4. 函数结束，RAII 依然生效：如果此时是锁着的，它会自动 unlock
```
#### 📝 标准模板
```cpp
class MyRobotNode {
private:
    std::mutex pose_mutex_; // 定义锁
    GeometryMsgs::Pose current_pose_; // 共享数据

public:
    // 回调线程（写）
    void callback(const SensorMsg& msg) {
	    // 1. 这里是处理无关紧要的数据，不需要锁（提高并发性能）
	    ROS_INFO("收到消息");

	    { // --- 临界区开始 ---
	        // guard 出生，自动上锁
	        std::lock_guard<std::mutex> guard(data_mutex_); 
	        
	        // 快速更新共享数据
	        this->latest_data_ = msg;
	        
	    } // --- 临界区结束 --- 
	      // 遇到右大括号，guard 死亡，自动解锁
	      
	    // 2. 这里做耗时的计算，此时锁已经释放了，别的线程可以进来了
	    heavy_calculation();
	}// 🔓 自动解锁

    // 主线程（读）
    void run() {
        while(ros::ok()) {
            {
                std::lock_guard<std::mutex> lock(pose_mutex_); // 🔒 上锁
                // 快速把数据读出来，或者计算
                process(current_pose_);
            } // 🔓 自动解锁（用大括号限制锁的范围，越短越好！）
            
            // 耗时的操作放到锁外面做，不要占着茅坑不拉屎
            heavyCalculation(); 
        }
    }
};
```


### 宏魔法 (Macro Magic)

#代码规范与效率 #源码阅读辅助

⚡️ **MoveIt (以及很多 ROS 高级库)** 中极其经典的一种用法
💡 懒人神器，一行代码搞定前置声明和所有智能指针的定义

🥊 **极大减少代码量 (Don't Repeat Yourself)**
- **没用宏之前**：每写一个类，你都要手动敲上面那 6 行 `typedef`。如果类名改了，你得改 7 个地方。
- **用了宏之后**：只要一行。ROS 里的类名通常很长（比如 `ModelBasedPlanningContext`），这能救命。
🥊 **加速编译 (Forward Declaration)** 注意宏里的第一句 `class C;`。
- 这是 **“前置声明”**。它告诉编译器：“有个类叫这个名字，你先记着，具体的定义在别的文件里，不用现在去找。”
- **效果**：在头文件里引用这个类时，不需要 `#include "model_based_planning_context.h"`。
- **收益**：减少了头文件之间的互相引用（Include Hell），大大加快了编译速度。

示例：
```cpp
MOVEIT_CLASS_FORWARD(ModelBasedPlanningContext);

#define MOVEIT_CLASS_FORWARD(C)        \                                                                           
  class C;                        \                                                                                
  MOVEIT_DECLARE_PTR(C, C)

#define MOVEIT_DECLARE_PTR(Name, Type)     \                                                                       
  typedef std::shared_ptr<Type> Name##Ptr;   \                                                                     
  typedef std::shared_ptr<const Type> Name##ConstPtr;     \                                                        
  typedef std::weak_ptr<Type> Name##WeakPtr;              \                                                        
  typedef std::weak_ptr<const Type> Name##ConstWeakPtr;    \                                                       
  typedef std::unique_ptr<Type> Name##UniquePtr;           \                                                       
  typedef std::unique_ptr<const Type> Name##ConstUniquePtr
```

会自动处理成为以下
```cpp
// 1. 前置声明 (Forward Declaration)
class ModelBasedPlanningContext; 

// 2. 自动定义一堆智能指针的别名 (Typedefs)
// ## 是连接符，把名字拼在一起
typedef std::shared_ptr<ModelBasedPlanningContext>       ModelBasedPlanningContextPtr;
typedef std::shared_ptr<const ModelBasedPlanningContext> ModelBasedPlanningContextConstPtr;
typedef std::weak_ptr<ModelBasedPlanningContext>         ModelBasedPlanningContextWeakPtr;
typedef std::weak_ptr<const ModelBasedPlanningContext>   ModelBasedPlanningContextConstWeakPtr;
typedef std::unique_ptr<ModelBasedPlanningContext>       ModelBasedPlanningContextUniquePtr;
typedef std::unique_ptr<const ModelBasedPlanningContext> ModelBasedPlanningContextConstUniquePtr;
```

⚡️ **语法细节解析**

- **`class C;`** 这告诉编译器 `C` 是一个类。有了这句话，你就可以声明 `C*` (指针) 或者 `std::shared_ptr<C>` (智能指针)，而不需要知道 `C` 到底长什么样（不需要 include 头文件）。
    
- **`Name##Ptr`** 这是 C/C++ 预处理器的 **“连接符” (Token Pasting Operator)**。
    
    - 如果 `Name` 是 `MyClass`。
    - `Name##Ptr` 就会变成 `MyClassPtr`。
    - 这就是为什么你在 MoveIt 代码里随处可见 `xxxPtr`，却找不到哪里定义了这个类型的原因

🔄 **源码阅读技巧**

1. **看到 `MOVEIT_CLASS_FORWARD(MyClass)`**： 脑子里自动把它替换成：**“这里声明了 `MyClass`，并且定义了 `MyClassPtr`, `MyClassConstPtr` 等全套智能指针。”**
2. **看到 `MyClassPtr`**： 不要去 grep 搜索它的定义，搜不到的。它就是 `std::shared_ptr<MyClass>` 的缩写。它是通过这个宏自动生成的。
3. **自己写代码时**： 如果你写的类会被广泛引用，建议也模仿这个写法（或者直接复制这个宏定义到你的工具头文件中），这显得非常 **“ROS Native”**。

### 函数指针

**函数也有地址？**

- **变量**（比如 `int a = 10`）：在内存里占个地盘，存的是**数据**。
- **函数**（比如 `void func()`）：在内存的代码段（Code Segment）占个地盘，存的是**指令**。

**函数指针**，就是一个变量，只不过它存的不是 `10`，而是**那个函数的第一行指令在内存中的地址**。 你拿到这个地址，就能让 CPU 跳过去执行那段代码。

☠️ 函数指针的声明语法非常反人类，新手看第一眼通常会晕
📝 **公式**：`返回值类型 (*指针变量名)(参数列表)`

**举例**： 我要指向一个 `int add(int a, int b)` 函数。

- **错误写法**：`int *ptr(int, int)` —— ❌ 这会被编译器理解为：一个返回 int 指针的函数。    
- **正确写法**：`int (*ptr)(int, int)` —— ✅ 括号把 `*ptr` 包起来，表示 `ptr` 是个指针。

```cpp
#include <iostream>

// 1. 定义一个普通函数 (目标)
int add(int a, int b) {
    return a + b;
}

int minus(int a, int b) {
    return a - b;
}

int main() {
    // 2. 声明函数指针，并赋值
    // 意思是：calc 是一个指针，指向“返回int，接收两个int”的函数
    int (*calc)(int, int); 

    // 指向加法
    calc = add;  // 函数名本身就是地址，写 &add 也可以
    std::cout << "3 + 2 = " << calc(3, 2) << std::endl; // 输出 5

    // 指向减法 (像换电池一样简单)
    calc = minus;
    std::cout << "3 - 2 = " << calc(3, 2) << std::endl; // 输出 1

    return 0;
}
``` 


### 栈展开 (Stack Unwinding)

#栈展开
通过异常捕获的传播来介绍一下**栈展开**

**场景假设**：

1. `main()` 函数调用了 -> `Plan()` (规划)
2. `Plan()` 调用了 -> `Compute()` (计算)
3. `Compute()` 调用了 -> `Math()` (数学运算)
4. **`Math()` 发生了除以零错误，抛出 `throw`！**

|**内存栈 (Stack)**|**状态**|
|---|---|
|**[4. Math]**|💣 **这里发生了爆炸 (throw)**|
|**[3. Compute]**|正在等待 Math 返回...|
|**[2. Plan]**|正在等待 Compute 返回...|
|**[1. Main]**|正在等待 Plan 返回... (这里写了 `try-catch`)|

当 `Math` 函数执行 `throw` 时，C++ 运行时系统接管了控制权，开启了“**逃生模式**”。它不会像 `return` 那样按部就班地返回数据，而是开始**暴力拆楼**。

#### 第一步：检查案发现场

系统看一眼 `Math()` 函数内部：

- _“你这里有 `try-catch` 包裹这行代码吗？”_
- **答案：没有。**
- **动作**：`Math` 函数被判定为“无能为力”。系统决定**销毁** `Math` 的栈帧（Stack Frame）。
    - **关键点**：`Math` 里的所有局部变量（比如 `int a`, `vector b`）会被立即**析构**（清理内存）。
    - `Math` 这一层“盘子”被扔掉了。
#### 第二步：向上级汇报 (传递错误)

错误像气泡一样，浮到了下一层 **`Compute()`**。

- 系统问 `Compute`：_“刚才 `Math` 炸了，抛出了一个错误，你的代码里有 `try-catch` 能接住吗？”_
- **答案：没有**（假设 Compute 也没写 try）。
- **动作**：`Compute` 也被判定无能为力。**销毁** `Compute` 的栈帧。
    - `Compute` 里的局部变量全部析构。
    - `Compute` 这一层也被扔掉了。
#### 第三步：继续上浮

错误继续浮到了 **`Plan()`**。
- 系统问 `Plan`：_“有 `catch` 吗？”_
- **答案：没有**。
- **动作**：**销毁** `Plan`。
#### 第四步：终极接盘

错误浮到了 **`Main()`**。

- 系统问 `Main`：_“有 `catch` 吗？”_
- **答案：有！** (`main` 里写了 `try { Plan() } catch (...)`)
- **动作**：
    1. 系统停止拆楼。
    2. 错误对象（那个异常变量）被传递给 `catch` 的参数。
    3. 程序**跳转**进 `catch` 块的代码开始执行。

```cpp
#include <iostream>

void Math() {
    // 3. 最底层抛出异常
    throw std::runtime_error("除以零错误！"); 
}

void Compute() {
    // 2. 这里没有 try-catch，所以会被“穿透”
    Math(); 
}

void Plan() {
    // 1. 这里也没有，继续“穿透”
    Compute();
}

int main() {
    try {
        Plan(); // 0. 从这里开始调用
    } 
    catch (const std::exception& e) {
        // 4. 最终在这里捕获！
        // 错误就像坐直梯一样，瞬间从 Math 直达 Main
        std::cout << "在 Main 里捕获: " << e.what() << std::endl;
    }
    return 0;
}
```