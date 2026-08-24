---
created: 2026-08-24
tags:
  - C++
  - STL
  - data-structure
  - algorithm
  - interview
status: active
---

# C++ 核心、STL、数据结构与算法系统复习

> 对应简历能力：C/C++ 编程、面向对象、STL、C++11、常用数据结构与算法。

## 0. 如何使用这份文档

这不是语法字典，而是一份从“会写”到“能解释、能排错、能在项目中选择”的知识地图。

每个主题至少达到四层：

1. **定义层**：知道概念是什么。
2. **机制层**：能解释内存、生命周期、复杂度或底层结构。
3. **应用层**：能说明为什么在当前问题中选择它。
4. **边界层**：知道失效条件、常见错误和替代方案。

关联专题：

- [[C++面向对象|面向对象详细复习]]
- [[C++现代用法|现代 C++ 项目用法]]
- [[并发编程|并发编程入口]]
- [[CPP-learning/STL/STL总结|STL 原有笔记]]
- [[CPP-learning/data_struct/data_struct|数据结构原有笔记]]
- [[CPP-learning/algorithm/algorithm|算法原有笔记]]

---

# 第一部分：C 与 C++ 编程基础

## 1. 从源代码到可执行程序

一个 C/C++ 程序通常经历四步：

```text
源文件
→ 预处理：展开 #include、宏和条件编译
→ 编译：语法/语义检查，生成汇编
→ 汇编：生成目标文件 .o/.obj
→ 链接：解析符号和库，生成可执行文件或动态库
```

### 编译错误与链接错误

- 编译错误：语法错误、类型不匹配、找不到声明、模板实例化失败。
- 链接错误：函数只有声明没有定义、库未链接、符号签名不一致、重复定义。
- 运行错误：越界、空悬指针、数据竞争、未捕获异常、资源耗尽。

### 头文件为什么需要 include guard

同一个头文件可能经多条包含路径重复展开。使用：

```cpp
#pragma once
```

或传统宏保护：

```cpp
#ifndef ROBOT_STATE_HPP
#define ROBOT_STATE_HPP

// declarations

#endif
```

### 声明与定义

- 声明告诉编译器“名字和类型存在”。
- 定义真正分配存储或提供函数实现。
- 一个非 `inline` 实体通常只能有一个定义，这属于 ODR（One Definition Rule）。

```cpp
// declaration
double norm(double x, double y);

// definition
double norm(double x, double y) {
    return std::sqrt(x * x + y * y);
}
```

## 2. 类型、对象与值类别

### 基本类型

常见类型包括整数、浮点、字符、布尔和 `void`。不要假设 `int` 永远是固定字节数；需要固定宽度时使用：

```cpp
#include <cstdint>

std::int32_t encoder_count{};
std::uint64_t timestamp_ns{};
```

### 初始化优先于赋值

```cpp
int a;        // 未初始化的局部变量，读取是未定义行为
int b = 0;    // 初始化
int c{0};     // 列表初始化，可阻止窄化转换
double d{3.0};
```

优先使用 `{}` 初始化，尤其在泛型和数值代码中，它能拦截部分危险转换：

```cpp
int x{3.14};  // 编译错误：窄化
```

### 左值与右值

- 左值通常有身份、可取地址，可出现在赋值号左侧。
- 纯右值通常是临时计算结果。
- `T&&` 是右值引用，是移动语义和完美转发的基础。

简单判断：如果表达式代表一个可以长期定位的对象，它通常是左值；临时结果通常是右值。

## 3. 作用域、存储期与生命周期

### 常见存储区域

```text
代码区      程序指令
静态存储区  全局变量、静态变量
栈          自动变量、函数调用帧
堆          动态分配对象
```

“栈/堆”是常见实现模型；C++ 标准更关注对象的存储期与生命周期。

### 四种存储期

- 自动存储期：进入块时创建，离开块时销毁。
- 静态存储期：程序开始到结束。
- 线程存储期：与线程生命周期一致。
- 动态存储期：显式分配，直到释放或由资源管理对象释放。

### 未定义行为

未定义行为意味着标准不规定程序结果，不能理解成“必然崩溃”。常见来源：

- 数组越界。
- 使用已释放对象。
- 有符号整数溢出。
- 读取未初始化变量。
- 数据竞争。
- 通过无效迭代器访问容器。

## 4. 指针、引用与 `const`

### 指针

指针保存地址，可以为空、可以重新指向其他对象：

```cpp
int value = 10;
int* ptr = &value;

if (ptr != nullptr) {
    *ptr = 20;
}
```

### 引用

引用是已有对象的别名，声明时必须绑定，通常不能改绑：

```cpp
int value = 10;
int& ref = value;
ref = 20;  // 修改 value
```

### 参数传递选择

| 场景 | 推荐形式 |
|---|---|
| 小型标量输入 | `int`、`double`，按值传递 |
| 大对象只读输入 | `const T&` |
| 函数需要修改调用者对象 | `T&` |
| 参数允许为空 | `T*` 或明确的可空封装 |
| 转移所有权 | `std::unique_ptr<T>` |
| 共享所有权 | `std::shared_ptr<T>`，谨慎使用 |

### `const` 的常见形式

```cpp
const int* p1;       // 指向 const int：不能通过 p1 修改值
int* const p2 = ...; // const 指针：不能改变指向
const int* const p3 = ...; // 指向 const 的 const 指针
```

记忆方法：从变量名向外读。

### 指针与引用的面试区别

- 指针可为空，引用按语言语义应绑定有效对象。
- 指针可改指向，引用通常不可改绑。
- 指针需要解引用，引用用法接近普通对象。
- 两者都可能悬空；引用并不自动保证生命周期安全。

## 5. 数组、字符串与内存函数

### C 数组与 `std::array`

```cpp
int raw[3]{1, 2, 3};
std::array<int, 3> safe{1, 2, 3};
```

`std::array` 保留定长、栈上布局，同时提供 `size()`、迭代器和 STL 算法兼容性。

### C 字符串与 `std::string`

- C 字符串以 `\0` 结尾，接口容易发生缓冲区越界。
- `std::string` 自己管理内存，优先作为业务字符串类型。
- 调用 C API 时可通过 `str.c_str()` 获取只读 C 字符串。

### `memcpy` 不能复制所有 C++ 对象

`memcpy` 适合按字节复制可平凡复制的数据。含 `std::string`、虚函数、智能指针或自定义资源所有权的对象不能用它完成语义正确的复制，应使用拷贝构造或赋值运算符。

## 6. 函数、重载与模板

### 函数重载

同一作用域中函数名相同、参数列表不同，可形成重载。不能仅靠返回类型区分：

```cpp
void send(int value);
void send(double value);
// double send(int value); // 不能只改返回类型
```

### 默认参数

默认参数通常放在声明中，并从右向左提供：

```cpp
void connect(int timeout_ms, int retries = 3);
```

### `inline`

`inline` 的核心语言作用是允许同一实体在多个翻译单元中出现等价定义，常用于头文件函数；编译器是否内联展开是优化决策，不由关键字强制。

### 函数模板

```cpp
template <typename T>
T clamp_value(const T& value, const T& low, const T& high) {
    return std::min(std::max(value, low), high);
}
```

模板定义通常放在头文件，因为实例化点需要看到完整定义。

## 7. C 与 C++ 的关键区别

| 主题 | C | C++ |
|---|---|---|
| 编程范式 | 过程式为主 | 多范式：过程式、OOP、泛型、函数式 |
| 资源管理 | 手工获取/释放 | RAII、容器、智能指针 |
| 抽象 | `struct` + 函数指针 | 类、模板、重载、虚函数 |
| 错误处理 | 返回码、`errno` | 返回值、异常、类型化结果 |
| 动态内存 | `malloc/free` | 优先容器/智能指针，底层为 `new/delete` |
| 字符串 | 字符数组 | `std::string` |

### `malloc/free` 与 `new/delete`

- `malloc` 只分配原始字节，不调用构造函数；失败时返回空指针。
- `new` 分配存储并构造对象；普通形式失败时抛出 `std::bad_alloc`。
- `free` 不调用析构函数；`delete` 先析构再释放。
- 两组 API 不能混用。
- 现代 C++ 业务代码优先使用容器和智能指针。注意 `std::make_unique` 从 C++14 开始提供，严格 C++11 环境可直接构造 `unique_ptr`。

---

# 第二部分：面向对象与资源管理

## 8. 封装、继承与多态

### 封装

封装不是“把变量设为 private”这么简单，而是让对象自己维护不变量：

```cpp
class JointLimit {
public:
    JointLimit(double min, double max) : min_(min), max_(max) {
        if (min > max) {
            throw std::invalid_argument("min must not exceed max");
        }
    }

    bool contains(double q) const noexcept {
        return q >= min_ && q <= max_;
    }

private:
    double min_;
    double max_;
};
```

对象一旦构造成功，就保证 `min_ <= max_`。

### 继承

公有继承表达“is-a”关系。若 Derived 不能在所有合理场景替代 Base，则不适合公有继承。

### 组合

组合表达“has-a”关系，通常比继承耦合更低：

```cpp
class RobotController {
private:
    SafetyLimiter limiter_;
    TrajectoryBuffer buffer_;
};
```

优先组合，只有需要真正的接口替换和动态多态时才使用继承。

## 9. 构造、析构与特殊成员函数

### 初始化列表

成员在进入构造函数体之前就已经初始化。初始化顺序由成员声明顺序决定，而不是初始化列表书写顺序：

```cpp
class State {
public:
    explicit State(std::size_t dof)
        : positions_(dof, 0.0), velocities_(dof, 0.0) {}

private:
    std::vector<double> positions_;
    std::vector<double> velocities_;
};
```

### Rule of Three / Five / Zero

如果类直接拥有需要手工释放的资源，通常要考虑：

- 析构函数。
- 拷贝构造函数。
- 拷贝赋值运算符。
- 移动构造函数。
- 移动赋值运算符。

更推荐 Rule of Zero：让 `std::vector`、`std::string`、智能指针等成员管理资源，使类不需要自己编写这五个函数。

### 拷贝与移动

```cpp
class Buffer {
public:
    Buffer(const Buffer&) = default;
    Buffer& operator=(const Buffer&) = default;
    Buffer(Buffer&&) noexcept = default;
    Buffer& operator=(Buffer&&) noexcept = default;
    ~Buffer() = default;
};
```

移动后对象仍然有效，但状态通常未指定；可以析构或重新赋值，不应假设它仍保留原值。

## 10. 虚函数与运行时多态

```cpp
class Planner {
public:
    virtual ~Planner() = default;
    virtual Path plan(const State& start, const State& goal) = 0;
};

class RrtConnectPlanner final : public Planner {
public:
    Path plan(const State& start, const State& goal) override;
};
```

关键点：

- 含虚函数的类通常通过虚表实现动态分派，但标准不强制具体布局。
- 通过基类指针删除派生对象时，基类析构函数必须是虚函数。
- `override` 让编译器检查是否真的重写。
- `final` 可禁止继续继承或重写。
- 构造和析构期间的虚调用不会分派到尚未构造或已经析构的派生部分。

### 对象切片

```cpp
void run(Planner planner);       // 错误设计：抽象类甚至不能按值传
void run(const Planner& planner); // 保留多态
```

派生对象按值赋给基类对象时，派生部分会被切掉。多态对象通常通过引用或指针使用。

## 11. RAII 与异常安全

RAII 将资源释放放入析构函数，使所有退出路径都能正确清理：

```cpp
void update() {
    std::lock_guard<std::mutex> lock(mutex_);
    // 即使后续抛出异常，lock 也会释放互斥锁
}
```

### 异常安全级别

- 无保证：异常后对象状态可能损坏。
- 基本保证：对象仍有效、资源不泄漏，但值可能变化。
- 强保证：操作失败时状态不变，类似事务回滚。
- 不抛异常保证：常用 `noexcept` 表达。

析构函数通常不应抛异常。移动构造标记 `noexcept`，可帮助容器在扩容时安全选择移动而不是拷贝。

## 12. 智能指针与所有权

### `std::unique_ptr`

表示唯一所有权，不可拷贝、可以移动：

```cpp
// std::make_unique 从 C++14 开始提供。
auto planner = std::unique_ptr<RrtConnectPlanner>(
    new RrtConnectPlanner());
std::unique_ptr<Planner> base = std::move(planner);
```

### `std::shared_ptr`

通过控制块记录强/弱引用计数。适合真正共享生命周期的对象，但有原子计数开销，也容易掩盖所有权设计。

### `std::weak_ptr`

不增加强引用计数，用于观察共享对象或打破循环引用：

```cpp
if (auto node = weak_node.lock()) {
    node->publish();
}
```

### 原始指针是否还能使用

可以。原始指针适合表达“不拥有对象的可空观察关系”，但必须保证被观察对象生命周期更长。所有权应通过值、引用、容器或智能指针明确表达。

---

# 第三部分：C++11 核心特性

## 13. `auto`、`decltype` 与类型推导

```cpp
auto count = values.size();
auto it = values.begin();
decltype(count) next_count{};
```

`auto` 减少冗长类型，但不要让代码失去语义。`auto` 按值推导时通常会丢掉顶层 `const` 和引用；需要引用时显式写：

```cpp
const auto& state = states.front();
```

## 14. `nullptr`

`nullptr` 有专门类型，可以转换为任意指针，避免整数 `0` 在重载中产生歧义：

```cpp
void open(int mode);
void open(Device* device);

open(nullptr); // 明确选择指针重载
```

## 15. 范围 `for`

```cpp
for (const auto& point : trajectory) {
    validate(point);
}
```

- 只读大对象用 `const auto&`。
- 需要修改元素用 `auto&`。
- 写 `auto` 会复制元素。

## 16. Lambda 表达式

```cpp
double threshold = 0.1;
auto is_near = [threshold](double value) {
    return std::abs(value) < threshold;
};
```

捕获方式：

- `[x]` 按值捕获。
- `[&x]` 按引用捕获。
- `[=]` 默认按值捕获使用到的局部变量。
- `[&]` 默认按引用捕获，异步任务中要格外小心生命周期。
- `[ptr = std::move(owner)]` 是初始化捕获，可转移对象。

异步回调不要随意捕获局部引用或裸 `this`，否则回调执行时对象可能已经销毁。

## 17. 移动语义

移动语义允许把资源从即将销毁的对象转移到新对象，避免深拷贝：

```cpp
std::vector<double> build_path() {
    std::vector<double> path(1000);
    return path; // 通常有返回值优化；必要时可移动
}
```

`std::move` 本身不移动，它只是把表达式转换成可被移动的右值类别；真正移动发生在移动构造或移动赋值中。

不要在返回局部对象时习惯性写 `return std::move(local);`，它可能阻碍返回值优化。

## 18. 完美转发概念

```cpp
template <typename T, typename... Args>
std::unique_ptr<T> make_object(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
```

转发引用 `T&&` 配合类型推导和 `std::forward`，保留实参的左值/右值属性。若项目允许 C++14 及以上，业务代码优先使用标准库提供的 `std::make_unique`；严格 C++11 环境可以直接构造 `unique_ptr` 或自行提供经过审查的工厂函数。

## 19. 其他常用 C++11 特性

### `enum class`

```cpp
enum class ControllerState {
    Idle,
    Running,
    Fault
};
```

作用域明确，不会隐式转换为整数，适合表达状态机。

### `constexpr`

```cpp
constexpr double kPi = 3.141592653589793;
constexpr double deg_to_rad(double deg) {
    return deg * kPi / 180.0;
}
```

允许表达可在编译期求值的常量和函数，同时也可以在运行期使用。

### 委托构造

```cpp
class Config {
public:
    Config() : Config(100, 0.01) {}
    Config(int rate, double dt) : rate_(rate), dt_(dt) {}
private:
    int rate_;
    double dt_;
};
```

### `= default` 与 `= delete`

```cpp
class DeviceConnection {
public:
    DeviceConnection(const DeviceConnection&) = delete;
    DeviceConnection& operator=(const DeviceConnection&) = delete;
};
```

用于明确生成或禁止特殊成员函数。

### `static_assert`

```cpp
static_assert(sizeof(std::uint32_t) == 4, "unexpected integer width");
```

在编译期验证假设。

---

# 第四部分：STL 系统知识

## 20. STL 的组成

STL 的核心不是“很多容器”，而是一套通过迭代器解耦数据结构和算法的泛型体系：

```text
容器 Container
↕ 迭代器 Iterator
算法 Algorithm
+ 函数对象、适配器、分配器
```

算法通常操作半开区间 `[first, last)`，好处是空区间自然表示为 `first == last`，且区间长度可用 `distance` 表达。

## 21. 顺序容器

### `std::vector`

底层是连续动态数组。

- 随机访问：$O(1)$。
- 尾部插入：均摊 $O(1)$。
- 中间插入/删除：$O(n)$。
- 内存连续，缓存友好，可与 C API 通过 `data()` 交互。
- 扩容会重新分配并移动/复制元素，导致指针、引用和迭代器失效。

```cpp
std::vector<double> joints;
joints.reserve(6);
for (double q : input) {
    joints.push_back(q);
}
```

`reserve(n)` 改变容量但不改变大小；`resize(n)` 改变元素数量。

### `std::deque`

通常由分段连续存储组成：

- 首尾插入/删除通常 $O(1)$。
- 随机访问 $O(1)$，但缓存局部性通常弱于 vector。
- 适合双端队列或轨迹缓冲。
- 插入导致的迭代器失效规则复杂，使用前查具体操作规则。

### `std::list`

双向链表：

- 已知位置插入/删除 $O(1)$。
- 查找和随机访问 $O(n)$。
- 节点额外保存两个指针，缓存不友好。
- 插入通常不使其他节点迭代器失效。

不要因为“中间删除快”就默认选 list；找到待删除节点本身可能已经是 $O(n)$。

### `std::array`

固定长度、连续存储、大小属于类型的一部分，适合自由度固定的关节数组。

## 22. 关联容器

### `std::map` / `std::set`

通常用平衡二叉搜索树实现：

- 查找、插入、删除：$O(\log n)$。
- 元素有序。
- `map` 存键值对，`set` 只存键。
- 适合需要有序遍历、范围查询或稳定复杂度的场景。

### `std::unordered_map` / `std::unordered_set`

哈希表：

- 平均查找、插入、删除：$O(1)$。
- 最坏情况：$O(n)$。
- 不保证遍历顺序。
- 性能受哈希函数、负载因子和 rehash 影响。

自定义键需要提供相等判断和哈希，并满足：相等对象必须有相同哈希值。

### `operator[]` 与 `at`

```cpp
std::unordered_map<std::string, int> counts;
int a = counts["joint"];    // 键不存在时插入默认值 0
int b = counts.at("joint"); // 键不存在时抛出异常
```

只查询时不要误用 `operator[]`，否则会修改容器。

## 23. 容器适配器

### `std::stack`

后进先出，常用于表达式解析、DFS、撤销操作。

### `std::queue`

先进先出，常用于 BFS、任务队列。

### `std::priority_queue`

默认是最大堆，`top()` 返回最大元素：

```cpp
using Item = std::pair<int, int>;
std::priority_queue<Item, std::vector<Item>, std::greater<Item>> min_heap;
```

适合 Top-K、Dijkstra、调度。插入和弹出为 $O(\log n)$，查看堆顶为 $O(1)$。

## 24. 迭代器

常见类别从弱到强：

- 输入迭代器。
- 输出迭代器。
- 前向迭代器。
- 双向迭代器。
- 随机访问迭代器。

算法对迭代器能力有要求，例如 `std::sort` 需要随机访问迭代器，因此不能直接排序 `std::list`；list 提供自己的 `sort()`。

### 删除元素的正确写法

```cpp
for (auto it = values.begin(); it != values.end();) {
    if (*it < 0) {
        it = values.erase(it);
    } else {
        ++it;
    }
}
```

对 vector 批量删除常用 erase-remove idiom：

```cpp
values.erase(
    std::remove_if(values.begin(), values.end(),
                   [](int x) { return x < 0; }),
    values.end());
```

## 25. 常用 STL 算法

### 查找与判断

```cpp
auto it = std::find(values.begin(), values.end(), target);
auto jt = std::find_if(values.begin(), values.end(), predicate);
bool all_ok = std::all_of(values.begin(), values.end(), predicate);
bool any_bad = std::any_of(values.begin(), values.end(), predicate);
```

### 排序与选择

```cpp
std::sort(values.begin(), values.end());
std::stable_sort(values.begin(), values.end(), compare);
std::nth_element(values.begin(), values.begin() + k, values.end());
```

- `sort` 平均/标准保证复杂度为 $O(n\log n)$。
- `stable_sort` 保持等价元素原相对顺序。
- `nth_element` 可在线性平均复杂度附近找到第 k 个位置，不保证其他部分完全有序。

### 二分相关算法

已排序区间中：

```cpp
auto first = std::lower_bound(v.begin(), v.end(), x); // 第一个 >= x
auto last  = std::upper_bound(v.begin(), v.end(), x); // 第一个 > x
bool found = std::binary_search(v.begin(), v.end(), x);
```

### 变换与归约

```cpp
std::transform(input.begin(), input.end(), output.begin(), func);
double sum = std::accumulate(values.begin(), values.end(), 0.0);
```

`accumulate` 的初值决定累加类型；对浮点数组若写 `0`，可能发生整数累加，应该写 `0.0`。

## 26. 容器选择速查

| 需求 | 首选 | 理由 |
|---|---|---|
| 默认动态序列 | `vector` | 连续、缓存友好、接口完整 |
| 固定长度 | `array` | 无动态分配、长度明确 |
| 两端频繁进出 | `deque` | 首尾操作高效 |
| FIFO | `queue` | 语义清晰 |
| LIFO | `stack` | 语义清晰 |
| 动态获取最大/最小值 | `priority_queue` | 堆顶 $O(1)$ |
| 有序键值与范围查询 | `map` | $O(\log n)$ 且有序 |
| 平均常数时间查找 | `unordered_map` | 哈希查找 |
| 唯一集合 | `set` / `unordered_set` | 按是否需要顺序选择 |

选择时依次问：是否需要顺序、随机访问、两端操作、稳定迭代器、范围查询、最坏复杂度保证和缓存局部性。

---

# 第五部分：数据结构

## 27. 复杂度基础

### 时间复杂度

常见增长顺序：

$$
O(1) < O(\log n) < O(n) < O(n\log n) < O(n^2) < O(2^n) < O(n!)
$$

复杂度描述输入规模增大时的增长趋势，忽略常数项和低阶项，但工程中缓存、分配和数据分布仍会影响实际速度。

### 空间复杂度

关注算法额外使用的存储，不仅是显式容器，还包括递归调用栈、哈希表、辅助数组等。

### 均摊复杂度

`vector::push_back` 单次扩容可能是 $O(n)$，但连续多次插入的平均成本为均摊 $O(1)$。均摊不是“每次都是常数”，而是操作序列总成本可控。

## 28. 数组与动态数组

数组元素连续存储，因此：

- 按下标访问地址可直接计算，为 $O(1)$。
- 中间插入删除需要移动后续元素，为 $O(n)$。
- 连续布局有良好缓存局部性。
- 二维数组通常按行连续存储，遍历顺序会影响缓存命中。

典型题型：双指针、滑动窗口、前缀和、二分查找、排序、区间合并。

## 29. 链表

### 单链表节点

```cpp
struct ListNode {
    int value{};
    ListNode* next{nullptr};
};
```

特点：

- 已知节点后插入/删除为 $O(1)$。
- 按位置访问为 $O(n)$。
- 节点不连续，缓存局部性差。
- 裸指针实现必须明确所有权；业务代码可由容器或智能指针管理。

### 反转链表

```cpp
ListNode* reverse_list(ListNode* head) {
    ListNode* previous = nullptr;
    ListNode* current = head;

    while (current != nullptr) {
        ListNode* next = current->next;
        current->next = previous;
        previous = current;
        current = next;
    }
    return previous;
}
```

循环不变量：`previous` 始终是已经反转部分的头，`current` 是尚未处理部分的头。时间 $O(n)$，额外空间 $O(1)$。

### 快慢指针

- 判断环：slow 每次一步，fast 每次两步；有环则相遇。
- 找中点：fast 到尾部时 slow 在中间。
- 找倒数第 k 个：先让 fast 领先 k 步，再同步移动。

## 30. 栈与队列

### 栈

LIFO。典型应用：

- 函数调用栈。
- 括号匹配。
- 表达式求值。
- 单调栈。
- 迭代 DFS。

### 队列

FIFO。典型应用：

- BFS。
- 生产者消费者。
- 消息/任务调度。
- 树的层序遍历。

### 循环队列

固定数组配合头尾索引，避免普通数组出队时搬移元素。必须统一“空”和“满”的判定：可以牺牲一个槽位，也可以额外维护元素数量。

### 单调队列

队列内部维持单调性，可在 $O(n)$ 总时间内求滑动窗口最大值。每个元素最多入队、出队一次。

## 31. 哈希表

哈希表把键映射到桶：

$$
\text{index}=h(\text{key}) \bmod \text{bucket\_count}
$$

冲突处理：

- 链地址法：同一桶中的元素形成链或小容器。
- 开放地址法：冲突后按探测规则寻找其他槽位。

关键概念：

- 负载因子 = 元素数 / 桶数。
- 负载过高会增加冲突，触发 rehash。
- rehash 会改变桶布局，并可能使迭代器失效。
- 哈希表平均 $O(1)$，但碰撞严重时最坏 $O(n)$。

常见题型：两数之和、频率统计、去重、前缀和计数、LRU 的索引表。

## 32. 树

### 基本术语

- 根、父节点、子节点、叶节点。
- 深度：从根到节点的边数。
- 高度：从节点到最深叶子的最长路径。
- 二叉树每个节点最多有两个孩子。

### 遍历

```cpp
struct TreeNode {
    int value{};
    TreeNode* left{nullptr};
    TreeNode* right{nullptr};
};
```

递归前序：

```cpp
void preorder(TreeNode* root, std::vector<int>& output) {
    if (root == nullptr) {
        return;
    }
    output.push_back(root->value);
    preorder(root->left, output);
    preorder(root->right, output);
}
```

层序遍历：

```cpp
std::vector<std::vector<int>> level_order(TreeNode* root) {
    std::vector<std::vector<int>> result;
    if (root == nullptr) {
        return result;
    }

    std::queue<TreeNode*> q;
    q.push(root);

    while (!q.empty()) {
        const std::size_t level_size = q.size();
        std::vector<int> level;
        level.reserve(level_size);

        for (std::size_t i = 0; i < level_size; ++i) {
            TreeNode* node = q.front();
            q.pop();
            level.push_back(node->value);

            if (node->left != nullptr) q.push(node->left);
            if (node->right != nullptr) q.push(node->right);
        }
        result.push_back(std::move(level));
    }
    return result;
}
```

### 二叉搜索树 BST

对每个节点，左子树键小于节点，右子树键大于节点（重复键策略需自行定义）。平均查找 $O(\log n)$，退化为链时最坏 $O(n)$。

### 平衡树

- AVL 树严格控制左右子树高度差，查询稳定，旋转可能较频繁。
- 红黑树使用颜色和结构约束保持近似平衡，插入删除综合性能好，常用于 `map/set`。
- B/B+ 树适合磁盘和数据库，分支多、树高低；B+ 树数据通常集中在叶节点并支持范围扫描。

### 堆

完全二叉树，常用数组存储。以 0 为起点时：

```text
parent(i) = (i - 1) / 2
left(i)   = 2 * i + 1
right(i)  = 2 * i + 2
```

- 建堆可做到 $O(n)$。
- 插入和弹出堆顶为 $O(\log n)$。
- 读取堆顶为 $O(1)$。

典型题型：Top-K、合并 K 个有序序列、中位数数据流、Dijkstra。

## 33. 图

图由顶点和边组成，可分为有向/无向、带权/无权。

### 邻接矩阵

- 空间 $O(V^2)$。
- 判断两点是否相邻为 $O(1)$。
- 适合稠密图或顶点较少的图。

### 邻接表

- 空间 $O(V+E)$。
- 遍历某顶点邻边高效。
- 适合稀疏图，是算法题与机器人路网的常见表示。

```cpp
using Graph = std::vector<std::vector<int>>;
```

### 图中的访问标记

无向图必须避免沿同一边来回；有向图做环检测时常用三种状态：

```text
0 = 未访问
1 = 当前递归路径中
2 = 已完成
```

## 34. 并查集

并查集维护不相交集合，支持：

- `find(x)`：找到代表元。
- `unite(a,b)`：合并集合。
- `same(a,b)`：判断连通性。

```cpp
class DisjointSet {
public:
    explicit DisjointSet(int n) : parent_(n), rank_(n, 0) {
        std::iota(parent_.begin(), parent_.end(), 0);
    }

    int find(int x) {
        if (parent_[x] != x) {
            parent_[x] = find(parent_[x]);
        }
        return parent_[x];
    }

    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return false;

        if (rank_[a] < rank_[b]) std::swap(a, b);
        parent_[b] = a;
        if (rank_[a] == rank_[b]) ++rank_[a];
        return true;
    }

private:
    std::vector<int> parent_;
    std::vector<int> rank_;
};
```

路径压缩 + 按秩合并后，单次操作的均摊复杂度接近 $O(1)$。典型应用：连通分量、Kruskal 最小生成树、动态连通性。

## 35. Trie 与 LRU

### Trie

前缀树按字符逐层组织，查询复杂度与字符串长度有关，适合前缀匹配和词典。代价是节点多、内存开销大。

### LRU Cache

需要同时满足：

- 查找 $O(1)$：哈希表。
- 更新最近使用顺序 $O(1)$：双向链表。

哈希表保存 `key → 链表迭代器`。访问后将节点移动到链表头；容量满时删除链表尾，并同步删除哈希索引。

---

# 第六部分：算法思想与模板

## 36. 解题统一流程

拿到题目先回答：

1. 输入规模与数据范围是什么？
2. 是否有序、是否允许修改、是否有重复或负数？
3. 目标是查找、计数、最优值、可行性还是枚举全部解？
4. 暴力解法是什么，瓶颈在哪里？
5. 能否利用顺序、单调性、重复子问题、图结构或空间换时间？
6. 边界：空输入、单元素、全重复、溢出、无解如何处理？

回答算法题时先说思路和不变量，再写代码，最后给复杂度与测试用例。

## 37. 二分查找

适用前提不是“数组”，而是存在单调性或可二分的判定函数。

### 查找第一个不小于目标的位置

```cpp
int lower_bound_index(const std::vector<int>& nums, int target) {
    int left = 0;
    int right = static_cast<int>(nums.size()); // [left, right)

    while (left < right) {
        int mid = left + (right - left) / 2;
        if (nums[mid] < target) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    return left;
}
```

循环不变量：答案始终位于半开区间 `[left, right)`。时间 $O(\log n)$，空间 $O(1)$。

常见错误：

- `left <= right` 与 `[left,right)` 模板混用。
- `mid = (left + right) / 2` 可能整数溢出。
- 找到相等就返回，无法处理“第一个/最后一个”边界。

### 答案二分

若问题的答案存在单调判定，如“速度为 x 时能否在规定时间完成”，可对答案空间二分，而不是对数组下标二分。

## 38. 排序

### 选择、冒泡、插入

| 算法 | 平均 | 最坏 | 稳定 | 特点 |
|---|---:|---:|---|---|
| 选择排序 | $O(n^2)$ | $O(n^2)$ | 否 | 交换次数少 |
| 冒泡排序 | $O(n^2)$ | $O(n^2)$ | 是 | 教学直观，工程很少使用 |
| 插入排序 | $O(n^2)$ | $O(n^2)$ | 是 | 小规模或近乎有序时表现好 |

### 归并排序

分治：把序列拆成两半分别排序，再线性合并。

- 时间：$O(n\log n)$。
- 额外空间：数组版本通常 $O(n)$。
- 稳定。
- 适合链表排序、外部排序和需要稳定性的场景。

### 快速排序

```cpp
void quick_sort(std::vector<int>& nums, int left, int right) {
    if (left >= right) return;

    int i = left;
    int j = right;
    const int pivot = nums[left + (right - left) / 2];

    while (i <= j) {
        while (nums[i] < pivot) ++i;
        while (nums[j] > pivot) --j;
        if (i <= j) {
            std::swap(nums[i], nums[j]);
            ++i;
            --j;
        }
    }

    if (left < j) quick_sort(nums, left, j);
    if (i < right) quick_sort(nums, i, right);
}
```

- 平均 $O(n\log n)$，最坏 $O(n^2)$。
- 原地版本额外栈空间平均 $O(\log n)$。
- 通常不稳定。
- 随机 pivot、三数取中和小区间插入排序可改善工程表现。

`std::sort` 的实现通常采用 introsort 思路：快速排序为主，递归过深时切换堆排序，小区间使用插入排序，从而保证最坏 $O(n\log n)$。

### 堆排序

- 时间 $O(n\log n)$。
- 原地，额外空间 $O(1)$。
- 不稳定。
- 缓存局部性通常不如快速排序。

### 排序选择

- 普通内存序列：优先 `std::sort`。
- 必须稳定：`std::stable_sort`。
- 只需第 k 个：`std::nth_element`。
- 数据范围小的整数：计数/桶/基数排序。
- 数据大于内存：外部归并排序。

## 39. 双指针、滑动窗口与前缀和

### 双指针

- 对撞指针：有序数组两数和、回文判断。
- 快慢指针：链表环、原地去重。
- 同向指针：区间压缩、合并。

### 滑动窗口

适合连续子数组/子串，并且窗口扩大与缩小具有可维护性质。

```cpp
int min_subarray_len(const std::vector<int>& nums, int target) {
    int best = std::numeric_limits<int>::max();
    int left = 0;
    long long sum = 0;

    for (int right = 0; right < static_cast<int>(nums.size()); ++right) {
        sum += nums[right];
        while (sum >= target) {
            best = std::min(best, right - left + 1);
            sum -= nums[left++];
        }
    }
    return best == std::numeric_limits<int>::max() ? 0 : best;
}
```

此模板依赖元素非负；若存在负数，窗口和不再单调，通常要换前缀和 + 哈希或单调队列。

### 前缀和

定义 `prefix[i]` 为前 i 个元素之和，则半开区间 `[l,r)` 的和为：

$$
\operatorname{sum}(l,r)=\operatorname{prefix}[r]-\operatorname{prefix}[l]
$$

预处理 $O(n)$，单次区间查询 $O(1)$。

## 40. 递归、回溯与分治

### 递归三要素

1. 函数定义：当前层解决什么子问题。
2. 终止条件：何时停止。
3. 递推关系：如何缩小问题。

递归要计入调用栈空间。深度不可控时考虑显式栈或迭代。

### 回溯模板

```cpp
void backtrack(const std::vector<int>& nums,
               int start,
               std::vector<int>& path,
               std::vector<std::vector<int>>& result) {
    result.push_back(path);

    for (int i = start; i < static_cast<int>(nums.size()); ++i) {
        path.push_back(nums[i]);
        backtrack(nums, i + 1, path, result);
        path.pop_back();
    }
}
```

回溯是深度优先枚举：选择 → 递归 → 撤销选择。剪枝必须保证不会删除可能产生正确答案的分支。

### 分治

把问题拆为相互独立的子问题，再合并结果。归并排序、快速排序、最近点对都属于分治思想。

## 41. DFS 与 BFS

### DFS

```cpp
void dfs(int u,
         const std::vector<std::vector<int>>& graph,
         std::vector<bool>& visited) {
    visited[u] = true;
    for (int v : graph[u]) {
        if (!visited[v]) {
            dfs(v, graph, visited);
        }
    }
}
```

适合连通分量、路径枚举、拓扑/环检测、回溯搜索。邻接表复杂度 $O(V+E)$。

### BFS

```cpp
std::vector<int> bfs_distance(
    int start,
    const std::vector<std::vector<int>>& graph) {
    std::vector<int> distance(graph.size(), -1);
    std::queue<int> q;

    distance[start] = 0;
    q.push(start);

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v : graph[u]) {
            if (distance[v] == -1) {
                distance[v] = distance[u] + 1;
                q.push(v);
            }
        }
    }
    return distance;
}
```

BFS 能求无权图的最短边数，因为节点按距离分层首次到达。加权图不能直接使用普通 BFS。

## 42. 拓扑排序

只适用于有向无环图 DAG。Kahn 算法：

1. 统计每个节点入度。
2. 将入度为 0 的节点入队。
3. 弹出节点并删除其出边，新的入度 0 节点入队。
4. 若最终处理节点数小于总节点数，说明存在环。

应用：任务依赖、课程安排、构建系统。

## 43. 最短路径

### Dijkstra

适用于边权非负的单源最短路：

```cpp
using Edge = std::pair<int, int>; // (neighbor, weight)

std::vector<long long> dijkstra(
    int start,
    const std::vector<std::vector<Edge>>& graph) {
    const long long inf = std::numeric_limits<long long>::max() / 4;
    std::vector<long long> dist(graph.size(), inf);

    using State = std::pair<long long, int>; // (distance, node)
    std::priority_queue<State, std::vector<State>, std::greater<State>> pq;

    dist[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        const State top = pq.top();
        pq.pop();

        const long long current_dist = top.first;
        const int u = top.second;

        if (current_dist != dist[u]) continue; // 丢弃过期堆项

        for (const Edge& edge : graph[u]) {
            const int v = edge.first;
            const int weight = edge.second;
            if (dist[v] > current_dist + weight) {
                dist[v] = current_dist + weight;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}
```

使用邻接表和二叉堆时，复杂度约为 $O((V+E)\log V)$。含负权边时不能使用 Dijkstra。

### Bellman-Ford

对所有边重复松弛 $V-1$ 轮，可处理负权，并在第 $V$ 轮仍可松弛时检测从源点可达的负环。复杂度 $O(VE)$。

### Floyd-Warshall

全源最短路动态规划：

$$
d_{ij}=\min(d_{ij}, d_{ik}+d_{kj})
$$

时间 $O(V^3)$，空间 $O(V^2)$，适合顶点较少的稠密图。

### A*

A* 使用：

$$
f(n)=g(n)+h(n)
$$

- $g(n)$：起点到当前节点的已知代价。
- $h(n)$：当前到目标的启发估计。
- 若启发函数可采纳，即不高估真实剩余代价，A* 可保证最优。
- 启发越有信息，通常扩展节点越少；$h=0$ 时退化为 Dijkstra。

机器人栅格规划中常用曼哈顿距离或欧氏距离，但必须与允许的运动方式和边代价匹配。

## 44. 最小生成树

### Kruskal

按边权升序处理边，若两端不在同一连通分量则选中并合并。使用并查集，复杂度主要来自排序，为 $O(E\log E)$。

### Prim

从一个节点开始，每次选择连接已选集合和未选集合的最小边。配合堆适合邻接表表示。

最小生成树连接所有节点且总边权最小，不等于从某个源点到各点的最短路径树。

## 45. 贪心算法

贪心每一步选择局部最优，希望得到全局最优。正确性需要交换论证、反证或问题结构证明，不能因为“看起来合理”就使用。

常见问题：

- 区间调度：按结束时间排序。
- Huffman 编码：每次合并最小频率节点。
- Kruskal/Prim。
- 部分背包。

0/1 背包不能按价值密度贪心，因为物品不可拆分。

## 46. 动态规划

### 识别条件

- 最优子结构。
- 重叠子问题。
- 状态可以完整描述决策所需信息。

### 五步法

1. 定义状态 `dp[...]` 的含义。
2. 写状态转移。
3. 初始化边界。
4. 确定遍历顺序。
5. 给出答案位置并验证小样例。

### 0/1 背包

每件物品最多选一次。设 `dp[c]` 为容量 c 的最大价值：

```cpp
int knapsack_01(const std::vector<int>& weight,
                const std::vector<int>& value,
                int capacity) {
    std::vector<int> dp(capacity + 1, 0);

    for (std::size_t i = 0; i < weight.size(); ++i) {
        for (int c = capacity; c >= weight[i]; --c) {
            dp[c] = std::max(dp[c], dp[c - weight[i]] + value[i]);
        }
    }
    return dp[capacity];
}
```

容量必须倒序遍历，否则同一物品会在当前轮被重复使用，变成完全背包语义。

### 最长递增子序列 LIS

- 基础 DP：`dp[i]` 表示以 i 结尾的最长递增子序列，时间 $O(n^2)$。
- 贪心 + 二分维护每种长度的最小尾值，可做到 $O(n\log n)$。

### 动态规划与贪心的区别

DP 保留多个子状态并系统比较；贪心只保留当前局部选择。若不能证明贪心选择性质，优先考虑 DP 或搜索。

## 47. 算法与机器人项目的对应关系

| 知识 | 机器人应用 |
|---|---|
| `vector` / `array` | 关节状态、轨迹点、点云索引 |
| `deque` / queue | 轨迹缓存、消息缓冲、BFS |
| `priority_queue` | A*、Dijkstra、候选目标排序 |
| 哈希表 | ID 映射、状态缓存、去重 |
| 树 | TF 层级、RRT 搜索树、行为树 |
| 图 | 路网、拓扑地图、任务依赖 |
| BFS/DFS | 连通域、探索、树遍历 |
| 二分 | 参数阈值、答案空间搜索 |
| 动态规划 | 离散优化、序列匹配、资源分配 |
| 并查集 | 地图区域合并、连通性、聚类辅助 |
| 最近邻 | RRT 扩展、点云查询 |

算法能力不只是刷题。面试时要能把“数据结构选择—复杂度—实时性—内存行为”落到机器人系统。

---

# 第七部分：高频陷阱与工程规范

## 48. 内存与生命周期陷阱

### 空悬指针与局部引用

```cpp
const int* bad_pointer() {
    int local = 42;
    return &local; // 错误：返回后 local 生命周期结束
}
```

返回局部对象的指针或引用会产生空悬访问。返回值通常可由复制省略或移动高效返回，不需要冒险。

### 容器修改后的失效引用

```cpp
std::vector<int> values{1, 2, 3};
int* first = &values[0];
values.push_back(4); // 可能扩容，first 可能失效
```

不要跨容器结构修改长期保存迭代器、引用或指针。若语义允许，保存下标并在使用时重新访问。

### C++ 常见未定义行为清单

- 数组或容器越界。
- 解引用空指针、空悬指针或无效迭代器。
- 读取未初始化局部变量。
- 有符号整数溢出。
- 修改字符串字面量。
- `new/free` 或 `malloc/delete` 混用。
- 通过非虚析构基类指针删除派生对象。
- 多线程数据竞争。

## 49. 数值与类型陷阱

### 有符号与无符号

`size()` 返回 `std::size_t`。负整数与无符号值比较时可能被转换为巨大正数。循环优先使用 `std::size_t`，需要转为 `int` 时先验证范围。

### 整数除法

```cpp
double wrong = 1 / 2;       // 0.0
double correct = 1.0 / 2.0; // 0.5
```

### 浮点比较

```cpp
bool nearly_equal(double a, double b,
                  double abs_tol = 1e-9,
                  double rel_tol = 1e-6) {
    const double diff = std::abs(a - b);
    return diff <= std::max(
        abs_tol, rel_tol * std::max(std::abs(a), std::abs(b)));
}
```

阈值应结合物理单位、数据尺度和传感器精度，不能机械使用固定 epsilon。

### 溢出

- 计算中点用 `left + (right - left) / 2`。
- 距离、计数、时间戳根据范围选择 `int64_t/uint64_t`。
- 加法前确认“无穷值”仍有余量，避免最短路计算溢出。

## 50. API 设计原则

- 用类型表达状态和单位，减少多个 `double` 参数混淆。
- 大对象只读输入用 `const T&`，需要所有权转移时显式使用值或智能指针。
- 一个函数只承担一个清晰职责。
- 用 `enum class` 代替难以理解的布尔模式参数。
- 明确错误策略：异常、错误码、可选结果不要随意混用。
- 算法核心与 ROS、网络、文件 I/O 解耦，便于单元测试。
- 对外接口隐藏不变量和容器实现细节。

## 51. 异常、断言与错误码

- `assert` 用于验证程序员假设，发布构建可能关闭，不能处理用户输入或硬件故障。
- 异常适合当前层无法合理处理的异常情况；实时控制路径要评估其时延与团队规范。
- 错误码适合 C 接口、实时路径或必须显式处理的故障，但要防止返回值被忽略。
- `noexcept` 是承诺；函数若实际抛出异常会调用 `std::terminate`。
- 析构函数一般不抛异常。

## 52. 并发基础

两个线程并发访问同一内存位置，至少一个写且没有同步，就构成数据竞争，行为未定义。

```cpp
class ThreadSafeState {
public:
    void set(double value) {
        std::lock_guard<std::mutex> lock(mutex_);
        value_ = value;
    }

    double get() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return value_;
    }

private:
    mutable std::mutex mutex_;
    double value_{0.0};
};
```

工程原则：

- 临界区尽量短，不持锁执行阻塞 I/O、长计算或未知回调。
- 多把锁使用固定顺序，或用 `std::lock` 协调获取。
- `atomic` 适合简单独立状态，不自动保证多个字段组成的一致快照。
- 控制线程与通信线程交换数据时，必须同时处理时间戳、超时和队列容量。

## 53. 测试意识

每个模块至少覆盖：

- 正常输入、空输入、最小输入。
- 上下边界、重复值、负值和极端值。
- 异常路径、容量边界与性能边界。
- NaN/Inf、单位错误和时间戳陈旧。

算法测试不仅验证答案，还要验证不变量。例如排序结果要同时满足“非降序”和“元素多重集合没有变化”。

---

# 第八部分：高频面试问答

## 54. C/C++ 核心问题

### 1. 指针和引用有什么区别？

指针保存地址，可为空、可改指向，需要显式解引用；引用是对象别名，初始化时必须绑定，语法上不可改绑。二者都不拥有对象，也都可能因对象提前销毁而悬空。可空观察关系用指针，必须存在的参数通常用引用。

### 2. `struct` 和 `class` 有什么区别？

语言能力基本相同，主要区别是默认权限：struct 默认公有成员和公有继承，class 默认私有。工程上常用 struct 表达简单数据聚合，用 class 表达维护不变量的抽象。

### 3. 深拷贝与浅拷贝是什么？

浅拷贝复制成员值；若成员是拥有资源的裸指针，两个对象会指向同一资源，可能重复释放。深拷贝复制独立资源。更好的设计是用标准容器和智能指针表达所有权，遵循 Rule of Zero。

### 4. 为什么多态基类需要虚析构？

通过基类指针删除派生对象时，虚析构保证派生析构和基类析构依次执行。否则行为未定义。若类明确不作为多态基类，不必无条件添加虚析构。

### 5. 虚函数如何实现？

主流 ABI 通常为多态对象保存虚表指针，调用时按动态类型查虚函数表。标准不强制具体布局。动态分派要求通过指针或引用；构造、析构期间不会分派到尚未构造或已销毁的派生部分。

### 6. 什么是 RAII？

构造时获取资源，析构时释放资源，把清理与作用域绑定。内存、锁、文件、socket 和设备句柄都可使用 RAII，它也是异常安全的基础。

### 7. `unique_ptr` 与 `shared_ptr` 怎么选？

默认用 unique_ptr 表达唯一所有权；只有多个所有者确实需要共同延长生命周期时才用 shared_ptr。观察共享对象但不拥有时用 weak_ptr。shared_ptr 不能自动解决循环引用和架构所有权混乱。

### 8. `std::move` 做了什么？

它主要进行类型转换，把表达式转为可匹配移动重载的右值类别，本身不搬资源。真正的资源转移发生在移动构造或移动赋值中；移动后的对象仍有效，但值通常未指定。

### 9. vector 扩容发生什么？

vector 申请更大的连续空间，把旧元素移动或复制过去，销毁旧元素并释放旧空间。扩容倍率不应依赖具体实现。扩容会使原有指针、引用和迭代器失效，已知规模时可用 reserve 降低扩容次数。

### 10. `reserve` 和 `resize` 的区别？

reserve 只改变容量下限，不创建逻辑元素，size 不变；resize 改变元素数量，扩大时构造新元素，缩小时销毁尾部元素。

### 11. map 和 unordered_map 怎么选？

需要有序遍历、范围查询或稳定 $O(\log n)$ 时选 map；只需按键快速查找且哈希可靠时选 unordered_map，平均 $O(1)$。还要考虑最坏复杂度、内存、rehash 和自定义键哈希。

### 12. `push_back` 与 `emplace_back` 的区别？

push_back 插入已有对象或临时对象；emplace_back 将参数转发给元素构造函数并在容器中构造。emplace 不一定更快，也可能匹配意外构造函数；对象已存在时直接 push_back 更清晰。

### 13. 什么是迭代器失效？

容器结构变化后，原迭代器、引用或指针不再有效。vector 扩容使全部失效，erase 使删除位置及之后的位置失效；不同容器规则不同。循环删除时应接收 erase 返回的新迭代器。

## 55. 数据结构与算法问题

### 14. 数组和链表如何选择？

数组连续、随机访问 $O(1)$、缓存友好，但中间插删 $O(n)$；链表已知节点插删 $O(1)$，但定位节点 $O(n)$、额外指针开销大、缓存差。多数动态序列优先 vector，只有地址稳定或频繁已知位置插删等明确需求才选链式结构。

### 15. 哈希冲突如何解决？

常见方法是链地址法和开放地址法。性能取决于哈希质量、负载因子、扩容策略和数据分布；平均 $O(1)$ 不等于最坏 $O(1)$。

### 16. BST 为什么会退化？

按有序序列插入普通 BST 时，节点可能全部落在同一侧，树高变成 n，操作退化为 $O(n)$。AVL 和红黑树通过旋转与平衡约束把高度控制在 $O(\log n)$。

### 17. BFS 为什么能求无权最短路？

BFS 按距离分层扩展，队列保证距离 d 的节点在 d+1 前处理，因此节点第一次访问时就是最少边数。边权不同时这一性质失效，应使用 Dijkstra 等算法。

### 18. Dijkstra 为什么不能处理负权边？

它依赖“当前最小暂定距离节点取出后，距离已经确定”的贪心性质。负权边可能从后续节点绕回并继续降低已确定距离，使前提失效。

### 19. 快排和归并如何选择？

归并稳定、最坏 $O(n\log n)$，但数组实现需额外 $O(n)$ 空间；快排通常原地、缓存友好、平均快，但不稳定，朴素实现最坏 $O(n^2)$。普通数组使用标准库排序，链表、外部排序或强稳定性需求更偏向归并。

### 20. 动态规划和贪心如何区分？

贪心每步只保留局部选择，必须证明贪心选择性质；DP 保留多个状态并比较合法转移。无法证明局部选择必然导向全局最优时，不能只凭直觉使用贪心。

### 21. 如何判断可以二分？

关键是存在单调判定：候选值变化时，答案从 false 统一转为 true，或反之。二分既可用于有序数组位置，也可用于答案空间；必须明确区间语义和边界不变量。

### 22. 复杂度和实际运行时间有什么区别？

复杂度描述渐近增长，不包含常数、缓存、分支预测、内存分配和编译优化。大规模通常优先低复杂度，但小规模工程决策还要结合基准测试、内存与实现复杂度。

---

# 第九部分：手写代码与复习安排

## 56. 第一优先级手写题

- [ ] 二分查找：精确值、左边界、右边界。
- [ ] 反转链表、中点、环检测、倒数第 k 个节点。
- [ ] 栈实现括号匹配；队列实现 BFS。
- [ ] 二叉树前中后序和层序遍历。
- [ ] 快速排序与归并排序。
- [ ] 哈希表解决两数之和与频率统计。
- [ ] Top-K：堆与 `nth_element` 两种方案。
- [ ] Dijkstra、拓扑排序、并查集。
- [ ] 0/1 背包和一类二维 DP。

## 57. 第二优先级手写题

- [ ] 双指针、滑动窗口、前缀和 + 哈希。
- [ ] 单调栈和单调队列。
- [ ] 回溯：子集、组合、排列。
- [ ] Kruskal 与最小生成树。
- [ ] LIS 的 $O(n^2)$ 与 $O(n\log n)$ 解法。
- [ ] LRU Cache。

## 58. 每道题的验收格式

```text
问题与约束：
暴力解法：
核心不变量：
数据结构选择：
时间复杂度：
空间复杂度：
边界用例：
替代方案与取舍：
```

不能只以“代码通过”为完成标准。

## 59. 四周复习路径

### 第 1 周：语言与对象模型

- Day 1：类型、初始化、生命周期、指针、引用、const。
- Day 2：编译、链接、头文件、ODR、static、inline。
- Day 3：封装、构造析构、拷贝与移动。
- Day 4：继承、组合、多态、虚析构、对象切片。
- Day 5：RAII、异常安全、智能指针。
- Day 6：闭卷问答 + 45 分钟代码测试。

### 第 2 周：C++11 与 STL

- Day 1：auto、nullptr、范围 for、列表初始化。
- Day 2：Lambda、函数对象、STL 算法。
- Day 3：右值引用、移动语义、`std::move`。
- Day 4：vector、deque、list、array。
- Day 5：map/set、unordered 容器、迭代器失效。
- Day 6：用 STL 实现轨迹缓存和 Top-K。

### 第 3 周：数据结构与基础算法

- Day 1：数组、链表、栈、队列。
- Day 2：哈希、前缀和、双指针、滑动窗口。
- Day 3：树、BST、堆和四种遍历。
- Day 4：二分、快排、归并。
- Day 5：DFS、BFS 和图表示。
- Day 6：90 分钟综合手写与复杂度复盘。

### 第 4 周：进阶算法与面试输出

- Day 1：Dijkstra、A*、拓扑排序。
- Day 2：并查集、Kruskal、Prim。
- Day 3：贪心证明、回溯剪枝。
- Day 4：DP 五步法、0/1 背包、LIS。
- Day 5：C++ 与算法全真模拟面试。
- Day 6：错题写入 [[../求职准备/错题与复盘日志]]，并用机器人项目解释 10 个数据结构选择。

---

# 第十部分：掌握程度自检

## 60. “熟练掌握 C/C++”

- [ ] 能解释编译、链接和常见错误分类。
- [ ] 能安全使用值、指针、引用和 `const`。
- [ ] 能解释对象生命周期、RAII 和异常安全。
- [ ] 能正确设计构造、析构、拷贝和移动。
- [ ] 能说明虚函数、虚析构和对象切片。
- [ ] 能用智能指针表达所有权，而不是消灭所有裸指针。
- [ ] 能识别越界、悬空、未初始化和数据竞争。

## 61. “熟练使用 STL”

- [ ] 能根据访问模式选择容器，而不是只背复杂度。
- [ ] 熟悉 vector 扩容和主要容器的迭代器失效。
- [ ] 能使用 sort、find、lower_bound、transform、accumulate。
- [ ] 能编写自定义比较器和哈希。
- [ ] 能说明 map 与 unordered_map 的取舍。
- [ ] 能用 priority_queue 实现 Top-K 或 Dijkstra。

## 62. “掌握数据结构与算法”

- [ ] 能实现链表、树遍历、堆、图遍历和并查集核心操作。
- [ ] 能写边界一致的二分模板。
- [ ] 能解释快速排序分区与最坏情况。
- [ ] 能按权值条件选择 BFS、Dijkstra、Bellman-Ford 或 A*。
- [ ] 能定义 DP 状态并说明遍历顺序。
- [ ] 能解释贪心依据、时间和空间复杂度。

## 63. 简历表述的面试底线

写“熟练掌握”后，面试官可以合理追问机制、边界和现场代码。至少准备：

1. vector 扩容、迭代器失效、reserve/resize。
2. 虚函数、虚析构、对象切片、构造析构顺序。
3. RAII、智能指针、移动语义。
4. map/unordered_map、vector/list/deque 的选择。
5. 二分、快排、链表反转、树遍历、BFS/DFS。
6. 动态规划状态定义与 0/1 背包倒序原因。
7. priority_queue + Dijkstra 及负权限制。
8. 轨迹缓存、RRT 树、导航图和消息队列中的实际选型。

若这些内容无法稳定回答，应暂时将简历措辞改为“熟悉”，并按本文件的四周路径补齐证据。
