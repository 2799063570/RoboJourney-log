# C++ 现代用法

> 目标：把 C++11 及之后最常用的语言与标准库能力，落实到 ROS2 和运动规划代码中；优先掌握能提升安全性、可读性和资源管理质量的写法。

## 先记住的结论

- 默认使用 RAII 管理资源：对象离开作用域时自动释放锁、文件句柄和内存。
- 默认传递 `const T&`；明确需要转移所有权时才使用 `T&&` 与 `std::move`。
- 默认使用 `std::unique_ptr`；确有共享生命周期时再使用 `std::shared_ptr`。
- 默认使用标准库算法、范围 `for` 循环和 `auto`，但不要让 `auto` 隐藏关键类型或所有权。
- 编译时尽量开启 `-Wall -Wextra -Wpedantic`（或 MSVC 的 `/W4`），把警告当成待修复问题。

## 学习路线

| 优先级 | 主题 | 在机器人软件中的用途 |
| --- | --- | --- |
| 1 | RAII、智能指针、移动语义 | 节点资源、插件对象、缓存和锁的生命周期 |
| 2 | `const`、引用、值类别 | 回调参数、消息处理、避免不必要拷贝 |
| 3 | `enum class`、`constexpr`、`nullptr` | 类型安全的状态机、配置与常量 |
| 4 | lambda、`std::function` | 定时器、回调、策略注入 |
| 5 | 并发与同步 | 多线程 executor 中共享状态保护 |
| 6 | 模板与泛型 | 数值工具、容器适配与可复用算法 |

## ROS2 常用写法

### 用 RAII 缩小临界区

```cpp
std::mutex state_mutex;
RobotState latest_state;

void onState(const RobotState& msg) {
  std::lock_guard<std::mutex> lock(state_mutex);
  latest_state = msg;
}
```

锁只覆盖共享数据的读写；规划、日志和 I/O 等耗时操作应放在锁外。更多背景见 [[CPP/C++生存指南#stdmutex 互斥锁]]。

### 明确所有权

```cpp
auto planner = std::make_unique<LocalPlanner>(); // 单一拥有者

void setPlanner(std::unique_ptr<LocalPlanner> next) {
  planner = std::move(next); // 明确转移所有权
}
```

不要用裸指针表达拥有关系。若接口只需观察对象，传 `const LocalPlanner&` 或 `LocalPlanner*`（可为空）即可。

### 用 `enum class` 表达状态

```cpp
enum class ExecutionState { idle, planning, executing, failed };

constexpr double kControlPeriodSeconds = 0.01;
```

`enum class` 不会与普通整数隐式混用；`constexpr` 表示编译期常量，适合控制周期、关节数量上限等固定配置。

## 容易踩的坑

- `std::move` 不会移动任何数据，只是允许移动；移动后对象仍然有效，但内容不可假设。
- 具名的 `T&&` 变量在表达式中是左值；继续转交时需要 `std::move(x)`。
- `shared_ptr` 不是“更安全的默认值”：循环引用会泄漏，原子引用计数也有开销。
- 回调捕获 `this` 前，要确认回调不会在对象析构后运行；异步任务优先捕获 `weak_ptr` 并在执行时检查。
- 不要在实时/高频控制循环中频繁分配内存、写磁盘日志或持有互斥锁。

## 下一步练习

- [ ] 为一个轨迹缓存类补上 `unique_ptr` 所有权设计，并写清楚谁创建、谁销毁。
- [ ] 将一个手动 `lock()/unlock()` 示例改为 `std::lock_guard`。
- [ ] 写一个接受 `const std::vector<double>&` 的函数，并解释何时应改为按值接收后移动。
- [ ] 在 ROS2 节点中用 `enum class` 管理“空闲—规划—执行—失败”状态。

## 关联笔记

- [[CPP/C++生存指南]]
- [[CPP/C++面向对象]]
- [[CPP/CPP-learning/STL/STL总结]]
- [[ROS/MoveIt/OMPL源码阅读/PlannerManager]]
