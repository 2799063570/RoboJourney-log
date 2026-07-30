---
created: 2026-06-04
tags:
  - ROS2
  - robot-arm
  - C++
  - learning-plan
status: active
reviewed: 2026-07-30
---

# 机械臂 ROS2 控制项目计划

> 本文包含不同时期的学习计划、扩展建议和勾选记录。它用于拆解工作，不代表当前实际进度；继续前请以代码仓库、构建日志和演示结果重新核验。

## 一句话目标

这是一份以 4 周为起点、后来加入扩展阶段的学习与项目规划。它用于拆解工作，不是当前进度的事实记录：

> 基于 ROS2 的机械臂速度-位置控制与轨迹平滑系统

最终不是只“学会语法”，而是形成一套能解释、能运行、能展示的工程：

1. 纯 C++ 机械臂控制基础库
2. ROS2 速度转位置控制节点
3. 简单机械臂 URDF / RViz2 / TF2 可视化
4. README、运行说明、面试讲解稿

---

## 文档状态与使用方式

- 本文混合了早期的 4 周计划、后续扩展建议和当时的勾选记录。
- 所有“第几周”“今天”“当前真实状态”均为编写时的上下文，不应直接视为现在的状态。
- 继续执行前，先以代码仓库、构建日志和演示结果核验每一项；核验后再更新对应复选框或另建项目日志。
- 为避免范围失控，优先完成纯 C++ 控制库、ROS2 节点和 URDF/RViz2 三个闭环；MoveIt2、ros2_control、视觉与嵌入式均属于后续扩展。

## 规划策略（历史）

主线从 **4 周加速版** 升级为 **基础闭环 + 进阶扩展版**。

原因：

- 你已经会一些 C++ 和 ROS1。
- ROS2 功能包创建、基础节点学习进度比原计划更快。
- 当时的学习节奏较快，因此将项目从“ROS2 入门控制演示”扩展为“机器人系统综合项目”。

新的策略不是简单加快，而是分层推进：

| 阶段      | 定位        | 目标                                             |
| ------- | --------- | ---------------------------------------------- |
| 第 1-4 周 | 基础闭环      | ROS2 控制节点 + URDF + RViz2 能完整跑通                 |
| 第 5-7 周 | 机械臂进阶     | MoveIt2 + ros2_control + trajectory controller |
| 第 8 周   | 移动机器人扩展   | Nav2 定位导航基础，理解移动机器人系统                          |
| 第 9 周   | 视觉与深度学习扩展 | YOLO 经验迁移到 ROS2 感知链路，补 PyTorch / LibTorch      |
| 第 10 周  | C++ 工程强化  | 现代 C++、数据结构算法、编码风格、项目重构                        |

我们的原则是：

- 每天 2 到 3 小时
- 每天必须有一个可运行的小结果
- 每周必须有一个可展示的阶段成果
- 不追求一次写完美，先跑通，再重构，再解释清楚
- 进阶内容要服务项目展示，不做散乱学习
- 学过的知识必须能写进 README、简历和面试讲解

---

## 编写时的进度判断（历史）

以下内容反映的是本文编写时的判断，已不作为当前项目状态。实际进度应以代码和可复现实验为准：

- C++ 有一定基础，但还需要补现代 C++、工程风格、数据结构算法。
- ROS1 有基础，迁移 ROS2 时重点关注 `rclcpp`、参数、launch、QoS（Quality of Service）、生命周期和组件化。
- 后续应在最小可视化闭环稳定后，再加入 MoveIt2、ros2_control 和其他机器人系统扩展。

当前最合适的目标升级为：

> 做一个“ROS2 机械臂控制与规划综合项目”：包含自写控制节点、URDF/RViz2 可视化、MoveIt2 规划、ros2_control 控制器接口，并扩展理解移动机器人导航和视觉感知链路。

---

## 项目最终结构

```text
robot_arm_control_ros2_practice/
├── robot_control_cpp/
│   ├── CMakeLists.txt
│   ├── include/
│   │   ├── robot_state.hpp
│   │   ├── safety_limiter.hpp
│   │   ├── cubic_interpolator.hpp
│   │   ├── trajectory_buffer.hpp
│   │   └── jacobian_solver.hpp
│   ├── src/
│   │   ├── robot_state.cpp
│   │   ├── safety_limiter.cpp
│   │   ├── cubic_interpolator.cpp
│   │   ├── trajectory_buffer.cpp
│   │   └── jacobian_solver.cpp
│   └── examples/
│       ├── test_robot_state.cpp
│       ├── test_safety_limiter.cpp
│       ├── test_cubic_interpolator.cpp
│       ├── test_trajectory_buffer.cpp
│       ├── control_loop_demo.cpp
│       └── test_jacobian_solver.cpp
├── robot_control_ros2/
│   ├── src/
│   │   ├── joint_velocity_publisher.cpp
│   │   ├── joint_velocity_subscriber.cpp
│   │   ├── timer_control_node.cpp
│   │   └── vel_to_pos_node.cpp
│   ├── config/
│   │   └── vel_to_pos.yaml
│   ├── launch/
│   │   └── vel_to_pos.launch.py
│   └── CMakeLists.txt
├── simple_arm_description/
│   ├── urdf/
│   │   └── simple_2dof_arm.urdf.xacro
│   ├── launch/
│   │   └── display.launch.py
│   └── rviz/
│       └── simple_arm.rviz
├── simple_arm_moveit_config/
│   ├── config/
│   │   ├── joint_limits.yaml
│   │   ├── kinematics.yaml
│   │   ├── ompl_planning.yaml
│   │   └── ros2_controllers.yaml
│   ├── launch/
│   │   ├── demo.launch.py
│   │   └── move_group.launch.py
│   └── srdf/
│       └── simple_arm.srdf
├── simple_arm_bringup/
│   ├── launch/
│   │   ├── sim_control.launch.py
│   │   ├── moveit_control.launch.py
│   │   └── full_demo.launch.py
│   └── config/
│       └── controllers.yaml
├── robot_vision_learning/
│   ├── scripts/
│   │   ├── yolo_ros2_node.py
│   │   └── image_subscriber.py
│   └── notes/
│       ├── pytorch_review.md
│       └── libtorch_notes.md
├── mobile_robot_learning/
│   ├── notes/
│   │   ├── nav2_overview.md
│   │   ├── localization_amcl.md
│   │   └── slam_toolbox.md
│   └── launch/
│       └── nav2_demo_notes.md
└── README.md
```

---

## 第 1 周：纯 C++ 控制库

### 本周目标

完成 4 个核心模块：

- [ ] `RobotState`
- [ ] `SafetyLimiter`
- [ ] `CubicInterpolator`
- [ ] `TrajectoryBuffer`

本周重点不是 C++ 语法本身，而是：

- 模块接口怎么设计
- 类之间怎么组合
- 测试程序怎么写
- CMake 怎么组织多个目标

### Day 1：RobotState + SafetyLimiter

- [x] 创建 `robot_control_cpp/` 项目结构
- mkdir robot_control_cpp/include robot_control_cpp/src robot_control_cpp/examples
- touch CMakeLists.txt
- [x] 实现 `RobotState`
- 存储一个时刻下机器人的状态 q + dq
- [x] 实现 `SafetyLimiter`
- 存储位置上下边界vector
- 存储最大速度绝对值
- 输入一个状态判断是否越界 返回裁剪后的状态
- [x] 写 `test_robot_state.cpp`
- examples下的main.cpp 对上述的RobotState对象进行测试
- [ ] 写 `test_safety_limiter.cpp`
- examples下的main.cpp 对上述的SafetyLimiter对象进行测试

验收：

- [x] 能运行 `./test_robot_state`
- [x] 能运行 `./test_safety_limiter`
- [x] 能解释为什么状态管理和安全限幅要拆成两个类
- 保证单一职责原则

### Day 2：CubicInterpolator

- [x] 实现三次插值轨迹生成
- 三次多项式插值
- [x] 支持任意自由度
- [x] 检查 `duration > 0`
- [x] 检查 `dt > 0`
- [x] 保证最后一个轨迹点时间等于 `duration`

验收：

- [x] 打印轨迹点数量
- [x] 打印第一个点、中间点、最后一个点
- [x] 打印最大速度
- [x] 起点速度接近 0
- [x] 终点速度接近 0

### Day 3：TrajectoryBuffer + 控制循环 Demo

- [x] 实现 `TrajectoryBuffer`
- [x] 用 `std::deque<TrajectoryPoint>` 保存轨迹点
- [x] 支持 push / pop / size / clear
- [x] 空队列 pop 时抛异常
- [x] 写 `control_loop_demo.cpp`

控制循环流程：

```text
设定当前关节角
设定目标关节角
生成三次插值轨迹
推入轨迹缓存
循环取点
位置限幅
速度限幅
更新 RobotState
打印状态
```

验收：

- [ ] 能运行 `./test_trajectory_buffer`
- [ ] 能运行 `./control_loop_demo`
- [ ] 能看到机械臂状态从起点逐步变化到目标点

### Day 4：重构 CMake + 简单断言测试

今天目标不是继续堆功能，而是把前 3 天写出来的代码整理成一个更像正式 C++ 工程的结构。

前 3 天的重点是“能跑起来”：`RobotState`、`SafetyLimiter`、`CubicInterpolator`、`TrajectoryBuffer` 和 `control_loop_demo` 都能单独验证。

Day 4 的重点是“能被稳定复用和检查”：核心代码进入 library，example 只负责演示或测试，编译器 warning 打开，测试程序不只打印输出，而是用 `assert` 自动判断对错。

#### 1. 当前问题

如果 CMake 里这样写：

```cmake
add_executable(test_control
    src/cubic_interpolator.cpp
    src/robot_state.cpp
    src/safety_limiter.cpp
    src/trajectory_buffer.cpp
    examples/main.cpp
)
```

这种写法可以编译出一个程序，但有几个问题：

- `src/*.cpp` 直接塞进 executable，核心模块没有形成可复用库
- 以后每加一个 example，都要重复写一遍 `src/*.cpp`
- ROS2 节点如果想复用这些控制逻辑，也要重复链接源文件
- CMake 里看不出来哪些是“库代码”，哪些是“测试/demo 代码”
- 测试程序如果只是 `std::cout`，结果对不对还要靠肉眼看

Day 4 要把它改成：

```text
src/*.cpp
  -> 编译成 robot_control_cpp library

examples/*.cpp
  -> 每个 example 单独生成 executable
  -> 每个 executable 链接 robot_control_cpp library
```

#### 2. 推荐目录结构

```text
robot_control_cpp/
├── CMakeLists.txt
├── include/
│   ├── robot_state.hpp
│   ├── safety_limiter.hpp
│   ├── cubic_interpolator.hpp
│   └── trajectory_buffer.hpp
├── src/
│   ├── robot_state.cpp
│   ├── safety_limiter.cpp
│   ├── cubic_interpolator.cpp
│   └── trajectory_buffer.cpp
└── examples/
    ├── test_robot_state.cpp
    ├── test_safety_limiter.cpp
    ├── test_cubic_interpolator.cpp
    ├── test_trajectory_buffer.cpp
    └── control_loop_demo.cpp
```

知识点：

- `include/` 放头文件，主要写类声明和函数声明
- `src/` 放实现文件，主要写类成员函数的具体实现
- `examples/` 放测试程序和演示程序，每个文件通常有自己的 `main()`
- 核心模块不应该依赖 example，example 应该依赖核心模块

#### 3. 第一步：创建 library

在 `CMakeLists.txt` 中先写：

```cmake
add_library(robot_control_cpp
    src/cubic_interpolator.cpp
    src/robot_state.cpp
    src/safety_limiter.cpp
    src/trajectory_buffer.cpp
)
```

含义：

- `add_library` 表示创建一个库
- `robot_control_cpp` 是库的名字
- 后面的 `src/*.cpp` 是组成这个库的源文件
- 这个库本身不是最终运行的程序，因为它没有 `main()`
- 它的作用是把控制逻辑打包，供其他可执行程序复用

知识点：

```text
library
表示可复用模块。

executable
表示最终能运行的程序，必须有 main()。

link
表示把 executable 和 library 连接起来，让 main() 可以调用库里的实现。
```

#### 4. 第二步：给 library 配置头文件路径

```cmake
target_include_directories(robot_control_cpp PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
```

含义：

- `target_include_directories` 给某个 target 添加头文件搜索路径
- `robot_control_cpp` 是目标库
- `${CMAKE_CURRENT_SOURCE_DIR}/include` 表示当前 `CMakeLists.txt` 所在目录下的 `include/`
- `PUBLIC` 表示这个 include 路径既给库自己用，也给链接这个库的 executable 用

为什么这里用 `PUBLIC`：

`robot_control_cpp` 的头文件是这个库的公开接口。

例如 `examples/test_robot_state.cpp` 需要写：

```cpp
#include "robot_state.hpp"
```

它虽然是 example 文件，但它链接了 `robot_control_cpp`，所以也应该自动获得 `include/` 这个头文件路径。

知识点：

```text
PRIVATE
只有当前 target 自己能用。

PUBLIC
当前 target 自己能用，链接它的 target 也能用。

INTERFACE
当前 target 自己不用，只有链接它的 target 使用。
```

对比旧写法：

```cmake
include_directories(include)
```

这也是添加头文件路径，但它是偏全局的老式写法。小项目能用，但项目变大后不容易看清楚“这个 include 路径到底属于哪个模块”。

Day 4 推荐使用：

```cmake
target_include_directories(...)
```

因为它能把依赖关系写得更清楚。

#### 5. 第三步：打开 warning

```cmake
target_compile_options(robot_control_cpp PRIVATE
    -Wall -Wextra -Wpedantic
)
```

含义：

- `target_compile_options` 给某个 target 添加编译选项
- `-Wall` 打开常见警告
- `-Wextra` 打开更多额外警告
- `-Wpedantic` 尽量检查是否符合标准 C++ 写法
- `PRIVATE` 表示这些 warning 只作用于 `robot_control_cpp` 这个库本身

warning 可能会发现的问题：

- 定义了变量但没有使用
- 函数参数没有使用
- 类型转换可能有风险
- 返回值没有覆盖所有分支
- 写法不够标准

如果也想检查 example，可以给每个 example 也加 warning：

```cmake
target_compile_options(test_robot_state PRIVATE
    -Wall -Wextra -Wpedantic
)
```

更简单的做法是写一个小函数，避免重复：

```cmake
function(add_robot_example target source)
    add_executable(${target} ${source})
    target_link_libraries(${target} robot_control_cpp)
    target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
endfunction()
```

知识点：

- warning 不是错误，但它经常提前暴露 bug
- 学 C++ 工程时应该尽早打开 warning
- 后面可以进一步加 `-Werror`，把 warning 当成 error，但初学阶段可以先不急

#### 6. 第四步：每个 example 单独生成可执行文件

基础写法：

```cmake
add_executable(test_robot_state
    examples/test_robot_state.cpp
)

target_link_libraries(test_robot_state
    robot_control_cpp
)
```

含义：

- `add_executable` 生成一个可执行程序
- `test_robot_state` 是生成出来的程序名
- `examples/test_robot_state.cpp` 里应该有 `main()`
- `target_link_libraries` 让这个程序可以使用 `robot_control_cpp` 库里的实现

多个 example 可以这样写：

```cmake
add_executable(test_robot_state examples/test_robot_state.cpp)
target_link_libraries(test_robot_state robot_control_cpp)

add_executable(test_safety_limiter examples/test_safety_limiter.cpp)
target_link_libraries(test_safety_limiter robot_control_cpp)

add_executable(test_cubic_interpolator examples/test_cubic_interpolator.cpp)
target_link_libraries(test_cubic_interpolator robot_control_cpp)

add_executable(test_trajectory_buffer examples/test_trajectory_buffer.cpp)
target_link_libraries(test_trajectory_buffer robot_control_cpp)

add_executable(control_loop_demo examples/control_loop_demo.cpp)
target_link_libraries(control_loop_demo robot_control_cpp)
```

也可以用函数简化：

```cmake
function(add_robot_example target source)
    add_executable(${target} ${source})
    target_link_libraries(${target} robot_control_cpp)
    target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
endfunction()

add_robot_example(test_robot_state examples/test_robot_state.cpp)
add_robot_example(test_safety_limiter examples/test_safety_limiter.cpp)
add_robot_example(test_cubic_interpolator examples/test_cubic_interpolator.cpp)
add_robot_example(test_trajectory_buffer examples/test_trajectory_buffer.cpp)
add_robot_example(control_loop_demo examples/control_loop_demo.cpp)
```

知识点：

- 每个 executable 都应该只包含自己的 `main.cpp`
- 核心实现从 library 来，不要在每个 executable 里重复列 `src/*.cpp`
- 这样以后加新测试，只需要新增一个 example，再链接同一个库

#### 7. 第五步：加入 `assert`

原来的测试可能是：

```cpp
RobotState state({1.0, 2.0}, {0.1, 0.2});

std::cout << state.positions()[0] << std::endl;
std::cout << state.velocities()[1] << std::endl;
```

这种写法只能看到输出，但不能自动判断对错。

Day 4 要改成：

```cpp
#include <cassert>
#include <iostream>

#include "robot_state.hpp"

int main()
{
    RobotState state({1.0, 2.0}, {0.1, 0.2});

    assert(state.dof() == 2);
    assert(state.positions()[0] == 1.0);
    assert(state.positions()[1] == 2.0);
    assert(state.velocities()[0] == 0.1);
    assert(state.velocities()[1] == 0.2);

    std::cout << "test_robot_state passed" << std::endl;
    return 0;
}
```

含义：

- `assert(condition)` 用来检查条件是否为真
- 如果条件为真，程序继续运行
- 如果条件为假，程序直接终止并显示出错位置
- 这样测试不再只靠肉眼看输出

知识点：

```text
std::cout
适合展示过程。

assert
适合自动检查结果。
```

注意：

`assert` 是最轻量的测试方式，不等于完整单元测试框架。

后面可以学习：

- GoogleTest
- Catch2
- ROS2 `ament_cmake_gtest`

但 Day 4 先用 `assert` 足够。

#### 8. 第六步：浮点数比较使用 `near(a, b, eps)`

不要直接比较浮点数：

```cpp
assert(x == 0.3);
```

原因是浮点数计算可能出现很小误差。例如理论上应该是 `0.3`，实际可能是：

```text
0.30000000000000004
```

推荐写一个辅助函数：

```cpp
#include <cmath>

bool near(double a, double b, double eps = 1e-6)
{
    return std::abs(a - b) < eps;
}
```

然后这样测试：

```cpp
assert(near(point.positions[0], 1.0));
assert(near(point.velocities[0], 0.0));
```

在 `CubicInterpolator` 测试中尤其重要：

```cpp
auto trajectory = interpolator.generate(
    {0.0, 0.0},
    {1.0, 2.0},
    2.0,
    0.1
);

assert(!trajectory.empty());

const auto& first = trajectory.front();
const auto& last = trajectory.back();

assert(near(first.time, 0.0));
assert(near(first.positions[0], 0.0));
assert(near(first.positions[1], 0.0));

assert(near(last.time, 2.0));
assert(near(last.positions[0], 1.0));
assert(near(last.positions[1], 2.0));
assert(near(last.velocities[0], 0.0));
assert(near(last.velocities[1], 0.0));
```

知识点：

- `double` 和 `float` 是近似表示，不适合直接用 `==`
- `eps` 是允许误差
- 控制、插值、速度、轨迹时间都属于浮点数密集场景，必须养成用 `near()` 的习惯

#### 9. 推荐完整 CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.10)

project(robot_control_cpp)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(robot_control_cpp
    src/cubic_interpolator.cpp
    src/robot_state.cpp
    src/safety_limiter.cpp
    src/trajectory_buffer.cpp
)

target_include_directories(robot_control_cpp PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(robot_control_cpp PRIVATE
    -Wall -Wextra -Wpedantic
)

function(add_robot_example target source)
    add_executable(${target} ${source})
    target_link_libraries(${target} robot_control_cpp)
    target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
endfunction()

add_robot_example(test_robot_state examples/test_robot_state.cpp)
add_robot_example(test_safety_limiter examples/test_safety_limiter.cpp)
add_robot_example(test_cubic_interpolator examples/test_cubic_interpolator.cpp)
add_robot_example(test_trajectory_buffer examples/test_trajectory_buffer.cpp)
add_robot_example(control_loop_demo examples/control_loop_demo.cpp)
```

如果当前只有一个 `examples/main.cpp`，可以先写成：

```cmake
add_robot_example(test_control examples/main.cpp)
```

等后面把测试拆开后，再改成多个 example。

#### 10. ROS2 中 CMakeLists.txt 的机制

前面的 `CMakeLists.txt` 是纯 C++ 项目的写法。后面进入 ROS2 后，`CMakeLists.txt` 仍然是 CMake，但会多一层 ROS2 的构建系统机制：`ament_cmake`。

ROS2 C++ 包的典型结构是：

```text
robot_control_ros2/
├── CMakeLists.txt
├── package.xml
├── src/
│   ├── joint_velocity_publisher.cpp
│   ├── joint_velocity_subscriber.cpp
│   ├── timer_control_node.cpp
│   └── vel_to_pos_node.cpp
├── config/
│   └── vel_to_pos.yaml
└── launch/
    └── vel_to_pos.launch.py
```

纯 C++ 项目里，你通常这样编译：

```bash
cmake ..
cmake --build .
```

ROS2 项目里，你通常这样编译：

```bash
colcon build
source install/setup.bash
```

区别是：

- `cmake` 只负责当前 CMake 项目
- `colcon` 负责整个 ROS2 workspace
- `ament_cmake` 是 ROS2 C++ package 常用的 CMake 扩展
- `package.xml` 声明包的依赖关系
- `CMakeLists.txt` 负责把依赖关系用到编译、链接和安装过程中

一个最小 ROS2 C++ 包的 `CMakeLists.txt` 通常长这样：

```cmake
cmake_minimum_required(VERSION 3.8)

project(robot_control_ros2)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

add_executable(joint_velocity_publisher
  src/joint_velocity_publisher.cpp
)

ament_target_dependencies(joint_velocity_publisher
  rclcpp
  std_msgs
)

install(TARGETS
  joint_velocity_publisher
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
```

逐段理解：

```cmake
find_package(ament_cmake REQUIRED)
```

含义：

- 找到 ROS2 的 `ament_cmake` 构建系统
- 没有它，最后的 `ament_package()`、`ament_target_dependencies()` 等机制不能用
- ROS2 C++ 包通常都需要这一句

```cmake
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
```

含义：

- 找到 ROS2 依赖包
- `rclcpp` 提供 ROS2 C++ 节点 API
- `std_msgs` 提供标准消息类型
- 这些依赖也应该同时写在 `package.xml` 里

```cmake
add_executable(joint_velocity_publisher
  src/joint_velocity_publisher.cpp
)
```

含义：

- 创建一个 ROS2 节点可执行文件
- 这个 executable 之后可以通过 `ros2 run` 运行

```cmake
ament_target_dependencies(joint_velocity_publisher
  rclcpp
  std_msgs
)
```

含义：

- 给 `joint_velocity_publisher` 自动添加 ROS2 依赖的 include 路径、链接库和编译选项
- 它可以理解为 ROS2 版本的依赖绑定写法
- 在普通 CMake 中，你可能会手动写 `target_link_libraries`
- 在 ROS2 中，依赖 ROS 包时更常用 `ament_target_dependencies`

```cmake
install(TARGETS
  joint_velocity_publisher
  DESTINATION lib/${PROJECT_NAME}
)
```

含义：

- 把编译出来的 executable 安装到 ROS2 workspace 的 `install/` 目录
- 没有 install，`ros2 run robot_control_ros2 joint_velocity_publisher` 可能找不到这个程序
- `DESTINATION lib/${PROJECT_NAME}` 是 ROS2 C++ 节点常见安装位置

```cmake
ament_package()
```

含义：

- 声明这是一个 ament package
- 通常放在 `CMakeLists.txt` 最后
- 它会导出包信息，让 ROS2 workspace 能识别这个包

#### 11. 纯 C++ library 和 ROS2 node 如何结合

后面更推荐的结构不是把所有控制逻辑直接写进 ROS2 node，而是：

```text
robot_control_cpp
  -> 纯 C++ 控制库

robot_control_ros2
  -> ROS2 节点层
  -> 订阅 topic
  -> 读取 parameter
  -> 调用 robot_control_cpp
  -> 发布 topic
```

也就是说，ROS2 节点负责“通信和系统集成”，纯 C++ 库负责“控制算法和数据结构”。

如果以后把 `robot_control_cpp` 放进同一个 ROS2 package，可以写成：

```cmake
add_library(robot_control_cpp
  src/robot_state.cpp
  src/safety_limiter.cpp
  src/cubic_interpolator.cpp
  src/trajectory_buffer.cpp
)

target_include_directories(robot_control_cpp PUBLIC
  include
)

add_executable(vel_to_pos_node
  src/vel_to_pos_node.cpp
)

ament_target_dependencies(vel_to_pos_node
  rclcpp
  std_msgs
)

target_link_libraries(vel_to_pos_node
  robot_control_cpp
)

install(TARGETS
  robot_control_cpp
  vel_to_pos_node
  DESTINATION lib/${PROJECT_NAME}
)
```

这里有两种依赖关系：

```text
vel_to_pos_node 依赖 ROS2 包：
用 ament_target_dependencies

vel_to_pos_node 依赖自己写的 C++ 库：
用 target_link_libraries
```

知识点：

- `ament_target_dependencies` 主要用于 ROS2 package 依赖
- `target_link_libraries` 主要用于链接普通 C++ library
- 一个 ROS2 executable 可以同时依赖 ROS2 包和本地 C++ 库
- 控制算法不要和 ROS2 API 过度绑定，这样以后更容易测试和复用

#### 12. ROS2 中安装 launch 和 config 文件

ROS2 不只要安装可执行文件，还要安装 launch、config、urdf 等资源文件。

例如：

```cmake
install(DIRECTORY
  launch
  config
  DESTINATION share/${PROJECT_NAME}
)
```

含义：

- 把 `launch/` 和 `config/` 复制到 `install/robot_control_ros2/share/robot_control_ros2/`
- 这样 `ros2 launch` 才能找到 launch 文件
- launch 文件中也可以通过 package share 路径找到 YAML 参数

对应关系：

```text
可执行文件
  install 到 lib/${PROJECT_NAME}
  用 ros2 run 启动

launch/config/urdf/rviz 等资源
  install 到 share/${PROJECT_NAME}
  用 ros2 launch 或 launch 文件读取
```

知识点：

- `ros2 run` 找的是安装后的 executable
- `ros2 launch` 找的是安装后的 launch 文件
- 修改 launch/config 后通常需要重新 `colcon build`，或者至少确认 install 中同步了文件
- ROS2 项目不是只编译代码，还要安装资源

#### 13. package.xml 和 CMakeLists.txt 的关系

ROS2 包里通常有两个核心构建文件：

```text
package.xml
CMakeLists.txt
```

它们分工不同：

```text
package.xml
声明这个包是什么、依赖什么、用什么构建类型。

CMakeLists.txt
说明具体怎么编译、链接、安装。
```

例如 `package.xml` 中写：

```xml
<buildtool_depend>ament_cmake</buildtool_depend>
<depend>rclcpp</depend>
<depend>std_msgs</depend>
```

那么 `CMakeLists.txt` 中通常要对应写：

```cmake
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
```

如果 `package.xml` 里声明了依赖，但 `CMakeLists.txt` 没有 `find_package`，编译时可能找不到头文件或库。

如果 `CMakeLists.txt` 里用了某个依赖，但 `package.xml` 没声明，包的依赖信息就不完整，别人复现或打包时容易出问题。

今日要建立这个观念：

```text
package.xml 负责告诉 ROS2：我依赖谁。
CMakeLists.txt 负责告诉编译器：我怎么使用这些依赖。
```

#### 14. 纯 CMake 和 ROS2 CMake 对照表

| 目标 | 纯 C++ CMake | ROS2 ament_cmake |
|---|---|---|
| 创建库 | `add_library` | `add_library` |
| 创建可执行文件 | `add_executable` | `add_executable` |
| 添加头文件路径 | `target_include_directories` | `target_include_directories` |
| 链接本地库 | `target_link_libraries` | `target_link_libraries` |
| 查找普通库 | `find_package` | `find_package` |
| 查找 ROS2 依赖 | 不涉及 | `find_package(rclcpp REQUIRED)` |
| 绑定 ROS2 依赖 | 不涉及 | `ament_target_dependencies` |
| 安装可执行文件 | 可选 | 必须，影响 `ros2 run` |
| 安装 launch/config | 不涉及 | 常用，影响 `ros2 launch` |
| 声明包结束 | 不涉及 | `ament_package()` |
| 构建命令 | `cmake .. && cmake --build .` | `colcon build` |

#### 15. Day 4 需要额外理解的 ROS2 CMake 问题

- [ ] `ament_cmake` 和普通 CMake 是什么关系
	CMake 是基础构建工具，ament_cmake 是 ROS2 基于 CMake 做的扩展 ，colcon 是工作空间级别的构建工具
	调用每个包的 CMake，CMake 中使用 ament_cmake 的 ROS2 扩展功能
- [ ] 为什么 ROS2 包既有 `package.xml` 又有 `CMakeLists.txt`
	package声明需要哪些包描述包的**元信息和依赖关系**，cmakelist描述这个包**具体怎么编译**
- [ ] `find_package(rclcpp REQUIRED)` 做了什么
	查找包的位置
- [ ] `ament_target_dependencies` 和 `target_link_libraries` 有什么区别
	链接自己写的 C++ library：target_link_libraries()  
	链接 ROS2/ament 包依赖：ament_target_dependencies()
- [ ] 为什么 ROS2 节点需要 `install(TARGETS ...)`
		这表示把编译出来的可执行文件安装到
- [ ] 为什么 launch/config 需要 `install(DIRECTORY ...)`
- [ ] 为什么 `ros2 run` 找的是 install 目录里的 executable
	因为 ROS2 使用的是 **安装后的包索引系统**，不是直接扫描源码目录或 build 目录
- [ ] 为什么纯 C++ 控制库最好不要直接依赖 ROS2 API
	第一，复用性变差；第二，测试变麻烦；第三，模块边界不清晰

#### 16. 编译和运行流程

建议不要在源码目录里直接生成编译文件，而是创建 `build/`：

```bash
cd robot_control_cpp
mkdir -p build
cd build
cmake ..
cmake --build .
```

运行：

```bash
./test_robot_state
./test_safety_limiter
./test_cubic_interpolator
./test_trajectory_buffer
./control_loop_demo
```

Windows 下可能是：

```powershell
.\test_robot_state.exe
.\test_safety_limiter.exe
.\test_cubic_interpolator.exe
.\test_trajectory_buffer.exe
.\control_loop_demo.exe
```

知识点：

- `cmake ..` 是配置阶段，生成构建系统
- `cmake --build .` 是编译阶段，真正调用编译器
- `build/` 目录可以删除重建，不应该把它当作源码的一部分
- 如果 CMakeLists.txt 改了，通常需要重新运行 `cmake ..`

ROS2 包的编译运行流程是：

```bash
cd ros2_ws
colcon build
source install/setup.bash
ros2 run robot_control_ros2 joint_velocity_publisher
```

如果运行 launch 文件：

```bash
ros2 launch robot_control_ros2 vel_to_pos.launch.py
```

知识点：

- 纯 C++ 项目用 `cmake --build`
- ROS2 workspace 用 `colcon build`
- `source install/setup.bash` 之后，当前终端才能找到新编译的 ROS2 包

#### 17. 今日任务清单

- [ ] 把 `src/*.cpp` 编译进 `robot_control_cpp` library
- [ ] 用 `target_include_directories` 配置 `include/`
- [ ] 打开 warning：`-Wall -Wextra -Wpedantic`
- [ ] 把 `examples/main.cpp` 拆成多个 example，或先保留一个 `test_control`
- [ ] 每个 example 单独生成可执行文件
- [ ] 每个 example 使用 `target_link_libraries(... robot_control_cpp)`
- [ ] 给至少 2 个测试程序加入 `assert`
- [ ] 写一个 `near(a, b, eps)` 用于浮点数比较
- [ ] 编译所有 examples
- [ ] 运行所有 examples
- [ ] 能读懂一个最小 ROS2 `ament_cmake` 的 `CMakeLists.txt`
- [ ] 能说明 `package.xml` 和 `CMakeLists.txt` 的对应关系
- [ ] 能说明 ROS2 中为什么需要 `install(TARGETS ...)`
	ROS2 运行时主要从 `install` 空间查找可执行文件和库，而不是直接从 `build` 目录或 `src` 目录查找

#### 18. 今日必须理解的问题

- [ ] `add_library` 和 `add_executable` 有什么区别
- [ ] 为什么核心模块应该先编译成 library
- [ ] 为什么 example 不应该重复列出所有 `src/*.cpp`
- [ ] `target_link_libraries` 解决的是什么问题
- [ ] `target_include_directories` 和 `include_directories` 的区别
- [ ] `PUBLIC`、`PRIVATE`、`INTERFACE` 分别是什么意思
- [ ] 为什么要打开 warning
- [ ] 为什么浮点数不能直接用 == 比较
- [ ] `assert` 和 `std::cout` 分别适合做什么
- [ ] `ament_package()` 为什么通常放在 ROS2 `CMakeLists.txt` 最后
- [ ] `ament_target_dependencies` 解决了哪些 ROS2 依赖问题

验收：

- [ ] 所有 examples 都能编译
- [ ] 所有 examples 都能运行
- [ ] 不只靠肉眼看输出，至少有一部分自动检查
- [ ] 能解释当前 CMakeLists.txt 中每一行的作用
- [ ] 能解释从 `src/*.cpp` 到最终可执行文件的编译链接流程
- [ ] 能解释一个最小 ROS2 C++ package 的 `CMakeLists.txt`
- [ ] 能说清 `ros2 run` 为什么依赖 `install(TARGETS ...)`

### Day 5：第一版 README

- [ ] 说明这个库解决什么问题
- [ ] 说明每个模块负责什么
- [ ] 写清楚如何编译
- [ ] 写清楚如何运行测试
- [ ] 写清楚控制循环 demo 的流程

验收：

- [ ] 别人只看 README，也能知道怎么编译和运行纯 C++ 控制库

---

---


### 第 1 周每日详细执行指南：纯 C++ 控制库

#### Day 1：RobotState + SafetyLimiter

实现过程：

- 创建 `robot_control_cpp/include`、`robot_control_cpp/src`、`robot_control_cpp/examples`
- 在 `robot_state.hpp/.cpp` 中实现 `RobotState`，保存关节位置 `q` 和关节速度 `dq`
- 在构造函数中检查 `positions` 和 `velocities` 维度是否一致
- 提供 `positions()`、`velocities()`、`dof()` 等查询接口
- 在 `safety_limiter.hpp/.cpp` 中实现 `SafetyLimiter`
- 保存关节位置上下限和最大速度
- 实现位置裁剪和速度裁剪
- 写 `examples/test_robot_state.cpp` 和 `examples/test_safety_limiter.cpp`

知识点：

- `std::vector<double>` 适合保存多关节数组
- 类的职责要单一，`RobotState` 管状态，`SafetyLimiter` 管安全限制
- 构造函数适合做维度合法性检查
- 查询函数应该尽量加 `const`
- 输入参数如果是大对象，优先用 `const std::vector<double>&`

今日产出：

- 能创建一个关节状态对象
- 能读取关节数量、位置和速度
- 能对越界位置和速度进行限制
- 能解释为什么状态管理和安全限制要拆成两个类

#### Day 2：CubicInterpolator

实现过程：

- 创建 `cubic_interpolator.hpp/.cpp`
- 设计 `TrajectoryPoint`，包含 `time`、`positions`、`velocities`
- 输入起点、终点、总时长 `duration`、采样间隔 `dt`
- 检查 `duration > 0`、`dt > 0`
- 检查起点和终点维度一致
- 用三次多项式生成轨迹点
- 保证第一个点时间为 `0`
- 保证最后一个点时间为 `duration`
- 写 `examples/test_cubic_interpolator.cpp`

知识点：

- 三次插值用于生成起止速度为 0 的平滑轨迹
- 轨迹点不只是位置，还应该包含时间和速度
- 控制里 `dt` 是离散采样周期
- 浮点数循环容易产生末尾误差，所以最后一个点要专门处理
- 插值模块应该只负责生成轨迹，不负责安全限幅和执行

今日产出：

- 能生成从起点到终点的多关节轨迹
- 能打印轨迹点数量、首点、中点、末点
- 能解释为什么起止速度接近 0

#### Day 3：TrajectoryBuffer + 控制循环 Demo

实现过程：

- 创建 `trajectory_buffer.hpp/.cpp`
- 用 `std::deque<TrajectoryPoint>` 保存待执行轨迹
- 实现 `push`、`pop`、`size`、`empty`、`clear`
- 空队列 `pop` 时抛出异常
- 写 `control_loop_demo.cpp`
- demo 流程是：生成轨迹 -> 推入 buffer -> 循环取点 -> 安全限幅 -> 更新 `RobotState` -> 打印状态

知识点：

- `std::deque` 适合从头部弹出、从尾部插入
- 轨迹执行本质是按时间顺序消费一串轨迹点
- 控制循环中通常不是一次性跳到目标，而是每个周期更新一点
- `throw std::runtime_error` 适合表达非法运行状态
- demo 的职责是串联模块，不应该把所有逻辑都写死在一个类里

今日产出：

- 能运行 `test_trajectory_buffer`
- 能运行 `control_loop_demo`
- 能看到状态从起点逐步变化到目标点
- 能解释为什么需要 trajectory buffer

#### Day 4：重构 CMake + 简单断言测试

实现过程：

- 把 `src/*.cpp` 编译进 `robot_control_cpp` library
- 用 `target_include_directories` 配置公开头文件路径
- 用 `target_compile_options` 打开 `-Wall -Wextra -Wpedantic`
- 每个 `examples/*.cpp` 单独生成 executable
- 每个 executable 使用 `target_link_libraries(... robot_control_cpp)`
- 给测试程序加入 `assert`
- 写 `near(a, b, eps)` 做浮点数比较
- 对照阅读一个 ROS2 `ament_cmake` 的 `CMakeLists.txt`
- 理解 `find_package`、`ament_target_dependencies`、`install`、`ament_package`
- 建立“纯 C++ 库负责算法，ROS2 node 负责通信”的工程分层

知识点：

- `add_library` 创建可复用库
- `add_executable` 创建可运行程序
- `target_link_libraries` 连接可执行文件和库
- `PUBLIC` 表示当前 target 和依赖它的 target 都能继承该属性
- `assert` 是最轻量的自动检查
- 浮点数比较要用误差范围，不要直接用 `==`
- `ament_cmake` 是 ROS2 C++ 包常用的 CMake 构建扩展
- `ament_target_dependencies` 用来绑定 ROS2 package 依赖
- `install(TARGETS ...)` 决定 `ros2 run` 能否找到节点可执行文件
- `install(DIRECTORY ...)` 决定 launch、config、urdf 等资源能否被 ROS2 找到

今日产出：

- 所有 examples 都能编译
- 所有 examples 都能运行
- 至少两个测试程序有自动断言
- 能解释当前 `CMakeLists.txt` 每一行的作用
- 能解释一个最小 ROS2 C++ package 的 `CMakeLists.txt`
- 能说清 `package.xml` 和 `CMakeLists.txt` 的分工

#### Day 5：第一版 README

实现过程：

- 在项目根目录创建或更新 `README.md`
- 写清楚项目目标：纯 C++ 机械臂控制基础库
- 按模块解释 `RobotState`、`SafetyLimiter`、`CubicInterpolator`、`TrajectoryBuffer`
- 写清楚目录结构
- 写清楚编译命令
- 写清楚每个 example 的运行命令和预期结果
- 写一段控制循环 demo 的流程说明

知识点：

- README 是项目的入口，不是最后随便补的说明
- 好 README 应该让别人能复现你的结果
- 模块说明要写“解决什么问题”，不要只写“这个文件存在”
- 运行命令要和实际项目结构一致
- 面试时 README 可以帮助你组织讲解顺序

今日产出：

- 别人只看 README 也能知道怎么编译和运行
- README 中有项目结构、模块职责、运行命令和 demo 流程
- 能用 1 分钟讲清楚第 1 周纯 C++ 控制库做了什么

---


## 第 2 周：Eigen、雅可比求解与项目化

### 本周目标

把纯 C++ 控制库从“轨迹和状态管理”推进到“简单运动学计算”。

- [ ] 引入 Eigen
- [ ] 实现 `JacobianSolver`
- [ ] 完成一个 2 自由度平面机械臂的雅可比速度映射
- [ ] 整理控制库接口和 README

### 核心任务

- [ ] 安装或配置 Eigen
- [ ] 写 `jacobian_solver.hpp`
- [ ] 写 `jacobian_solver.cpp`
- [ ] 写 `test_jacobian_solver.cpp`
- [ ] 输入关节速度，计算末端速度
- [ ] 输入期望末端速度，尝试求解关节速度

验收：

- [ ] 能解释雅可比矩阵在速度控制中的作用
- [ ] 能解释奇异位形为什么危险
- [ ] 能运行所有 C++ examples
- [ ] README 能说明控制库整体架构

---

---


### 第 2 周每日详细执行指南：Eigen、雅可比求解与项目化

#### Day 1：安装或配置 Eigen

实现过程：

- 确认系统是否已经安装 Eigen
- 在 CMake 中查找 Eigen，优先使用 `find_package(Eigen3 REQUIRED)`
- 如果没有系统 Eigen，记录安装方式或临时 include 路径
- 写一个最小 Eigen demo，创建 `Vector2d`、`Matrix2d`
- 编译并运行 demo，确认 Eigen 可用

知识点：

- Eigen 是 header-only 数值计算库
- 向量和矩阵是机器人运动学的基础数据结构
- `Eigen::VectorXd` 适合动态维度
- `Eigen::MatrixXd` 适合动态矩阵
- CMake 依赖应该显式写清楚，不能只靠 IDE 自动找到

今日产出：

- Eigen 可以被 CMake 正确找到
- 能编译一个最小 Eigen 程序
- 能解释为什么后续雅可比计算需要矩阵库

#### Day 2：设计 JacobianSolver 接口

实现过程：

- 创建 `jacobian_solver.hpp`
- 明确目标：先做 2 自由度平面机械臂
- 输入关节角 `q1`、`q2` 和连杆长度 `l1`、`l2`
- 输出末端速度雅可比矩阵 `J`
- 设计函数，例如 `compute_jacobian(q)`、`forward_velocity(q, dq)`
- 先只写声明和注释，不急着写复杂实现

知识点：

- 雅可比矩阵描述关节速度到末端速度的线性映射
- 对 2D 平面机械臂，末端速度可以写成 `[vx, vy]`
- 关节速度可以写成 `[dq1, dq2]`
- 核心关系是 `v = J(q) * dq`
- 接口设计要先明确输入、输出和单位

今日产出：

- 有清晰的 `JacobianSolver` 头文件
- 能解释每个函数负责什么
- 能画出 2 自由度平面机械臂的关节和连杆

#### Day 3：实现正向速度映射

实现过程：

- 在 `jacobian_solver.cpp` 中实现雅可比矩阵计算
- 根据 2 连杆平面机械臂公式写出 `J`
- 输入 `q` 和 `dq`
- 计算 `end_effector_velocity = J * dq`
- 写 `test_jacobian_solver.cpp`
- 用简单角度测试输出是否符合直觉

知识点：

- 正运动学是从关节角求末端位置
- 雅可比是正运动学对关节角的局部导数
- 速度映射比位置映射更适合理解控制
- 机器人控制中经常把关节空间速度映射到笛卡尔空间速度

今日产出：

- 能输入关节速度并输出末端速度
- 能运行 `test_jacobian_solver`
- 能解释 `J * dq` 的物理意义

#### Day 4：尝试逆向速度求解

实现过程：

- 输入期望末端速度 `v_desired`
- 尝试求解关节速度 `dq`
- 对方阵情况使用 `J.inverse() * v`
- 对接近奇异的情况记录问题
- 如果矩阵不可逆，先打印提示，不要求完全解决
- 写测试用例观察不同姿态下的结果

知识点：

- 逆速度求解是从末端速度反求关节速度
- 奇异位形会导致某些方向速度无法实现
- 矩阵求逆不是万能的，实际工程中常用伪逆和阻尼最小二乘
- 控制里要关心数值稳定性

今日产出：

- 能尝试从末端速度求关节速度
- 能观察某些姿态下求解不稳定
- 能解释“奇异位形为什么危险”

#### Day 5：把 JacobianSolver 接入 CMake 和 examples

实现过程：

- 把 `src/jacobian_solver.cpp` 加入 `robot_control_cpp` library
- 把 `include/jacobian_solver.hpp` 加入 include 目录
- 新增 `test_jacobian_solver` executable
- 链接 `robot_control_cpp` 和 Eigen
- 打开 warning 后修复编译提示

知识点：

- 新模块加入项目时要同时改源码、头文件、CMake、测试
- CMake 中第三方库应该用 target 方式链接
- 数值模块也需要测试，不应该只看打印结果
- 工程化的重点是“新增模块有固定流程”

今日产出：

- 所有 C++ examples 都能编译
- `test_jacobian_solver` 能运行
- README 或笔记中补充 JacobianSolver 的作用

#### Day 6：整理控制库接口和异常处理

实现过程：

- 检查所有类的 public 接口是否过多
- 检查所有维度不一致的情况是否有明确处理
- 给非法输入加 `throw std::invalid_argument`
- 给非法运行状态加 `throw std::runtime_error`
- 检查函数命名是否统一
- 检查 `const` 使用是否合理

知识点：

- 接口越小，模块越容易维护
- 参数错误和运行状态错误可以用不同异常表达
- `const correctness` 能帮助编译器检查误修改
- 控制库的输入检查很重要，不能默认调用者永远正确

今日产出：

- 代码接口更统一
- 常见错误输入有明确异常
- 能解释每个模块的输入合法性要求

#### Day 7：第 2 周 README 和复盘

实现过程：

- 在 README 中新增“运动学与雅可比”章节
- 写清楚 2 自由度平面机械臂假设
- 写清楚 `v = J(q) * dq`
- 整理本周新增文件
- 记录 Eigen 的 CMake 配置方式
- 写一段面试讲解稿

知识点：

- 复盘不是流水账，要提炼项目能力
- 机器人项目中，数学公式要能对应到代码函数
- README 中的公式、代码和运行命令要互相对应
- 面试讲解要从问题、方法、实现、验证四步讲

今日产出：

- README 能说明控制库整体架构
- 能解释雅可比矩阵在速度控制中的作用
- 能运行所有 C++ examples

---


## 第 3 周：ROS2 入门迁移 + vel_to_pos_node

### 本周定位

你现在会 ROS1，也已经知道 ROS2 功能包怎么创建，所以第 3 周不要直接硬写完整控制节点。先把 ROS2 的基本套路吃透，再把纯 C++ 控制逻辑迁移进去。

本周的真实目标是：

- [ ] 能说清楚 ROS1 和 ROS2 在包、节点、构建、参数、launch 上的差异
- [ ] 能独立创建并编译一个 ROS2 C++ package
- [ ] 能写最小 publisher / subscriber / timer
- [ ] 能用 parameter 和 launch 管理节点
- [ ] 完成 `vel_to_pos_node.cpp` 的第一版

### ROS1 到 ROS2 对照表

先把这张表背熟，后面写代码会顺很多：

| ROS1                             | ROS2                                 |
| -------------------------------- | ------------------------------------ |
| `catkin_make` / `catkin build`   | `colcon build`                       |
| `catkin_ws/src`                  | `ros2_ws/src`                        |
| `package.xml` + `CMakeLists.txt` | 仍然是 `package.xml` + `CMakeLists.txt` |
| `roscore`                        | 通常不需要单独启动 master                     |
| `rosrun pkg node`                | `ros2 run pkg node`                  |
| `roslaunch pkg file.launch`      | `ros2 launch pkg file.launch.py`     |
| `rosnode list`                   | `ros2 node list`                     |
| `rostopic list`                  | `ros2 topic list`                    |
| `rostopic echo`                  | `ros2 topic echo`                    |
| `rostopic pub`                   | `ros2 topic pub`                     |
| `rosparam`                       | `ros2 param`                         |
| `ros::NodeHandle`                | `rclcpp::Node`                       |
| `ros::Publisher`                 | `rclcpp::Publisher<T>::SharedPtr`    |
| `ros::Subscriber`                | `rclcpp::Subscription<T>::SharedPtr` |
| `ros::Timer`                     | `rclcpp::TimerBase::SharedPtr`       |

---

### Day 1：确认 ROS2 工作区和包结构

目标：只做一件事，把 ROS2 工程结构弄清楚。

- [ ] 创建工作区：`ros2_ws/src`
- [ ] 创建包：`robot_control_ros2`
- [ ] 依赖先只加 `rclcpp` 和 `std_msgs`
- [ ] 看懂 `package.xml`
- [ ] 看懂 `CMakeLists.txt`
- [ ] 用 `colcon build` 编译
- [ ] source 环境：`source install/setup.bash`

建议命令：

```bash
mkdir -p ~/ros2_ws/src
cd ~/ros2_ws/src
ros2 pkg create robot_control_ros2 --build-type ament_cmake --dependencies rclcpp std_msgs
cd ~/ros2_ws
colcon build
source install/setup.bash
ros2 pkg list | grep robot_control_ros2
```

今天必须理解：

- [x] `src/` 下面放 ROS2 package
- [x] `colcon build` 是在工作区根目录运行
- [x] `install/setup.bash` 的作用是把新包加入当前终端环境
- [x] 每开一个新终端都要重新 source

验收：

- [ ] `ros2 pkg list` 能看到 `robot_control_ros2`
- [ ] 能说清楚 package、node、executable 不是同一个东西

---

### Day 2：最小 Publisher 节点

目标：写一个节点定时发布关节速度命令。

文件：

```text
robot_control_ros2/src/joint_velocity_publisher.cpp
```

节点功能：

- 节点名：`joint_velocity_publisher`
- topic：`/joint_velocity_cmd`
- 消息类型：先用 `std_msgs/msg/Float64MultiArray`
- 每 100 ms 发布一次 6 维速度

核心概念：

- [ ] `rclcpp::init`
- [ ] `std::make_shared<NodeClass>()`
- [ ] `rclcpp::spin`
- [ ] `rclcpp::shutdown`
- [ ] `create_publisher`
- [ ] `create_wall_timer`

验收命令：

```bash
ros2 run robot_control_ros2 joint_velocity_publisher
ros2 topic list
ros2 topic echo /joint_velocity_cmd
ros2 topic hz /joint_velocity_cmd
```

验收：

- [ ] 能看到 `/joint_velocity_cmd`
- [ ] `echo` 能看到数组数据
- [ ] `hz` 接近 10 Hz
- [ ] 能解释 timer callback 为什么会周期执行

---

### Day 3：最小 Subscriber 节点

目标：写一个订阅节点，能收到 Day 2 发布的速度命令。

文件：

```text
robot_control_ros2/src/joint_velocity_subscriber.cpp
```

节点功能：

- 节点名：`joint_velocity_subscriber`
- 订阅：`/joint_velocity_cmd`
- 收到后打印 6 个关节速度

核心概念：

- [ ] `create_subscription`
- [ ] callback 函数参数
- [ ] `SharedPtr`
- [ ] QoS 队列深度
- [ ] `RCLCPP_INFO`

验收命令：

```bash
ros2 run robot_control_ros2 joint_velocity_subscriber
ros2 run robot_control_ros2 joint_velocity_publisher
```

也可以手动发布：

```bash
ros2 topic pub /joint_velocity_cmd std_msgs/msg/Float64MultiArray "{data: [0.1, 0.0, 0.0, 0.0, 0.0, 0.0]}"
```

验收：

- [ ] subscriber 能打印收到的数据
- [ ] 能解释 publisher 和 subscriber 不需要互相知道对方存在
- [ ] 能解释 topic 名字和消息类型必须匹配

---

### Day 4：Timer 控制循环节点

目标：理解 ROS2 控制节点通常不是“收到一次就算一次”，而是有一个固定频率控制循环。

文件：

```text
robot_control_ros2/src/timer_control_node.cpp
```

节点功能：

- 节点名：`timer_control_node`
- 内部保存当前关节位置 `q`
- 每 10 ms 执行一次 timer callback
- 每次让 `q[0] += 0.001`
- 发布 `/joint_position_cmd`

核心概念：

- [ ] 控制频率
- [ ] `dt`
- [ ] 节点内部状态变量
- [ ] timer callback 和 subscriber callback 的区别

验收命令：

```bash
ros2 run robot_control_ros2 timer_control_node
ros2 topic echo /joint_position_cmd
ros2 topic hz /joint_position_cmd
```

验收：

- [ ] 能看到位置连续变化
- [ ] 频率接近 100 Hz
- [ ] 能解释为什么控制循环通常用 timer，而不是只依赖 subscriber callback

---

### Day 5：参数和 YAML 配置

目标：把硬编码的控制参数改成 ROS2 parameter。
这里需要借助于参数服务器，需要注意的ROS2不同于ROS1，参数隶属于每个节点

我们可以借助于参数输入
launch 启动节点时读取 YAML 文件，把里面匹配当前节点名的参数作为初始覆盖值传给节点；节点内部通过 `declare_parameter()` 声明参数时，会优先使用这些 YAML 中的值。
```python
Node(
    package="robot_control_ros2",
    executable="vel_to_pos_node",
    name="vel_to_pos_node",
    parameters=[
        config_dir,# 文件路径
        {
            "dt": 0.01,
            "control_rate": 100.0
        }# 参数字典
    ]
)
```

新增文件：

```text
robot_control_ros2/config/vel_to_pos.yaml
```

建议参数：

```yaml
vel_to_pos_node:
  ros__parameters:
    dof: 6
    control_rate: 100.0
    max_velocity: 0.5
    lower_limits: [-3.14, -1.57, -3.14, -3.14, -2.0, -6.28]
    upper_limits: [3.14, 1.57, 3.14, 3.14, 2.0, 6.28]
```
这里需要注意的节点名称要对应

今天任务：

- [x] 学会 `declare_parameter`
- [x] 学会 `get_parameter`
	对于参数服务器需要先声明后set get
- [ ] 学会 `ros2 param list`
- [ ] 学会 `ros2 param get`
- [ ] 学会从 YAML 启动节点

验收命令：

```bash
ros2 run robot_control_ros2 timer_control_node --ros-args --params-file src/robot_control_ros2/config/vel_to_pos.yaml
ros2 param list
ros2 param get /timer_control_node control_rate
```

验收：

- [ ] 节点能读取 YAML 参数
- [ ] 能用命令行查看参数
- [ ] 能解释为什么参数不应该写死在代码里

---

### Day 6：完成 vel_to_pos_node 第一版

目标：把前几天的 publisher、subscriber、timer、参数合成一个真正的控制节点。

文件：

```text
robot_control_ros2/src/vel_to_pos_node.cpp
```

输入：

- `/joint_velocity_cmd`

输出：

- `/joint_position_cmd`

内部状态：

- `q_current`
- `dq_cmd`
- `lower_limits`
- `upper_limits`
- `max_velocity`
- `dt`

核心逻辑：

```text
subscriber callback:
  保存最新速度命令 dq_cmd

timer callback:
  dq_cmd 限幅
  q_current = q_current + dq_cmd * dt
  q_current 限幅
  发布 q_current
```

今天先不追求完美：

- [ ] 可以先用 `std::vector<double>` 写内部逻辑
- [ ] 可以先不用接第 1、2 周的纯 C++ library
- [ ] 先保证 ROS2 通信链路跑通
- [ ] 跑通后再考虑把 `SafetyLimiter` 接回来

验收命令：

```bash
ros2 run robot_control_ros2 vel_to_pos_node
ros2 topic pub /joint_velocity_cmd std_msgs/msg/Float64MultiArray "{data: [0.1, 0.0, 0.0, 0.0, 0.0, 0.0]}"
ros2 topic echo /joint_position_cmd
```

验收：

- [ ] 输入正速度时，位置逐渐增加
- [ ] 输入 0 速度时，位置保持
- [ ] 输入超过 `max_velocity` 的速度时，会被限制
- [ ] 超过位置上下限时，会被限制

---

### Day 7：Launch 文件和本周复盘

目标：用 launch 一次启动节点，整理第 3 周 README。

新增文件：

```text
robot_control_ros2/launch/vel_to_pos.launch.py
```

launch 目标：

- [ ] 启动 `vel_to_pos_node`
- [ ] 加载 `vel_to_pos.yaml`
- [ ] 后续可以选择同时启动测试 publisher

验收命令：

```bash
ros2 launch robot_control_ros2 vel_to_pos.launch.py
ros2 topic pub /joint_velocity_cmd std_msgs/msg/Float64MultiArray "{data: [0.1, 0.0, 0.0, 0.0, 0.0, 0.0]}"
ros2 topic echo /joint_position_cmd
```

本周复盘要写进 README：

- [ ] 如何创建包
- [ ] 如何编译
- [ ] 如何 source
- [ ] 如何启动每个节点
- [ ] topic 输入输出是什么
- [ ] 参数文件怎么改
- [ ] `vel_to_pos_node` 的控制逻辑图

第 3 周总验收：

- [ ] 能独立写出一个 ROS2 C++ publisher
- [ ] 能独立写出一个 ROS2 C++ subscriber
- [ ] 能独立写出一个 timer 控制循环
- [ ] 能用 YAML 参数配置节点
- [ ] 能用 launch 启动节点
- [ ] 能解释 `vel_to_pos_node` 的数据流

---

---


### 第 3 周每日详细执行指南：ROS2 入门迁移 + vel_to_pos_node

#### Day 1：确认 ROS2 工作区和包结构

实现过程：

- 创建 `ros2_ws/src`
- 使用 `ros2 pkg create` 创建 `robot_control_ros2`
- 依赖先只加入 `rclcpp` 和 `std_msgs`
- 在工作区根目录运行 `colcon build`
- source `install/setup.bash`
- 用 `ros2 pkg list` 确认包存在

知识点：

- ROS2 工作区通常由 `src`、`build`、`install`、`log` 组成
- package 是功能包，不等于 node
- executable 是包里可以运行的程序
- `colcon build` 要在工作区根目录运行
- `source install/setup.bash` 会把新包加入当前终端环境

今日产出：

- 能成功创建并编译 ROS2 C++ package
- 能解释 package、node、executable 的区别
- 能说清 ROS1 `catkin_make` 和 ROS2 `colcon build` 的差异

#### Day 2：最小 Publisher 节点

实现过程：

- 创建 `joint_velocity_publisher.cpp`
- 继承 `rclcpp::Node`
- 创建 publisher，topic 为 `/joint_velocity_cmd`
- 消息类型使用 `std_msgs::msg::Float64MultiArray`
- 创建 100 ms timer
- 每次 timer callback 发布 6 维速度数组
- 在 CMake 中添加 executable 和 install 规则

知识点：

- publisher 负责向 topic 发布消息
- timer callback 适合周期性任务
- ROS2 C++ 节点通常用 `std::make_shared`
- topic 名称和消息类型必须和订阅端匹配
- `ros2 topic echo` 用来查看消息内容

今日产出：

- `ros2 run robot_control_ros2 joint_velocity_publisher` 能运行
- `/joint_velocity_cmd` 能被 `ros2 topic echo` 看到
- `ros2 topic hz` 接近 10 Hz

#### Day 3：最小 Subscriber 节点

实现过程：

- 创建 `joint_velocity_subscriber.cpp`
- 创建 subscription，订阅 `/joint_velocity_cmd`
- callback 接收 `Float64MultiArray`
- 打印收到的 6 维速度
- 在 CMake 中添加 executable 和 install 规则
- 同时运行 publisher 和 subscriber 观察通信

知识点：

- subscriber 不需要知道 publisher 的对象，只需要 topic 和消息类型一致
- callback 是收到消息后被 ROS2 executor 调用的函数
- QoS 队列深度影响消息缓存
- `SharedPtr` 是 ROS2 消息回调中常见写法

今日产出：

- subscriber 能收到 publisher 发出的速度命令
- 能用命令行手动发布 topic 进行测试
- 能解释 publisher 和 subscriber 的解耦关系

#### Day 4：Timer 控制循环节点

实现过程：

- 创建 `timer_control_node.cpp`
- 节点内部保存当前关节位置 `q`
- 创建 10 ms timer
- 每个周期更新 `q[0] += 0.001`
- 发布 `/joint_position_cmd`
- 用 `ros2 topic echo` 和 `ros2 topic hz` 验证

知识点：

- 控制节点通常依赖固定频率循环，而不是只在收到消息时计算
- `dt` 是每个控制周期的时间间隔
- timer callback 适合实现控制循环
- subscriber callback 适合更新输入命令
- 控制节点内部需要保存状态

今日产出：

- 能看到位置命令连续变化
- 发布频率接近 100 Hz
- 能解释为什么控制循环通常用 timer

#### Day 5：参数和 YAML 配置

实现过程：

- 创建 `config/vel_to_pos.yaml`
- 写入 `dof`、`control_rate`、`max_velocity`、`lower_limits`、`upper_limits`
- 在节点中使用 `declare_parameter`
- 使用 `get_parameter` 读取参数
- 用 `--ros-args --params-file` 加载 YAML
- 用 `ros2 param list/get` 查看参数

知识点：

- 参数用于把可调配置从代码中移出
- YAML 文件适合保存控制参数
- ROS2 参数需要先声明再读取
- 参数名、节点名和 YAML 层级必须匹配
- 不同机器人可以复用同一份代码，只替换参数文件

今日产出：

- 节点能从 YAML 读取参数
- 能用命令行查看参数
- 能解释为什么控制参数不应该硬编码

#### Day 6：完成 vel_to_pos_node 第一版

实现过程：

- 创建 `vel_to_pos_node.cpp`
- 订阅 `/joint_velocity_cmd`
- 保存最新速度命令 `dq_cmd`
- timer 中做速度限幅
- 用 `q_current = q_current + dq_cmd * dt` 积分位置
- 做位置限幅
- 发布 `/joint_position_cmd`
- 加入参数读取

知识点：

- 速度到位置的转换本质是离散积分
- subscriber callback 更新输入，timer callback 执行控制
- 安全限幅要同时考虑速度和位置
- 控制状态应该保存在节点内部
- 频率越高，积分越平滑，但计算压力也更高

今日产出：

- 输入速度命令后，输出位置命令会持续变化
- 超过限制时会被裁剪
- 能解释速度控制和位置命令之间的关系

#### Day 7：Launch 文件和本周复盘

实现过程：

- 创建 `launch/vel_to_pos.launch.py`
- 在 launch 中启动 `vel_to_pos_node`
- 加载 YAML 参数文件
- 可选：同时启动测试 publisher
- 用 `ros2 launch` 一键启动
- 记录本周 ROS2 命令和排错方式

知识点：

- launch 用于组织多个节点和配置
- `ros2 run` 适合单节点调试
- `ros2 launch` 适合系统启动
- 参数文件路径要通过 package share 目录定位
- 复盘时要整理 topic、parameter、launch 三条线

今日产出：

- 一条 launch 命令能启动控制节点
- README 中有 ROS2 节点运行方法
- 能讲清楚 `vel_to_pos_node` 的输入、输出和内部状态

---


## 第 4 周：URDF / RViz2 / robot_state_publisher + 项目整理

### 本周定位

第 4 周不要一上来就追求复杂机械臂。先做一个最小 2 自由度机械臂，只要能在 RViz2 里看到模型，并且能通过 joint state 改变姿态，就已经完成核心目标。

本周目标：

- [ ] 理解 URDF 描述的是机器人结构
- [ ] 理解 `joint_states` 描述的是关节当前状态
- [ ] 理解 `robot_state_publisher` 根据 URDF 和 joint state 发布 TF
- [ ] RViz2 中能看到机械臂
- [ ] 把第 3 周的 `/joint_position_cmd` 转成 `/joint_states`
- [ ] 整理 README、简历描述、面试讲解稿

---

### Day 1：先理解可视化链路

今天先不要写太多代码，先把链路搞明白：

```text
URDF / xacro
  描述 link 和 joint 的结构

/joint_states
  给出每个 joint 当前角度

robot_state_publisher
  读取 URDF 和 /joint_states
  发布 TF

RViz2
  显示 robot model 和 TF
```

必须理解：

- [ ] `link` 是刚体
- [ ] `joint` 是 link 之间的连接
- [ ] revolute joint 需要 axis、limit、origin
- [ ] RViz2 不负责计算运动学，它只是显示 TF 和模型
- [ ] `robot_state_publisher` 才是把 joint angle 转成 TF 的关键节点

验收：

- [ ] 能画出上面的数据流
- [ ] 能解释 URDF、joint state、TF、RViz2 各自负责什么

---

### Day 2：创建 simple_arm_description 包

目标：建立机械臂描述包。

建议命令：

```bash
cd ~/ros2_ws/src
ros2 pkg create simple_arm_description --build-type ament_cmake
```

目录结构：

```text
simple_arm_description/
├── urdf/
│   └── simple_2dof_arm.urdf.xacro
├── launch/
│   └── display.launch.py
├── rviz/
│   └── simple_arm.rviz
├── package.xml
└── CMakeLists.txt
```

今天任务：

- [ ] 创建 `urdf/`
- [ ] 创建 `launch/`
- [ ] 创建 `rviz/`
- [ ] 修改 `CMakeLists.txt`，安装这些目录
- [ ] 检查 `package.xml` 是否需要加入 `xacro`、`robot_state_publisher`、`rviz2`

验收：

- [ ] `colcon build` 成功
- [ ] `ros2 pkg list | grep simple_arm_description` 能找到包

---

### Day 3：写最小 2 自由度 URDF / xacro

目标：先让 RViz2 能显示一个简单机械臂，不追求外观。

模型结构：

```text
base_link
  |
joint1: revolute, 绕 z 轴
  |
link1
  |
joint2: revolute, 绕 z 轴
  |
link2
```

建议尺寸：

- `base_link`：小盒子
- `link1`：长度 1.0
- `link2`：长度 0.8
- `joint1` limit：`-3.14` 到 `3.14`
- `joint2` limit：`-1.57` 到 `1.57`

今天任务：

- [ ] 写 `simple_2dof_arm.urdf.xacro`
- [ ] 每个 link 都有 visual
- [ ] 每个 revolute joint 都有 axis
- [ ] 每个 revolute joint 都有 limit
- [ ] 用 `xacro` 检查能否展开

验收命令：

```bash
ros2 run xacro xacro src/simple_arm_description/urdf/simple_2dof_arm.urdf.xacro
```

验收：

- [ ] xacro 能正常输出 robot XML
- [ ] 没有 XML 语法错误
- [ ] 能说清楚 `origin xyz rpy` 是父子 link 的相对位姿

---

### Day 4：启动 robot_state_publisher + RViz2

目标：在 RViz2 里看到静态机械臂。

新增文件：

```text
simple_arm_description/launch/display.launch.py
```

launch 启动：

- [ ] `robot_state_publisher`
- [ ] `joint_state_publisher_gui` 或后面自己写的 joint state 节点
- [ ] `rviz2`

如果先用 GUI：

```bash
sudo apt install ros-$ROS_DISTRO-joint-state-publisher-gui
```

启动命令：

```bash
ros2 launch simple_arm_description display.launch.py
```

验收：

- [ ] RViz2 中 Fixed Frame 设为 `base_link`
- [ ] 能看到 RobotModel
- [ ] 拖动 joint slider 后机械臂姿态变化
- [ ] TF tree 没有明显断裂

今天重点：

- [ ] 不纠结模型好不好看
- [ ] 先保证 link / joint / TF 链路通

---

### Day 5：把 /joint_position_cmd 转成 /joint_states

目标：让第 3 周的控制输出驱动 RViz2 里的机械臂。

新增节点：

```text
robot_control_ros2/src/position_to_joint_state_node.cpp
```

输入：

- `/joint_position_cmd`

输出：

- `/joint_states`

消息类型：

- 输入仍用 `std_msgs/msg/Float64MultiArray`
- 输出用 `sensor_msgs/msg/JointState`

节点逻辑：

```text
订阅 /joint_position_cmd
取前 2 个关节角
填入 JointState:
  name = ["joint1", "joint2"]
  position = [q0, q1]
  header.stamp = now()
发布 /joint_states
```

今天任务：

- [ ] 给 `robot_control_ros2` 增加 `sensor_msgs` 依赖
- [ ] 写 `position_to_joint_state_node.cpp`
- [ ] 修改 `CMakeLists.txt`
- [ ] 编译通过
- [ ] 手动发布 `/joint_position_cmd` 测试 RViz2 姿态变化

验收命令：

```bash
ros2 run robot_control_ros2 position_to_joint_state_node
ros2 topic pub /joint_position_cmd std_msgs/msg/Float64MultiArray "{data: [0.5, 0.3]}"
ros2 topic echo /joint_states
```

验收：

- [ ] `/joint_states` 能看到 joint1、joint2
- [ ] RViz2 中机械臂会随输入姿态变化
- [ ] 能解释为什么 joint name 必须和 URDF 里的 joint 名字一致

---

### Day 6：完整联调

目标：把整条链路跑起来。

完整链路：

```text
/joint_velocity_cmd
  -> vel_to_pos_node
  -> /joint_position_cmd
  -> position_to_joint_state_node
  -> /joint_states
  -> robot_state_publisher
  -> /tf
  -> RViz2
```

今天任务：

- [ ] 启动 `vel_to_pos_node`
- [ ] 启动 `position_to_joint_state_node`
- [ ] 启动 `display.launch.py`
- [ ] 手动发布 `/joint_velocity_cmd`
- [ ] 观察 `/joint_position_cmd`
- [ ] 观察 `/joint_states`
- [ ] 观察 RViz2 姿态变化

验收命令示例：

```bash
ros2 launch simple_arm_description display.launch.py
ros2 run robot_control_ros2 vel_to_pos_node
ros2 run robot_control_ros2 position_to_joint_state_node
ros2 topic pub /joint_velocity_cmd std_msgs/msg/Float64MultiArray "{data: [0.1, 0.1, 0.0, 0.0, 0.0, 0.0]}"
```

验收：

- [ ] 发布速度后，位置会积分变化
- [ ] joint state 会更新
- [ ] RViz2 里的机械臂会动
- [ ] 能画出完整数据流

---

### Day 7：README、简历描述和面试讲解稿

目标：把项目变成能展示的东西。

README 必须包含：

- [ ] 项目目标
- [ ] 系统架构图
- [ ] package 说明
- [ ] topic 说明
- [ ] parameter 说明
- [ ] build 命令
- [ ] run 命令
- [ ] RViz2 展示说明
- [ ] 已知限制

简历描述初稿：

```text
实现了一个基于 ROS2 的简化机械臂速度-位置控制与可视化系统。项目包含纯 C++ 控制库、ROS2 速度积分位置控制节点、URDF 机械臂模型和 RViz2 可视化链路。通过 timer 控制循环订阅关节速度命令，进行限幅和积分后发布关节位置，并转换为 JointState 驱动 robot_state_publisher 生成 TF，实现机械臂姿态实时显示。
```

面试讲解顺序：

1. 为什么先做纯 C++ 控制库：控制逻辑不依赖 ROS，方便测试。
2. ROS2 节点怎么设计：速度输入、位置输出、timer 控制循环。
3. 为什么要限幅：防止速度和位置命令超过安全范围。
4. URDF 和 RViz2 怎么接：位置命令转 JointState，再由 robot_state_publisher 发布 TF。
5. 项目还能怎么扩展：接真实控制器、接 MoveIt2、加入轨迹跟踪和雅可比逆解。

第 4 周总验收：

- [ ] RViz2 中能看到简单 2 自由度机械臂
- [ ] 能通过 `/joint_position_cmd` 改变机械臂姿态
- [ ] 能通过 `/joint_velocity_cmd` 驱动完整链路
- [ ] README 可以让别人复现项目
- [ ] 简历描述能在 3 到 5 句话内讲清楚

---

---


### 第 4 周每日详细执行指南：URDF / RViz2 / TF2 可视化闭环

#### Day 1：先理解可视化链路

实现过程：

- 画出链路：URDF -> robot_state_publisher -> TF -> RViz2
- 理解 `/joint_states` 的作用
- 理解 `robot_description` 参数的作用
- 阅读一个最小 URDF 示例
- 记录每个工具负责哪一段

知识点：

- URDF 描述机器人结构
- TF 描述坐标系之间的实时关系
- RViz2 只是可视化工具，不负责控制
- `robot_state_publisher` 根据 URDF 和 joint states 发布 TF
- 关节状态是可视化活动机械臂的关键输入

今日产出：

- 能画出 URDF 到 RViz2 的数据流
- 能解释 `/joint_states` 和 `/tf` 的关系
- 能说清 RViz2 里机械臂为什么会动

#### Day 2：创建 simple_arm_description 包

实现过程：

- 创建 `simple_arm_description`
- 建立 `urdf/`、`launch/`、`rviz/` 目录
- 配置 `package.xml`
- 配置 `CMakeLists.txt` 安装 urdf、launch、rviz 文件
- 用 `colcon build` 验证包能编译

知识点：

- description 包通常只放模型、launch 和可视化配置
- URDF 文件需要被安装到 share 目录才能被 launch 找到
- ROS2 package 不一定必须有 C++ 节点
- `install(DIRECTORY ...)` 是资源文件包常用写法

今日产出：

- `simple_arm_description` 包存在并能编译
- 模型资源目录结构清楚
- 能解释 description 包和 control 包的区别

#### Day 3：写最小 2 自由度 URDF / xacro

实现过程：

- 创建 `simple_2dof_arm.urdf.xacro`
- 定义 `base_link`
- 定义 `link1`、`link2`
- 定义两个 revolute joint
- 设置 joint axis 和 origin
- 添加简单 visual 几何体
- 用 `xacro` 或 launch 加载模型

知识点：

- link 表示刚体，joint 表示连接关系
- revolute joint 表示旋转关节
- `origin xyz rpy` 表示相对位姿
- `axis` 表示关节旋转轴
- xacro 可以减少重复，适合后续参数化模型

今日产出：

- 有一个 2 自由度平面机械臂模型
- 模型语法能通过检查
- 能解释 link 和 joint 的关系

#### Day 4：启动 robot_state_publisher + RViz2

实现过程：

- 写 `display.launch.py`
- 在 launch 中加载 xacro 为 `robot_description`
- 启动 `robot_state_publisher`
- 启动 `joint_state_publisher_gui` 或准备后续自写 joint state 节点
- 启动 RViz2 并设置 Fixed Frame
- 保存 RViz 配置

知识点：

- `robot_description` 通常是 URDF XML 字符串
- `robot_state_publisher` 需要机器人模型和关节状态
- RViz2 Fixed Frame 必须选择 TF 树中存在的坐标系
- GUI joint state publisher 适合手动拖动关节测试模型

今日产出：

- RViz2 中能看到机械臂
- 拖动 joint 后机械臂能变化
- 能解释 TF 树中 base、link1、link2 的关系

#### Day 5：把 /joint_position_cmd 转成 /joint_states

实现过程：

- 创建一个 joint state 发布节点
- 订阅 `/joint_position_cmd`
- 将位置数组转换成 `sensor_msgs::msg::JointState`
- 填充 `name`、`position`、`velocity`、`header.stamp`
- 发布 `/joint_states`
- 和前面的 `vel_to_pos_node` 联调

知识点：

- `/joint_position_cmd` 是你的控制命令
- `/joint_states` 是 ROS 生态通用的关节状态接口
- JointState 的 `name` 顺序要和 URDF joint 名称匹配
- 可视化链路通常使用标准消息类型

今日产出：

- 速度命令能间接驱动 RViz2 中机械臂运动
- 能解释命令 topic 和状态 topic 的区别
- 能说清从控制节点到可视化的完整链路

#### Day 6：完整联调

实现过程：

- 启动 `vel_to_pos_node`
- 启动 joint state 转换节点
- 启动 `robot_state_publisher`
- 启动 RViz2
- 发送速度命令
- 观察 `/joint_position_cmd`、`/joint_states`、`/tf`
- 记录每个 topic 是否正常

知识点：

- 系统联调要逐段确认，不要只看最终 RViz2
- `ros2 topic echo` 用来看数据
- `ros2 topic hz` 用来看频率
- TF 问题通常和 frame 名称、joint 名称、时间戳有关

今日产出：

- 至少一条完整控制到可视化链路能跑通
- 有清楚的问题记录
- 能用命令定位是控制、状态发布还是可视化出问题

#### Day 7：README、简历描述和面试讲解稿

实现过程：

- 更新 README 的可视化章节
- 写出启动命令
- 加入系统链路图
- 写简历描述初稿
- 写 1 分钟讲解稿
- 记录当前限制，例如模型简单、无真实动力学

知识点：

- 项目展示要讲清楚闭环链路，而不是只说“我打开了 RViz”
- 简历描述要突出 ROS2、URDF、TF、RViz2 和控制节点连接
- 面试讲解要能区分命令、状态、可视化和模型

今日产出：

- README 能复现 RViz2 demo
- 简历中有一条可用项目描述
- 能完整讲清第 4 周可视化闭环

---


## 第 5 周：MoveIt2 机械臂运动规划

### 本周定位

第 5 周开始从“自己写控制链路”进入“工业常用规划框架”。目标不是一口气掌握 MoveIt2 源码，而是先把一个机械臂模型接入 MoveIt2，能在 RViz2 中完成规划和执行。

本周目标：

- [ ] 理解 MoveIt2 的核心组成：MoveGroup、PlanningScene、PlanningPipeline、Planner、TrajectoryExecution
- [ ] 为 `simple_arm_description` 生成 MoveIt2 配置包
- [ ] 能在 RViz2 MotionPlanning 插件中拖动目标位姿或关节目标
- [ ] 能使用 OMPL 完成一次规划
- [ ] 能解释 MoveIt2 和自己写的 `vel_to_pos_node` 的区别

### MoveIt2 核心链路

```text
RobotModel / SRDF
  -> PlanningScene
  -> PlanningPipeline
  -> OMPL Planner
  -> RobotTrajectory
  -> TrajectoryExecution
  -> ros2_control controller
```

今天先理解这些概念：

- [ ] `move_group` 节点负责什么
- [ ] URDF 和 SRDF 的区别
- [ ] Planning Group 是什么
- [ ] Planning Scene 为什么要维护碰撞环境
- [ ] OMPL planner 在 MoveIt2 中处于哪一层

### 本周实作任务

### Day 1：MoveIt2 概念和安装检查

今天先不要急着配置机械臂，先确认环境和概念。

- [x] 确认 ROS2 发行版和 MoveIt2 是否匹配
	jazzy版本的ROS 
	sudo apt install ros-jazzy-moveit安装对应的Moveit
- [x] 安装或检查 `moveit`、`moveit_setup_assistant`
	`QT_QPA_PLATFORM=xcb ros2 launch moveit_setup_assistant setup_assistant.launch.py`
- [x] 跑一个官方 MoveIt2 demo 或 tutorial
- [x] 记录 `move_group`、RViz2 MotionPlanning、PlanningScene 的作用
	move_group 是 MoveIt2 的核心后端节点。它负责加载机器人模型、语义模型和规划配置，包括 URDF、SRDF、joint_limits、kinematics、OMPL 等参数。它内部集成了机器人状态监控、PlanningScene 管理、碰撞检测、路径规划、轨迹后处理和轨迹执行管理等功能。  
	RViz2 MotionPlanning 是 MoveIt2 的图形交互插件。它本身不是规划器，也不直接完成碰撞检测和路径搜索。它主要作为前端界面，用来显示机器人模型、规划场景、起点状态、目标状态和规划轨迹。
	PlanningScene 是 MoveIt2 中描述当前规划环境的核心数据结构。它包含机器人当前状态、机器人模型、关节限制、自碰撞信息、环境障碍物、附着物体、允许碰撞矩阵等信息。
- [x] 在笔记中画出 MoveIt2 核心链路

验收：

- [x] 能启动一个 MoveIt2 示例
- [ ] 能说清楚 MoveIt2 主要负责规划，不直接等同于底层控制器
	MoveIt2 主要负责运动规划和轨迹生成。它根据机器人模型、当前状态、目标状态和 PlanningScene 中的环境信息，计算一条满足关节限制和碰撞约束的轨迹。
	MoveIt2 本身不是电机驱动器，也不是底层控制器。它不会直接控制电机电流、PWM、伺服周期或驱动器通信。MoveIt2 输出的是较高层的轨迹，例如一组带时间戳的关节位置、速度和加速度。
- [x] 能解释 URDF 和 SRDF 的区别
	URDF 描述机器人“物理模型”和“运动学结构”。
	SRDF是路径规划相关的参数配置语义信息

### Day 2：检查 simple arm 模型是否适合 MoveIt2

今天检查第 4 周做的机械臂模型。

- [x] 检查 `simple_2dof_arm.urdf.xacro`
	ros2 xacro xacro ....xacro
- [x] 确认 joint 类型、axis、limit 是否完整
	revolute；z轴；位置限位
- [x] 确认 link / joint 名称清晰
- [x] 用 `xacro` 展开模型
- [x] 用 RViz2 显示模型
- [x] 记录哪些地方需要为了 MoveIt2 修改
	collision

验收：

- [ ] URDF 能被正常展开
- [ ] 每个 revolute joint 都有 limit
- [ ] 能解释为什么 MoveIt2 需要 joint limits

### Day 3：创建 simple_arm_moveit_config

目标是生成或手写 MoveIt2 配置包。

- [x] 使用 MoveIt Setup Assistant 或手动创建 `simple_arm_moveit_config`
- [x] 配置 planning group：`arm`
- [x] 配置 planning joints：`joint1`、`joint2`
- [x] 生成或整理 SRDF
- [x] 生成 `joint_limits.yaml`
	需要注意是小数
- [x] 生成 `kinematics.yaml`
- [x] 生成 `ompl_planning.yaml`

验收：

- [x] 配置包能被 colcon 编译
- [x] 能解释 planning group 是什么
- [x] 能在文件里找到 `arm` 对应哪些 joints

### Day 4：启动 MoveIt2 RViz demo

今天目标是看到 MotionPlanning 面板并能规划。

- [x] 启动 `demo.launch.py`
- [x] RViz2 Fixed Frame 设置正确
- [x] MotionPlanning 面板加载成功
- [x] 选择 planning group
- [x] 设置一个关节目标
- [x] 点击 Plan
- [x] 观察规划轨迹

验收：

- [ ] RViz2 中能看到机械臂
- [ ] 能规划至少 1 条轨迹
- [ ] 能说出规划失败时先检查哪几个地方：joint limits、planning group、TF、碰撞模型、日志

### Day 5：OMPL planner 对比

今天重点不是调参到完美，而是理解 MoveIt2 通过 OMPL 调用不同规划器。

- [ ] 找到 `ompl_planning.yaml`
- [ ] 尝试 RRTConnect
- [ ] 尝试 RRTstar
- [ ] 尝试改变 planning time
- [ ] 对同一个目标重复规划 3 次
- [ ] 记录规划时间、成功率、路径现象

验收：

- [ ] 能解释 OMPL 是 MoveIt2 的规划插件之一
- [ ] 能说明 RRTConnect 通常适合快速找可行解
- [ ] 能说明 RRTstar 更偏向渐进优化，但可能更慢

### Day 6：MoveGroupInterface C++ demo

今天写一个应用层 C++ 节点调用 MoveIt2。

- [ ] 创建 `moveit_cpp_demo.cpp`
- [ ] 引入 `moveit_ros_planning_interface`
- [ ] 创建 `MoveGroupInterface`
- [ ] 设置 planning group
- [ ] 设置 joint target
- [ ] 调用 `plan()`
- [ ] 打印规划是否成功

验收：

- [ ] 能用 `ros2 run` 启动 demo
- [ ] 能通过代码触发一次规划
- [ ] 能解释 `MoveGroupInterface` 是应用层接口，不是底层 planner 本身

### Day 7：MoveIt2 复盘和 README

今天把 MoveIt2 变成项目资产。

- [ ] 在 README 增加 MoveIt2 运行命令
- [ ] 写 MoveIt2 架构图
- [ ] 写配置包说明
- [ ] 写 planning group 说明
- [ ] 写 OMPL planner 对比记录
- [ ] 写“MoveIt2 与自写控制链路区别”表格

验收：

- [ ] 别人能按 README 启动 MoveIt2 demo
- [ ] 能用 1 分钟解释 MoveIt2 在项目中的位置

验收：

- [ ] MoveIt2 demo 能启动
- [ ] RViz2 中能看到 MotionPlanning 面板
- [ ] 能完成一次 OMPL 规划
- [ ] 能写一个 C++ MoveGroupInterface demo
- [ ] README 增加 MoveIt2 运行说明

---

---


### 第 5 周每日详细执行指南：MoveIt2 规划

#### Day 1：MoveIt2 概念和安装检查

实现过程：

- 检查 MoveIt2 是否安装
- 运行官方 demo 或查看版本
- 学习 planning scene、planning group、move_group、OMPL 的关系
- 记录 MoveIt2 输入输出是什么

知识点：

- MoveIt2 负责运动规划，不直接等于底层电机控制
- planning group 是一组可规划关节
- planning scene 包含机器人、环境和碰撞信息
- OMPL 是常见采样规划库

今日产出：

- 能确认环境是否可用
- 能解释 MoveIt2 在项目中的位置
- 能说清 planner 和 controller 的区别

#### Day 2：检查 simple arm 模型是否适合 MoveIt2

实现过程：

- 检查 URDF 是否有完整 link 和 joint
- 检查 joint limit 是否合理
- 检查是否有 collision 几何
- 检查 TF 树是否连通
- 记录需要补齐的模型信息

知识点：

- MoveIt2 需要机器人模型、关节限制和碰撞模型
- visual 只用于显示，collision 用于碰撞检查
- joint limit 会影响规划空间
- 模型质量直接影响规划是否成功

今日产出：

- 有一份模型检查清单
- 知道 simple arm 哪些地方需要补齐
- 能解释 visual 和 collision 的区别

#### Day 3：创建 simple_arm_moveit_config

实现过程：

- 使用 MoveIt Setup Assistant 或手动创建配置包
- 定义 planning group，例如 `arm`
- 配置 joint limits
- 配置 kinematics
- 生成 SRDF
- 检查配置文件目录

知识点：

- SRDF 是对 URDF 的语义补充
- planning group 决定哪些关节参与规划
- kinematics 配置决定逆解插件和参数
- MoveIt2 配置包通常不写核心算法，而是组织配置

今日产出：

- 有 `simple_arm_moveit_config`
- 能找到 `arm` group 对应的 joints
- 能解释 SRDF 解决了 URDF 没表达的哪些信息

#### Day 4：启动 MoveIt2 RViz demo

实现过程：

- 启动 `demo.launch.py`
- 打开 RViz2 MotionPlanning 面板
- 设置 Fixed Frame
- 选择 planning group
- 设置一个关节目标
- 点击 Plan
- 观察规划轨迹

知识点：

- MotionPlanning 面板是 MoveIt2 的交互调试入口
- Plan 表示只规划，不一定执行
- Execute 需要控制器支持
- 规划失败要看模型、limits、group 和碰撞设置

今日产出：

- RViz2 中能看到 MotionPlanning 面板
- 至少成功规划一条轨迹
- 能解释 Plan 和 Execute 的区别

#### Day 5：OMPL planner 对比

实现过程：

- 查看 `ompl_planning.yaml`
- 尝试不同 planner，例如 RRTConnect、RRTstar
- 对同一个目标重复规划
- 记录规划时间和轨迹效果
- 写一张简单对比表

知识点：

- 采样规划不保证每次结果完全一样
- 不同 planner 适合不同场景
- 规划时间、路径质量、成功率都需要观察
- 工程中通常先用稳定默认配置，再逐步调参

今日产出：

- 有 OMPL planner 对比记录
- 能解释为什么同一目标规划结果可能不同
- 能知道 MoveIt2 中 planner 配置在哪里

#### Day 6：MoveGroupInterface C++ demo

实现过程：

- 创建 C++ demo 节点
- 初始化 `rclcpp`
- 创建 `MoveGroupInterface`
- 设置 planning group
- 设置 joint target 或 pose target
- 调用 `plan`
- 打印规划是否成功

知识点：

- MoveGroupInterface 是 C++ 调用 MoveIt2 的常用接口
- 代码规划和 RViz2 规划本质上都在调用 move_group
- 目标可以是关节空间，也可以是末端位姿
- demo 先只做 plan，不急着 execute

今日产出：

- C++ 代码能调用 MoveIt2 规划
- 能解释 RViz2 操作和 C++ API 的对应关系
- 有可运行的规划 demo

#### Day 7：MoveIt2 复盘和 README

实现过程：

- 更新 README 中 MoveIt2 章节
- 写配置包结构说明
- 记录启动命令
- 写 planning group、SRDF、OMPL 的说明
- 整理常见失败点

知识点：

- MoveIt2 是规划层，不是底层控制层
- 配置文件是 MoveIt2 项目的核心资产之一
- README 要能帮助别人复现规划 demo
- 面试讲解要强调“我知道规划链路怎么连起来”

今日产出：

- README 能说明 MoveIt2 demo 如何启动
- 能解释规划链路
- 能说出当前项目还缺少 ros2_control 执行链路

---


## 第 6 周：ros2_control 与控制器接口

### 本周定位

第 6 周把机械臂从“RViz2 可视化”和“MoveIt2 规划”推进到“控制器接口”。`ros2_control` 是 ROS2 中连接上层规划和底层硬件/仿真的关键框架。

本周目标：

- [ ] 理解 `ros2_control`、`controller_manager`、`hardware_interface`
- [ ] 配置 `joint_state_broadcaster`
- [ ] 配置 `joint_trajectory_controller`
- [ ] 让 MoveIt2 输出轨迹发送到 controller
- [ ] 能解释 `FollowJointTrajectory` action 的作用

核心概念：

| 概念 | 作用 |
|---|---|
| `ros2_control` | 控制框架 |
| `hardware_interface` | 抽象真实硬件或仿真硬件 |
| `controller_manager` | 管理控制器加载、启动、停止 |
| `joint_state_broadcaster` | 发布关节状态 |
| `joint_trajectory_controller` | 接收轨迹并执行 |
| `FollowJointTrajectory` | MoveIt2 和控制器之间常用 action 接口 |

本周任务：

### Day 1：理解 ros2_control 架构

- [ ] 学习 `ros2_control` 总体结构
	`ros2_control` 是 ROS2 中连接“上层规划/控制命令”和“底层真实硬件或仿真硬件”的标准控制框架。
	它不是单纯的一个节点，也不是单纯的一个控制器，而是一整套结构：**URDF 描述硬件接口 → controller_manager 管理控制器 → controller 产生控制指令 → hardware_interface 读写真实硬件或仿真硬件**。
- [ ] 区分 hardware、controller、controller_manager
- [ ] 理解 command interface 和 state interface
- [ ] 整理 `position`、`velocity`、`effort` 三类接口
- [ ] 记录 `joint_state_broadcaster` 和 `joint_trajectory_controller` 的职责

验收：

- [ ] 能画出 `MoveIt2 -> controller -> hardware` 的关系
- [ ] 能解释为什么 MoveIt2 不直接控制电机

### Day 2：在 URDF 中加入 ros2_control 标签

- [ ] 修改 `simple_2dof_arm.urdf.xacro`
- [ ] 为 joint1、joint2 添加 command interface
- [ ] 为 joint1、joint2 添加 state interface
- [ ] 先使用 mock 或 fake hardware
- [ ] 用 xacro 检查 XML 是否正确

验收：

- [ ] URDF 能展开
- [ ] 能解释 command interface 是上层写入的命令
- [ ] 能解释 state interface 是底层反馈的状态

### Day 3：配置 controllers.yaml

- [ ] 创建或整理 `ros2_controllers.yaml`
- [ ] 配置 `controller_manager`
- [ ] 配置 `joint_state_broadcaster`
- [ ] 配置 `joint_trajectory_controller`
- [ ] 指定 joints：`joint1`、`joint2`
- [ ] 指定 command interfaces
- [ ] 指定 state interfaces

验收：

- [ ] YAML 结构清楚
- [ ] 能解释 controller 名称、type、joints 各自含义

### Day 4：启动 controller_manager

- [ ] 写或修改 launch 文件
- [ ] 启动 `ros2_control_node`
- [ ] 加载 `robot_description`
- [ ] 检查 controller manager 是否存在
- [ ] 执行 `ros2 control list_hardware_interfaces`
- [ ] 执行 `ros2 control list_controllers`

验收：

- [ ] 能看到 hardware interfaces
- [ ] 能看到 controller 处于 configured 或 active 状态

### Day 5：加载并测试 joint_state_broadcaster

- [ ] 加载 `joint_state_broadcaster`
- [ ] 激活 controller
- [ ] echo `/joint_states`
- [ ] 检查 joint name 是否与 URDF 一致
- [ ] 检查 RViz2 是否能显示状态

验收：

- [ ] `/joint_states` 正常发布
- [ ] RViz2 模型不报 TF / joint state 错误

### Day 6：测试 joint_trajectory_controller

- [ ] 加载 `joint_trajectory_controller`
- [ ] 查看 action 列表
- [ ] 确认 `FollowJointTrajectory` action 存在
- [ ] 用命令行或小脚本发送一条简单轨迹
- [ ] 观察 `/joint_states`
- [ ] 观察 RViz2 中机械臂变化

验收：

- [ ] 能发送一条目标关节轨迹
- [ ] 能解释 trajectory point 里的 positions 和 time_from_start
- [ ] 能解释 action goal、feedback、result

### Day 7：连接 MoveIt2 Execute

- [ ] 修改 MoveIt2 controller 配置
- [ ] 确认 MoveIt2 找到 trajectory controller
- [ ] 在 RViz2 中 Plan
- [ ] 点击 Execute
- [ ] 观察 controller 是否收到轨迹
- [ ] 记录失败原因和解决方式

验收：

- [ ] MoveIt2 Execute 至少能连到 controller 接口
- [ ] 能说明 MoveIt2 规划结果如何通过 action 发给 ros2_control

验收命令方向：

```bash
ros2 control list_controllers
ros2 control list_hardware_interfaces
ros2 action list
ros2 action info /joint_trajectory_controller/follow_joint_trajectory
```

本周验收：

- [ ] 能看到 controller 已 active
- [ ] 能解释 broadcaster 和 controller 的区别
- [ ] 能发送一条 FollowJointTrajectory 测试轨迹
- [ ] 能说明 MoveIt2 规划结果如何进入 ros2_control

---

---


### 第 6 周每日详细执行指南：ros2_control 和控制器

#### Day 1：理解 ros2_control 架构

实现过程：

- 学习 controller_manager、hardware_interface、controller 的关系
- 画出 MoveIt2 -> trajectory controller -> hardware 的链路
- 记录 joint_state_broadcaster 和 joint_trajectory_controller 的作用

知识点：

- ros2_control 是 ROS2 标准控制框架
- controller_manager 负责管理控制器生命周期
- hardware_interface 抽象真实硬件或模拟硬件
- controller 负责把命令转换为硬件接口调用

今日产出：

- 能画出 ros2_control 架构图
- 能解释 controller_manager 的作用
- 能说清 broadcaster 和 controller 的区别

#### Day 2：在 URDF 中加入 ros2_control 标签

实现过程：

- 在 xacro 中添加 `<ros2_control>` 标签
- 定义硬件插件
- 为每个 joint 配置 command interface
- 为每个 joint 配置 state interface
- 检查 joint 名称和 URDF 一致

知识点：

- ros2_control 信息通常写在 URDF/xacro 中
- command interface 表示控制器可以写入什么命令
- state interface 表示系统可以读出什么状态
- joint 名称必须贯穿 URDF、controller 配置和 MoveIt2

今日产出：

- URDF 中包含 ros2_control 配置
- 能解释 command interface 和 state interface
- 能知道 joint 名称不一致会导致什么问题

#### Day 3：配置 controllers.yaml

实现过程：

- 创建 `controllers.yaml`
- 配置 `controller_manager`
- 添加 `joint_state_broadcaster`
- 添加 `joint_trajectory_controller`
- 配置 joints、command_interfaces、state_interfaces
- 检查 YAML 缩进

知识点：

- YAML 缩进错误会导致参数加载失败
- controller 名称会影响后续加载命令
- `joint_trajectory_controller` 接收轨迹命令
- MoveIt2 Execute 通常依赖 trajectory controller

今日产出：

- 有可读的 controllers.yaml
- 能解释每个 controller 的作用
- 能说明 controller 名称、type、joints 的含义

#### Day 4：启动 controller_manager

实现过程：

- 写或修改 launch 文件
- 启动 `ros2_control_node`
- 加载 `robot_description`
- 加载 controllers.yaml
- 执行 `ros2 control list_hardware_interfaces`
- 执行 `ros2 control list_controllers`

知识点：

- controller_manager 是 ros2_control 的运行核心
- 硬件接口加载失败通常和 URDF 或插件有关
- controller 可以处于 unconfigured、inactive、active 等状态
- ros2 control 命令用于调试控制器状态

今日产出：

- 能看到 hardware interfaces
- 能看到 controller_manager
- 能解释 controller 状态含义

#### Day 5：加载并测试 joint_state_broadcaster

实现过程：

- 使用 spawner 加载 `joint_state_broadcaster`
- 检查 controller 是否 active
- 查看 `/joint_states`
- 确认 joint 名称和数量正确
- 在 RViz2 中确认状态可视化正常

知识点：

- joint_state_broadcaster 负责把硬件状态发布成 `/joint_states`
- 它不是轨迹控制器
- `/joint_states` 是许多 ROS 工具共用的标准接口
- broadcaster active 是后续可视化和 MoveIt2 状态同步的基础

今日产出：

- `/joint_states` 能正常发布
- RViz2 能读取关节状态
- 能解释为什么先启动 broadcaster

#### Day 6：测试 joint_trajectory_controller

实现过程：

- 使用 spawner 加载 `joint_trajectory_controller`
- 检查 controller active
- 手动发送一条简单 trajectory
- 观察 joint states 是否变化
- 检查控制器 action/topic 接口

知识点：

- trajectory controller 接收的是轨迹，不是单个速度值
- 轨迹点包含位置、速度和时间
- FollowJointTrajectory 是机械臂控制常见接口
- 控制器执行失败要检查 joint 名称、时间戳和接口类型

今日产出：

- 能手动发送简单轨迹
- joint states 能随轨迹变化
- 能解释 trajectory controller 的输入格式

#### Day 7：连接 MoveIt2 Execute

实现过程：

- 检查 MoveIt2 controller 配置
- 让 MoveIt2 找到 `joint_trajectory_controller`
- 在 RViz2 中 Plan
- 尝试 Execute
- 观察 controller 是否收到轨迹
- 记录失败点

知识点：

- MoveIt2 Execute 需要标准控制器接口
- Plan 成功不代表 Execute 一定成功
- 规划层和控制层通过 action/controller 配置连接
- 联调时要同时看 MoveIt2 日志和 controller 状态

今日产出：

- 至少能明确 Execute 链路卡在哪里
- 理想状态下能完成一次规划执行
- 能解释 MoveIt2 和 ros2_control 的连接关系

---


## 第 7 周：机械臂项目整合与工程化

### 本周定位

第 7 周不再继续堆新概念，而是把前面内容整合成一个完整可展示项目。

最终项目展示链路：

```text
MoveIt2 target
  -> OMPL planning
  -> RobotTrajectory
  -> FollowJointTrajectory
  -> joint_trajectory_controller
  -> joint states
  -> robot_state_publisher
  -> RViz2
```

同时保留自写控制链路：

```text
/joint_velocity_cmd
  -> vel_to_pos_node
  -> /joint_position_cmd
  -> /joint_states
  -> RViz2
```

本周任务：

### Day 1：建立 bringup 包

- [ ] 创建 `simple_arm_bringup`
- [ ] 建立 `launch/`
- [ ] 建立 `config/`
- [ ] 把启动相关逻辑从各包中整理出来
- [ ] 写 `sim_control.launch.py`
- [ ] 写 `moveit_control.launch.py`
- [ ] 写 `full_demo.launch.py` 草稿

验收：

- [ ] bringup 包能编译
- [ ] launch 文件命名清楚
- [ ] 能解释 description、moveit_config、bringup 三个包各自负责什么

### Day 2：整理 launch 启动顺序

- [ ] 确认 robot_description 从哪里加载
- [ ] 确认 robot_state_publisher 启动顺序
- [ ] 确认 controller_manager 启动顺序
- [ ] 确认 MoveIt2 move_group 启动顺序
- [ ] 确认 RViz2 启动配置
- [ ] 给 launch 文件加入必要参数

验收：

- [ ] 能单独启动可视化链路
- [ ] 能单独启动 ros2_control 链路
- [ ] 能单独启动 MoveIt2 链路

### Day 3：统一配置文件

- [ ] 检查 `joint_limits.yaml`
- [ ] 检查 `kinematics.yaml`
- [ ] 检查 `ompl_planning.yaml`
- [ ] 检查 `ros2_controllers.yaml`
- [ ] 删除重复或无效配置
- [ ] 为每个配置文件写一句说明

验收：

- [ ] 重要配置都能在 README 中解释
- [ ] 不再依赖“我自己记得这个文件干嘛用”

### Day 4：完整联调

- [ ] 启动 `full_demo.launch.py`
- [ ] 检查 `/tf`
- [ ] 检查 `/joint_states`
- [ ] 检查 controller 状态
- [ ] 在 MoveIt2 中 Plan
- [ ] 尝试 Execute
- [ ] 记录问题清单

验收：

- [ ] 至少有一条完整链路能稳定跑
- [ ] 所有失败点都有记录，而不是只说“跑不起来”

### Day 5：README 完整复现步骤

README 必须包含：

- [ ] 项目目标
- [ ] 系统架构图
- [ ] 包结构说明
- [ ] 依赖安装
- [ ] build 命令
- [ ] source 命令
- [ ] 自写控制链路运行命令
- [ ] MoveIt2 规划运行命令
- [ ] ros2_control 测试命令
- [ ] 常见问题

验收：

- [ ] 只看 README 可以复现基础演示
- [ ] README 中没有“看情况”“自己改一下”这种模糊描述

### Day 6：简历描述和面试讲解稿

- [ ] 写 3 行简历项目描述
- [ ] 写 1 分钟讲解版本
- [ ] 写 3 分钟讲解版本
- [ ] 写“项目难点”3 条
- [ ] 写“如果继续扩展”3 条
- [ ] 写“我负责/我实现了什么”清单

验收：

- [ ] 能不看文档讲 1 分钟
- [ ] 能回答为什么用了 MoveIt2 和 ros2_control

### Day 7：项目复盘和下一阶段计划

- [ ] 列出已完成内容
- [ ] 列出未完成内容
- [ ] 列出 3 个最值得修的 bug
- [ ] 列出 3 个最值得加的功能
- [ ] 决定是否进入 Nav2 / 视觉 / C++ 重构阶段

验收：

- [ ] 项目状态清楚
- [ ] 下一步不是凭感觉，而是根据清单推进

本周验收：

- [ ] 一条 launch 能启动完整演示
- [ ] README 能让别人复现
- [ ] 能讲清楚自写控制、MoveIt2、ros2_control 三者关系
- [ ] 项目可以作为简历核心项目

---

---


### 第 7 周每日详细执行指南：Bringup 和完整系统集成

#### Day 1：建立 bringup 包

实现过程：

- 创建 `simple_arm_bringup`
- 建立 `launch/` 和 `config/`
- 把系统级启动文件放进 bringup 包
- 配置安装规则

知识点：

- bringup 包负责系统启动，不负责具体算法
- description、control、moveit_config、bringup 应该分工清楚
- 大项目中 launch 分层很重要

今日产出：

- 有独立 bringup 包
- 能解释为什么不把所有 launch 都塞进一个包

#### Day 2：整理 launch 启动顺序

实现过程：

- 列出需要启动的组件
- 排序：robot_description -> ros2_control -> controllers -> MoveIt2 -> RViz2
- 将启动顺序写入 launch
- 必要时添加延迟或事件处理

知识点：

- 启动顺序会影响节点是否能拿到参数和服务
- 控制器必须在 controller_manager 存在后加载
- MoveIt2 需要机器人描述和控制器配置
- launch 不是简单堆节点，还要管理依赖关系

今日产出：

- 有清晰启动顺序
- 一条 launch 命令能启动大部分系统
- 能解释为什么某些节点必须先启动

#### Day 3：统一配置文件

实现过程：

- 整理 controllers.yaml、joint_limits.yaml、kinematics.yaml 等配置
- 确认文件位置
- 删除重复或过期配置
- README 中记录每个配置文件用途

知识点：

- 配置文件是系统行为的一部分
- 同一个参数散落多处会导致联调困难
- 文件命名应该能表达用途
- 配置路径要和 launch 中引用一致

今日产出：

- 配置文件结构清晰
- 重要配置都有说明
- 不依赖记忆也能找到参数来源

#### Day 4：完整联调

实现过程：

- 启动 `full_demo.launch.py`
- 检查 `/tf`
- 检查 `/joint_states`
- 检查 controller 状态
- 在 MoveIt2 中 Plan
- 尝试 Execute
- 记录所有失败点和对应日志

知识点：

- 完整联调要按链路分段排查
- 问题记录要写具体命令、现象和日志
- 一次联调失败不代表系统方向错，可能只是配置不一致
- 稳定复现比偶然成功更重要

今日产出：

- 至少一条完整链路能稳定跑，或有完整问题清单
- 能说清每个组件是否正常
- 能定位失败发生在哪一层

#### Day 5：README 完整复现步骤

实现过程：

- 写环境要求
- 写构建命令
- 写 source 命令
- 写完整启动命令
- 写单独调试命令
- 写常见问题和解决方式

知识点：

- README 复现步骤要按新机器视角写
- 命令不能只写自己记得的简写
- 常见问题要写错误现象和检查命令
- 项目展示依赖可复现性

今日产出：

- README 可以指导别人从零运行 demo
- 有清楚的 build、launch、debug 三类命令
- 能用 README 反向检查项目结构

#### Day 6：简历描述和面试讲解稿

实现过程：

- 写 2 到 3 条简历 bullet
- 每条包含技术栈、动作和结果
- 写 2 分钟项目讲解稿
- 准备 5 个可能被问的问题

知识点：

- 简历描述要体现你做了系统集成，不只是跟教程
- 面试讲解要按“控制库 -> ROS2 节点 -> 可视化 -> 规划 -> 控制执行”展开
- 不夸大没完成的部分，但要清楚说明设计思路

今日产出：

- 有可放进简历的项目描述
- 有面试讲解稿
- 能回答项目架构相关问题

#### Day 7：项目复盘和下一阶段计划

实现过程：

- 列出已完成链路
- 列出未完成或不稳定点
- 给每个问题标优先级
- 决定下一阶段是补稳定性、加功能还是写文档

知识点：

- 项目复盘要区分事实、问题和下一步
- 技术债要记录，不要靠脑子记
- 后续计划要服务项目展示，不要无限发散

今日产出：

- 有完整复盘清单
- 有下一阶段优先级
- 能判断项目是否达到简历展示标准

---


## 第 8 周：移动机器人定位导航 Nav2 入门

### 为什么要学移动机器人导航

你的主线是机械臂，但机器人岗位常常同时看 ROS2 基础、机械臂 MoveIt2、移动机器人 Nav2、TF、URDF、sensor 和 map。因此 Nav2 不需要现在做深，但应该建立系统认知。

本周目标：

- [ ] 理解移动机器人导航栈整体结构
- [ ] 理解 map、odom、base_link、laser_frame 的 TF 关系
- [ ] 理解 AMCL 定位
- [ ] 理解 costmap
- [ ] 理解 planner、controller、behavior tree
- [ ] 跑通一个 Nav2 仿真 demo 或至少完成架构笔记

核心链路：

```text
map
  -> localization / AMCL
  -> global costmap
  -> global planner
  -> local costmap
  -> controller
  -> cmd_vel
  -> mobile base
```

本周任务：

### Day 1：Nav2 总体架构

- [ ] 阅读 Nav2 概览
- 给机器人一个目标点，它根据地图、传感器、定位和规划算法，自动完成“定位 → 全局规划 → 局部避障控制 → 异常恢复 → 到达目标”的流程。
- 
- [ ] 画出 `map -> odom -> base_link` TF 树
- [ ] 理解 `cmd_vel` 的作用
- [ ] 理解 navigation goal 的输入
- [ ] 记录 Nav2 中 planner、controller、recoveries / behaviors 的职责

验收：

- [ ] 能解释定位、规划、控制三个词在移动机器人里的含义
- [ ] 能说明 Nav2 的输出为什么通常是 `/cmd_vel`

### Day 2：TF 与坐标系

- [ ] 学习 `map` 坐标系
- [ ] 学习 `odom` 坐标系
- [ ] 学习 `base_link` 坐标系
- [ ] 学习 `laser_frame` 或 camera frame
- [ ] 对比机械臂中的 `base_link`、`link1`、`link2`

验收：

- [ ] 能画出移动机器人 TF 树
- [ ] 能解释 `map -> odom` 和 `odom -> base_link` 的区别

### Day 3：定位 AMCL

- [ ] 学习 AMCL 的输入：map、laser scan、odom、initial pose
- [ ] 学习 AMCL 的输出：`map -> odom`
- [ ] 理解粒子滤波的大概思想
- [ ] 在笔记中写 AMCL 解决什么问题

验收：

- [ ] 能解释“定位”不是“导航”
- [ ] 能说明为什么机器人需要初始位姿

### Day 4：Costmap

- [ ] 学习 global costmap
- [ ] 学习 local costmap
- [ ] 学习 obstacle layer
- [ ] 学习 inflation layer
- [ ] 画出障碍物膨胀示意图

验收：

- [ ] 能解释为什么机器人不能贴着障碍物规划
- [ ] 能解释 global costmap 和 local costmap 的区别

### Day 5：Planner 和 Controller

- [ ] 学习 global planner 的输入输出
- [ ] 学习 local controller 的输入输出
- [ ] 理解路径 path 和速度 command 的区别
- [ ] 记录 Nav2 中 planner/controller 和 MoveIt2 planner/controller 的异同

验收：

- [ ] 能说明 planner 输出 path，不是直接输出电机控制
- [ ] 能说明 controller 负责跟踪路径并输出 `cmd_vel`

### Day 6：跑一个 Nav2 demo 或做完整架构笔记

如果环境允许：

- [ ] 启动 TurtleBot3 或 Nav2 demo
- [ ] 加载地图
- [ ] 设置 initial pose
- [ ] 设置 navigation goal
- [ ] 观察 global path 和 local behavior

如果环境暂时不允许：

- [ ] 完成 Nav2 架构笔记
- [ ] 整理常用 topic
- [ ] 整理常用 launch
- [ ] 整理常见问题

验收：

- [ ] 至少完成 demo 或一篇完整 Nav2 架构笔记

### Day 7：Nav2 和机械臂项目对比

- [ ] 写 Nav2 和 MoveIt2 对比表
- [ ] 写移动机器人和机械臂控制对比
- [ ] 写哪些知识可复用：TF、launch、参数、RViz2、action
- [ ] 写哪些知识不同：底盘速度控制、地图、定位、costmap

验收：

- [ ] 能用 1 分钟解释为什么机械臂项目也值得了解 Nav2

验收：

- [ ] 能画出 Nav2 架构图
- [ ] 能解释定位和导航的区别
- [ ] 能解释 `cmd_vel` 在移动机器人中的作用
- [ ] 能说明机械臂控制和移动机器人控制的共同点与区别

---

---


### 第 8 周每日详细执行指南：Nav2 移动机器人扩展

#### Day 1：Nav2 总体架构

实现过程：

- 学习 Nav2 的 planner、controller、behavior tree、costmap
- 画出输入输出链路
- 对比 MoveIt2 的规划执行链路

知识点：

- Nav2 面向移动机器人导航
- planner 输出路径，controller 输出速度命令
- behavior tree 负责组织导航行为
- Nav2 的核心输出通常是 `/cmd_vel`

今日产出：

- 能画出 Nav2 架构图
- 能解释 Nav2 和 MoveIt2 的相同点与不同点

#### Day 2：TF 与坐标系

实现过程：

- 学习 `map`、`odom`、`base_link`
- 画出移动机器人 TF 树
- 对比机械臂的 `base_link`、`link1`、`tool0`
- 记录 TF 断裂会造成的问题

知识点：

- `map` 是全局定位坐标系
- `odom` 是局部连续里程计坐标系
- `base_link` 是机器人本体坐标系
- TF 是机器人系统中所有空间信息的基础

今日产出：

- 能解释 `map -> odom -> base_link`
- 能说明机械臂 TF 和移动机器人 TF 的区别

#### Day 3：定位 AMCL

实现过程：

- 学习 AMCL 的输入输出
- 理解地图、激光、初始位姿的作用
- 记录 AMCL 发布的 TF
- 画出定位流程

知识点：

- AMCL 用粒子滤波估计机器人在地图中的位置
- 初始位姿帮助定位收敛
- 定位不是导航，定位只是知道自己在哪
- 传感器数据和地图匹配是定位核心

今日产出：

- 能解释 AMCL 的作用
- 能说明为什么机器人需要 initial pose

#### Day 4：Costmap

实现过程：

- 学习 global costmap
- 学习 local costmap
- 学习 obstacle layer
- 学习 inflation layer
- 画出障碍物膨胀示意图

知识点：

- costmap 把环境转换成规划可用的代价地图
- inflation 让机器人远离障碍物
- global costmap 用于全局规划
- local costmap 用于局部避障

今日产出：

- 能解释机器人为什么不能贴着障碍物规划
- 能解释 global costmap 和 local costmap 的区别

#### Day 5：Planner 和 Controller

实现过程：

- 学习 global planner 的输入输出
- 学习 local controller 的输入输出
- 对比 path 和 velocity command
- 记录 planner/controller 与 MoveIt2 的对应关系

知识点：

- planner 输出路径，不直接控制电机
- controller 跟踪路径并输出 `/cmd_vel`
- 路径是几何结果，速度命令是控制结果
- 移动机器人和机械臂都有规划层和控制层

今日产出：

- 能解释 planner 和 controller 的区别
- 能说明 Nav2 controller 和 MoveIt2 controller 的不同

#### Day 6：跑一个 Nav2 demo 或做完整架构笔记

实现过程：

- 如果环境允许，启动 TurtleBot3 或 Nav2 demo
- 加载地图
- 设置 initial pose
- 设置 navigation goal
- 观察 global path 和 local behavior
- 如果环境不允许，整理完整架构笔记

知识点：

- demo 运行要同时关注地图、定位、TF、costmap 和控制输出
- 导航失败可能来自定位、地图、TF、costmap 或 controller
- 没有环境时，架构笔记也可以形成有效学习成果

今日产出：

- 至少完成 demo 或完整 Nav2 架构笔记
- 能说出 Nav2 调试时先看哪些信息

#### Day 7：Nav2 和机械臂项目对比

实现过程：

- 写 Nav2 和 MoveIt2 对比表
- 写移动机器人和机械臂控制对比
- 整理可复用知识：TF、launch、参数、RViz2、action
- 整理不同点：地图、定位、costmap、cmd_vel

知识点：

- 机器人系统有共通知识，也有形态差异
- 机械臂重规划和关节轨迹
- 移动机器人重定位、地图和底盘速度控制
- 横向理解能增强机器人系统能力

今日产出：

- 能用 1 分钟解释为什么机械臂项目也值得了解 Nav2
- 有一张清晰对比表

---


## 第 9 周：YOLO、PyTorch 与 LibTorch 机器人感知扩展

### 是否需要学习 PyTorch / LibTorch

需要，但学习目标要明确：

- 你做过 YOLO，说明视觉项目经验可以成为优势。
- 对机器人方向来说，更重要的是“如何把视觉结果接入 ROS2 系统”。
- PyTorch 用于训练和 Python 推理。
- LibTorch 用于 C++ 部署和 ROS2 C++ 节点结合。

本周目标：

- [ ] 复习 PyTorch tensor、model、checkpoint、inference
- [ ] 理解 YOLO 输出：bbox、class、confidence
- [ ] 写一个 ROS2 Python image subscriber
- [ ] 写一个 YOLO ROS2 detection node 设计草图
- [ ] 初步了解 LibTorch C++ 推理流程
- [ ] 思考视觉检测如何和机械臂抓取任务连接

学习重点：

| 内容 | 学到什么程度 |
|---|---|
| Python | 会写 ROS2 Python 节点、图像订阅、结果发布 |
| PyTorch | 会加载模型、前处理、推理、后处理 |
| YOLO | 会解释输入输出和检测结果 |
| LibTorch | 了解 C++ 加载 TorchScript 模型的流程 |
| OpenCV | 会做图像读取、显示、坐标绘制 |
| ROS2 image pipeline | 理解 `sensor_msgs/Image`、`cv_bridge` |

和机械臂项目的连接：

```text
camera image
  -> YOLO detection
  -> object bbox
  -> target center
  -> camera coordinate estimate
  -> TF transform to robot base
  -> MoveIt2 grasp planning
```

本周验收：

- [ ] 能解释 PyTorch 和 LibTorch 的区别
- [ ] 能说明 YOLO 检测结果如何发布成 ROS2 topic
- [ ] 能写出视觉到机械臂抓取的系统框图
- [ ] 能判断哪些部分现在做，哪些部分以后再做

### Day 1：复盘已有 YOLO 项目

- [ ] 写清楚自己做过的 YOLO 项目输入是什么
- [ ] 写清楚输出是什么：bbox、class、confidence
- [ ] 写清楚训练、验证、推理流程
- [ ] 整理模型文件、数据集、评价指标
- [ ] 写“这个经验如何迁移到机器人”

验收：

- [ ] 能用 1 分钟讲清楚自己的 YOLO 项目
- [ ] 能说明检测结果如何变成机器人可用的信息

### Day 2：PyTorch 推理复习

- [ ] 复习 tensor shape
- [ ] 复习 `model.eval()`
- [ ] 复习 `torch.no_grad()`
- [ ] 复习 checkpoint 加载
- [ ] 复习图像前处理：resize、normalize、to tensor
- [ ] 复习后处理：NMS、阈值过滤

验收：

- [ ] 能写一个最小 PyTorch inference 脚本
- [ ] 能解释训练模式和推理模式区别

### Day 3：ROS2 Python 图像节点

- [ ] 创建或设计 `robot_vision_learning`
- [ ] 写 `image_subscriber.py`
- [ ] 订阅 `sensor_msgs/msg/Image`
- [ ] 使用 `cv_bridge` 转 OpenCV 图像
- [ ] 打印图像尺寸
- [ ] 可选：显示图像窗口

验收：

- [ ] 能订阅图像 topic
- [ ] 能解释 ROS image message 和 OpenCV image 的区别

### Day 4：YOLO ROS2 detection node 草稿

- [ ] 设计 `yolo_ros2_node.py`
- [ ] 订阅 image
- [ ] 前处理图像
- [ ] 调用 YOLO 模型推理
- [ ] 后处理得到 bbox
- [ ] 发布检测结果
- [ ] 可选：发布带框图像

输出 topic 设计：

```text
/detections
/debug_image
```

验收：

- [ ] 能画出 image -> detection 的节点图
- [ ] 能说明检测结果应该用什么消息表达：自定义 msg、数组、vision_msgs

### Day 5：视觉结果如何接机械臂

- [ ] 理解 bbox 中心点只是图像坐标
- [ ] 学习相机内参概念
- [ ] 学习深度信息或单目估计的限制
- [ ] 学习 camera frame 到 robot base 的 TF
- [ ] 设计“检测目标 -> MoveIt2 抓取目标”的流程

验收：

- [ ] 能说明 2D bbox 不能直接等于机械臂抓取位姿
- [ ] 能画出 camera -> base_link -> end_effector 的转换链路

### Day 6：LibTorch 入门了解

- [ ] 理解 PyTorch 和 LibTorch 的关系
- [ ] 理解 TorchScript
- [ ] 学习 `torch.jit.trace` 或 `torch.jit.script`
- [ ] 了解 C++ 加载 `.pt` 模型的流程
- [ ] 记录 LibTorch 部署难点：依赖、CMake、CUDA、版本匹配

验收：

- [ ] 能说清楚什么时候用 Python 推理，什么时候考虑 LibTorch
- [ ] 不要求本周完整部署 LibTorch

### Day 7：感知扩展复盘

- [ ] 写 PyTorch / YOLO / LibTorch 对比表
- [ ] 写 ROS2 视觉节点 topic 设计
- [ ] 写机械臂视觉抓取扩展路线
- [ ] 决定是否把视觉部分作为项目加分项，而不是主线

验收：

- [ ] 能把 YOLO 经验和 ROS2 机械臂项目连起来讲
- [ ] 能明确当前阶段不被视觉部分拖慢主线

---

---


### 第 9 周每日详细执行指南：YOLO、PyTorch 与 LibTorch 感知扩展

#### Day 1：复盘已有 YOLO 项目

实现过程：

- 写清已有 YOLO 项目的输入
- 写清输出：bbox、class、confidence
- 记录训练、验证、推理流程
- 整理模型文件和数据集
- 思考检测结果如何给机器人使用

知识点：

- 检测模型输出的是图像坐标信息
- bbox 不能直接等于机器人抓取位姿
- 机器人需要把视觉结果转换到空间坐标系
- 已有项目经验要转化为机器人系统能力

今日产出：

- 能 1 分钟讲清自己的 YOLO 项目
- 能说明检测结果如何变成机器人可用信息

#### Day 2：PyTorch 推理复习

实现过程：

- 复习 tensor shape
- 复习 `model.eval()`
- 复习 `torch.no_grad()`
- 加载 checkpoint
- 写最小 inference 脚本
- 整理前处理和后处理流程

知识点：

- `eval()` 会切换推理模式
- `no_grad()` 会关闭梯度计算
- 图像前处理通常包括 resize、normalize、to tensor
- 后处理通常包括阈值过滤和 NMS

今日产出：

- 有一个最小 PyTorch inference 脚本
- 能解释训练模式和推理模式的区别

#### Day 3：ROS2 Python 图像节点

实现过程：

- 创建或设计 `robot_vision_learning`
- 写 `image_subscriber.py`
- 订阅 `sensor_msgs/msg/Image`
- 使用 `cv_bridge` 转 OpenCV 图像
- 打印图像尺寸
- 可选：显示图像窗口

知识点：

- ROS image message 是消息格式
- OpenCV image 是图像处理数组
- `cv_bridge` 负责两者转换
- Python 节点适合快速集成视觉模型

今日产出：

- 能订阅图像 topic
- 能获得 OpenCV 图像
- 能解释 ROS 图像消息和 OpenCV 图像的区别

#### Day 4：YOLO ROS2 detection node 草稿

实现过程：

- 设计 `yolo_ros2_node.py`
- 订阅 image
- 前处理图像
- 调用 YOLO 模型推理
- 后处理得到 bbox
- 发布检测结果
- 可选：发布带框 debug image

知识点：

- 感知节点一般是 image -> inference -> detections
- 检测结果可以用自定义 msg、数组或 `vision_msgs`
- debug image 方便观察模型效果
- 推理速度会影响实时性

今日产出：

- 有 YOLO ROS2 节点草稿
- 能画出 image -> detection 的节点图
- 能说明检测结果消息该怎么设计

#### Day 5：视觉结果如何接机械臂

实现过程：

- 理解 bbox 中心点只是图像坐标
- 学习相机内参概念
- 学习深度信息或单目估计限制
- 学习 camera frame 到 robot base 的 TF
- 设计检测目标到 MoveIt2 抓取目标的流程

知识点：

- 2D 图像坐标不能直接作为 3D 抓取点
- 需要相机标定和深度估计
- TF 用于坐标系转换
- 抓取任务还需要姿态、碰撞和夹爪策略

今日产出：

- 能解释 bbox 为什么不能直接控制机械臂
- 能画出 camera -> base_link -> end_effector 的转换链路

#### Day 6：LibTorch 入门了解

实现过程：

- 理解 PyTorch 和 LibTorch 的关系
- 学习 TorchScript
- 了解 `torch.jit.trace` 和 `torch.jit.script`
- 了解 C++ 加载 `.pt` 模型流程
- 记录部署难点：依赖、CMake、CUDA、版本匹配

知识点：

- PyTorch 更适合训练和快速验证
- LibTorch 用于 C++ 推理部署
- TorchScript 是模型从 Python 到 C++ 的桥梁
- LibTorch 部署成本比 Python 高

今日产出：

- 能说明什么时候用 Python 推理，什么时候考虑 LibTorch
- 不要求本周完整部署 LibTorch

#### Day 7：感知扩展复盘

实现过程：

- 写 PyTorch、YOLO、LibTorch 对比表
- 写 ROS2 视觉节点 topic 设计
- 写机械臂视觉抓取扩展路线
- 决定视觉部分是否作为加分项

知识点：

- 感知扩展不能拖慢主线控制项目
- 加分项要有清晰边界
- 视觉到抓取是系统链路，不是单模型问题

今日产出：

- 能把 YOLO 经验和 ROS2 机械臂项目连接起来讲
- 明确当前阶段不被视觉部分拖慢主线

---


## 第 10 周：C++ 数据结构算法、现代 C++ 与编码风格

### 是否需要学数据结构和算法

需要，但不需要一开始刷很难的算法题。机器人项目中最常用的是：

- `vector`
- `deque`
- `queue`
- `map` / `unordered_map`
- `priority_queue`
- 图搜索
- BFS / DFS
- Dijkstra / A*
- 简单排序和查找
- 数值计算中的矩阵、向量、插值

对机器人方向，优先级最高的是：

1. 能写清楚数据结构的用途
2. 能读懂算法复杂度
3. 能实现 A*、Dijkstra、RRT 的基础版本
4. 能在项目里解释为什么选这个结构

### 现代 C++ 必学内容

| 内容 | 项目用途 |
|---|---|
| `auto` | 简化复杂类型 |
| range-for | 遍历容器 |
| `nullptr` | 替代 `NULL` |
| `enum class` | 更安全的枚举 |
| `std::unique_ptr` | 独占资源 |
| `std::shared_ptr` | ROS2 中广泛使用 |
| lambda | 替代部分 `std::bind` |
| `std::function` | 保存回调 |
| move semantics | 避免不必要拷贝 |
| `const` correctness | 接口设计 |
| RAII | 资源生命周期管理 |
| `optional` / `variant` | 表达可能为空或多类型结果 |

### C++ 编写风格

项目中应逐步建立这些习惯：

- [ ] 类名使用 `PascalCase`
- [ ] 函数名使用 `snake_case` 或项目统一风格
- [ ] 成员变量使用 `_` 后缀，例如 `timer_`
- [ ] 头文件只放声明，源文件放实现
- [ ] public 接口少而清晰
- [ ] 查询函数加 `const`
- [ ] 大对象参数用 `const T&`
- [ ] 避免裸 `new/delete`，优先智能指针和标准容器
- [ ] CMake 用 target 风格
- [ ] 每个模块有最小测试
- [ ] README 写清楚运行方式

本周任务：

### Day 1：项目代码风格体检

- [ ] 检查类名是否统一
- [ ] 检查函数名是否统一
- [ ] 检查成员变量是否使用 `_` 后缀
- [ ] 检查头文件 include guard 或 `#pragma once`
- [ ] 检查 public / private 是否合理
- [ ] 检查是否有过长函数
- [ ] 列出需要重构的点

验收：

- [ ] 得到一份 C++ 风格问题清单
- [ ] 能说出自己项目采用的命名规范

### Day 2：const correctness 和参数传递

- [ ] 检查查询函数是否加 `const`
- [ ] 检查大对象参数是否用 `const T&`
- [ ] 检查需要修改的参数是否用 `T&`
- [ ] 检查返回值是否有悬空引用风险
- [ ] 修改 `RobotState`、`SafetyLimiter`、`TrajectoryBuffer` 中不合理接口

验收：

- [ ] 能解释为什么 `positions() const` 很重要
- [ ] 能解释 `const T&` 和 `T` 传参区别

### Day 3：现代 C++ 回调写法

- [ ] 复习 `std::bind`
- [ ] 复习 lambda
- [ ] 把一个 timer callback 改写成 lambda
- [ ] 把一个 subscriber callback 改写成 lambda
- [ ] 对比可读性
- [ ] 记录 ROS2 中 `SharedPtr` 的常见写法

验收：

- [ ] 能写出 `std::bind(&Class::func, this, _1)`
- [ ] 能写出等价 lambda
- [ ] 能解释 lambda 捕获 `[this]`

### Day 4：智能指针和 RAII

- [ ] 复习 `unique_ptr`
- [ ] 复习 `shared_ptr`
- [ ] 复习 `weak_ptr`
- [ ] 检查项目中是否有裸 `new/delete`
- [ ] 用标准容器或智能指针替代不必要的手动内存管理
- [ ] 记录 ROS2 为什么大量使用 `SharedPtr`

验收：

- [ ] 能解释所有权
- [ ] 能解释 `unique_ptr` 和 `shared_ptr` 的区别
- [ ] 能解释 RAII 为什么能减少资源泄漏

### Day 5：数据结构和控制项目对应

- [ ] 复习 `vector`
- [ ] 复习 `deque`
- [ ] 复习 `queue`
- [ ] 复习 `map` / `unordered_map`
- [ ] 复习 `priority_queue`
- [ ] 写一张表：每种结构在机器人项目中适合放什么

示例：

| 数据结构 | 项目用途 |
|---|---|
| `vector` | 关节位置、速度、轨迹点数组 |
| `deque` | 轨迹缓存 |
| `queue` | 消息队列、任务队列 |
| `unordered_map` | joint name 到 index 的映射 |
| `priority_queue` | A* open set |

验收：

- [ ] 能解释为什么 `TrajectoryBuffer` 适合用 `deque`
- [ ] 能解释 joint name 映射为什么适合用 map

### Day 6：算法小 demo：A* 或 Dijkstra

- [ ] 选择 A* 或 Dijkstra
- [ ] 写一个 2D grid map
- [ ] 设置起点、终点、障碍物
- [ ] 输出路径
- [ ] 使用 `priority_queue`
- [ ] 记录算法复杂度和适用场景

验收：

- [ ] demo 能运行
- [ ] 能解释 open set、closed set
- [ ] 能说明 A* 和 Dijkstra 的区别
- [ ] 能把它和 Nav2 global planner 联系起来

### Day 7：C++ 工程复盘和重构提交

- [ ] 整理本周 C++ 修改点
- [ ] 更新 README 或代码注释
- [ ] 整理 C++ 面试问题与项目对应表
- [ ] 提交一次代码重构
- [ ] 写“我的 C++ 编码规范”小节

验收：

- [ ] 项目代码比重构前更清楚
- [ ] 能从项目里举例说明现代 C++ 用法

验收：

- [ ] 能解释 `shared_ptr` 为什么在 ROS2 中常见
- [ ] 能解释 lambda 和 `std::bind` 的关系
- [ ] 能解释 `vector`、`deque`、`queue` 在项目中分别适合哪里
- [ ] 能说出自己的 C++ 编码规范

---

---


### 第 10 周每日详细执行指南：C++ 工程强化

#### Day 1：项目代码风格体检

实现过程：

- 检查类名是否统一
- 检查函数命名是否统一
- 检查成员变量命名是否统一
- 检查头文件是否有 include guard 或 `#pragma once`
- 检查 public/private 是否合理
- 列出需要重构的点

知识点：

- 统一风格比个人偏好更重要
- public 接口越少，类越容易维护
- 头文件应该尽量少暴露实现细节
- 风格体检是重构前的准备工作

今日产出：

- 得到一份 C++ 风格问题清单
- 能说出项目采用的命名规范

#### Day 2：const correctness 和参数传递

实现过程：

- 检查查询函数是否加 `const`
- 检查大对象参数是否用 `const T&`
- 检查需要修改的参数是否用 `T&`
- 检查返回引用是否有悬空风险
- 修改 `RobotState`、`SafetyLimiter`、`TrajectoryBuffer` 中不合理接口

知识点：

- `const` 可以表达函数不会修改对象
- `const T&` 避免不必要拷贝
- 返回引用要确保被引用对象生命周期足够长
- const correctness 是 C++ 接口质量的重要部分

今日产出：

- 核心类接口更安全
- 能解释 `positions() const` 为什么重要
- 能解释值传递和引用传递的区别

#### Day 3：现代 C++ 回调写法

实现过程：

- 复习 `std::bind`
- 复习 lambda
- 把一个 timer callback 改成 lambda
- 把一个 subscriber callback 改成 lambda
- 对比可读性

知识点：

- `std::bind` 可以绑定成员函数和参数
- lambda 更直观，适合局部回调
- `[this]` 表示捕获当前对象
- ROS2 中两种写法都常见

今日产出：

- 能写出 `std::bind(&Class::func, this, _1)`
- 能写出等价 lambda
- 能解释 lambda 捕获 `[this]`

#### Day 4：智能指针和 RAII

实现过程：

- 复习 `unique_ptr`
- 复习 `shared_ptr`
- 复习 `weak_ptr`
- 检查项目中是否有裸 `new/delete`
- 用标准容器或智能指针替代不必要手动内存管理
- 记录 ROS2 为什么大量使用 `SharedPtr`

知识点：

- RAII 让资源生命周期绑定对象生命周期
- `unique_ptr` 表达独占所有权
- `shared_ptr` 表达共享所有权
- `weak_ptr` 避免循环引用
- 现代 C++ 应尽量避免裸 `new/delete`

今日产出：

- 能解释所有权
- 能解释 `unique_ptr` 和 `shared_ptr` 的区别
- 能解释 RAII 为什么能减少资源泄漏

#### Day 5：数据结构和控制项目对应

实现过程：

- 复习 `vector`
- 复习 `deque`
- 复习 `queue`
- 复习 `map` 和 `unordered_map`
- 复习 `priority_queue`
- 写一张表，对应机器人项目中的使用场景

知识点：

- `vector` 适合关节数组
- `deque` 适合轨迹缓存
- `queue` 适合任务队列
- `unordered_map` 适合 joint name 到 index 映射
- `priority_queue` 适合 A* open set

今日产出：

- 能解释为什么 `TrajectoryBuffer` 适合用 `deque`
- 能解释 joint name 映射为什么适合用 map
- 有一张数据结构和机器人场景对应表

#### Day 6：算法小 demo：A* 或 Dijkstra

实现过程：

- 选择 A* 或 Dijkstra
- 创建 2D grid map
- 设置起点、终点和障碍物
- 使用 `priority_queue`
- 输出路径
- 记录复杂度和适用场景

知识点：

- Dijkstra 适合无启发式最短路
- A* 在 Dijkstra 基础上加入启发函数
- open set 保存待探索节点
- closed set 保存已处理节点
- Nav2 global planner 和这些图搜索思想有关

今日产出：

- demo 能运行
- 能解释 open set 和 closed set
- 能说清 A* 和 Dijkstra 的区别

#### Day 7：C++ 工程复盘和重构提交

实现过程：

- 整理本周 C++ 修改点
- 更新 README 或代码注释
- 整理 C++ 面试问题与项目对应表
- 提交一次重构
- 写“我的 C++ 编码规范”小节

知识点：

- 重构要有目标，不是随便改代码
- 每次重构后都要跑测试
- 面试问题最好能从项目中举例回答
- 编码规范要服务可维护性

今日产出：

- 项目代码比重构前更清楚
- README 记录了关键设计
- 能从项目中举例说明现代 C++ 用法

---

## 第 11 周：嵌入式 Linux / STM32 / micro-ROS 求职加分扩展

### 本周定位

这一周不是把方向改成纯嵌入式，而是给机器人项目补上“上位机 ROS2 + 下位机控制器”的系统理解。

主线仍然是：

```text
C++ + ROS2 + 机械臂控制 + MoveIt2 + ros2_control
```

嵌入式扩展的定位是：

```text
求职加分项：
理解 Linux 应用开发、串口/CAN 通信、STM32 下位机、实时控制任务，以及它们如何接入 ROS2 系统。
```

不要一开始就钻太深：

- 不急着学 Linux 内核驱动
- 不急着学 Yocto 深度裁剪
- 不急着写复杂 bootloader
- 不急着啃 FreeRTOS 内核源码
- 不急着把 STM32 所有外设都学一遍

本周目标是形成一条能讲清楚、能做 demo、能写进简历的链路：

```text
ROS2 上位机
  -> 串口 / CAN / UDP
  -> STM32 或模拟下位机
  -> 电机命令 / 编码器状态
  -> 回传 joint state
  -> ROS2 可视化或控制闭环
```

### 树莓派 4B 在本项目中的角色

手里有树莓派 4B 时，最推荐把它当成机器人上位机或边缘计算机，而不是当成 STM32 那种硬实时下位机。

推荐分工：

```text
电脑 / 笔记本
  -> 代码开发
  -> SSH 远程登录树莓派
  -> RViz2 可视化
  -> MoveIt2 调试

Raspberry Pi 4B
  -> 运行 ROS2 控制节点
  -> 连接 USB 摄像头 / CSI 摄像头
  -> 通过 UART / CAN / UDP 连接 STM32 或模拟下位机
  -> 发布 /joint_states、/tf、/image_raw
  -> 记录 rosbag2 和运行日志

STM32 / 电机控制板
  -> 读取编码器
  -> 输出 PWM / CAN 电机命令
  -> 执行实时控制任务
  -> 回传关节状态
```

最适合用树莓派 4B 做的项目：

- [ ] 作为 ROS2 实机部署平台，运行 `vel_to_pos_node`、`joint_state_bridge`、`robot_state_publisher`
- [ ] 作为 ROS2 与 STM32 的通信桥，完成命令下发和状态回传
- [ ] 接 USB/CSI 相机，发布 `/image_raw`
- [ ] 做 OpenCV、AprilTag、ArUco 或轻量视觉检测
- [ ] 做小车 / Nav2 上位机，为后续移动机器人扩展做准备
- [ ] 练习 SSH、systemd、rosbag2、固定 IP、launch 一键启动等工程能力

不建议一开始做的事：

- [ ] 不要让树莓派直接承担高频硬实时电机闭环
- [ ] 不要一开始就在树莓派上跑完整桌面 + RViz2 + 大型 MoveIt2 项目
- [ ] 不要在树莓派 4B 上强行跑大型 YOLO 实时检测
- [ ] 不要一开始研究 Linux 内核驱动或 Yocto

推荐系统选择：

```text
如果重点是 ROS2：
Ubuntu Server 24.04 ARM64 + ROS2 Jazzy
或 Ubuntu Server 22.04 ARM64 + ROS2 Humble

如果重点是 GPIO / 相机 / Linux 基础：
Raspberry Pi OS 64-bit
```

本项目更推荐优先使用 Ubuntu Server + ROS2，因为后续和 ROS2 机械臂、MoveIt2、ros2_control 的环境更一致。

### 本周核心目标

- [ ] 理解机器人系统中的上位机 / 下位机分工
- [ ] 学会 Linux 下串口或 Socket 基础通信
- [ ] 了解 STM32 常用外设：GPIO、Timer、PWM、Encoder、UART、CAN
- [ ] 理解 FreeRTOS 中 task、queue、semaphore、控制周期
- [ ] 设计 ROS2 与 STM32 的通信协议
- [ ] 完成一个“ROS2 节点 + 模拟下位机”的通信 demo
- [ ] 如果有 STM32 板子，再迁移到真实硬件
- [ ] 了解 micro-ROS 的作用和适用边界
- [ ] 把树莓派 4B 配置成 ROS2 实机部署平台
- [ ] 在树莓派上运行至少一个 ROS2 控制或通信节点

### 求职定位

这部分适合支撑这些岗位关键词：

```text
机器人软件工程师
ROS2 开发工程师
机器人控制工程师
嵌入式机器人开发
运动控制工程师
AGV / AMR / 机械臂系统开发
```

简历表达方向：

```text
了解机器人上位机与下位机控制架构，使用 ROS2 完成控制命令下发和关节状态回传，
设计串口/CAN 通信协议，模拟 STM32 下位机完成电机命令解析、状态反馈和基础闭环控制。
```

### Day 1：机器人中的上位机 / 下位机架构

目标：先建立系统分层认知，不急着写代码。

任务：

- [ ] 画出上位机 / 下位机 / 电机驱动 / 传感器之间的关系
- [ ] 区分 ROS2 节点、控制器、驱动器、MCU 的职责
- [ ] 整理机械臂项目中哪些逻辑适合放 ROS2，哪些适合放 STM32
- [ ] 对比 `ros2_control` 和真实硬件下位机的关系
- [ ] 把树莓派 4B 放进系统架构图，明确它作为 ROS2 上位机的位置
- [ ] 写一段“为什么机器人项目需要嵌入式知识”的说明

推荐架构图：

```text
Laptop / Desktop
  -> RViz2 / MoveIt2 / SSH / 开发调试
  -> Wi-Fi / Ethernet
  -> Raspberry Pi 4B
  -> ROS2 控制节点 / serial_bridge_node / camera_node
  -> joint trajectory / joint command
  -> ROS2 hardware interface 或通信节点
  -> 串口 / CAN / EtherCAT / UDP
  -> STM32 / 电机控制板
  -> 电机驱动器
  -> 编码器 / 电流 / IMU
  -> 状态回传
  -> /joint_states
```

知识点：

- 上位机通常负责规划、任务逻辑、可视化、参数管理
- 下位机通常负责实时控制、采样、电机驱动、保护逻辑
- ROS2 不适合直接做所有硬实时控制
- STM32 适合处理周期稳定、靠近硬件的任务
- `ros2_control` 可以把上层轨迹和底层硬件接口标准化

验收：

- [ ] 能解释“上位机”和“下位机”的区别
- [ ] 能画出 ROS2 到 STM32 的控制链路
- [ ] 能说明哪些部分应该放在 ROS2，哪些部分应该放在 MCU
- [ ] 能说明树莓派和笔记本、STM32 分别负责什么

### Day 2：Linux 应用开发基础和串口通信

目标：先在 Linux 用户态理解“上位机如何和设备通信”。

任务：

- [ ] 复习 Linux 文件设备概念
- [ ] 理解 `/dev/ttyUSB0`、`/dev/ttyACM0`
- [ ] 学习串口参数：baudrate、data bits、stop bits、parity
- [ ] 写一个 C++ 串口发送程序
- [ ] 写一个 C++ 串口接收程序
- [ ] 如果没有真实串口，用伪终端或 Python 脚本模拟
- [ ] 在树莓派 4B 上确认串口设备或 USB 串口设备名称
- [ ] 用 SSH 登录树莓派，完成一次远程编译和运行

最小通信协议示例：

```text
上位机发送：
CMD q1 q2 q3 q4 q5 q6

下位机回传：
STATE q1 q2 q3 q4 q5 q6 dq1 dq2 dq3 dq4 dq5 dq6
```

知识点：

- Linux 中很多硬件设备以文件形式暴露
- 串口通信本质是字节流，不天然知道一条消息在哪里结束
- 通信协议要设计帧头、字段、分隔符或校验
- 串口调试要关注权限、波特率、换行符和阻塞读取

验收：

- [ ] 能解释 `/dev/ttyUSB0` 是什么
- [ ] 能写出最小串口读写流程
- [ ] 能说明为什么通信协议需要消息边界
- [ ] 能通过 SSH 在树莓派上运行一个最小通信程序

### Day 3：ROS2 通信桥节点设计

目标：把 Linux 通信程序升级成 ROS2 节点。

任务：

- [ ] 创建 `embedded_bridge_node.cpp`
- [ ] 订阅 `/joint_position_cmd`
- [ ] 将 ROS2 消息编码成串口协议
- [ ] 从串口读取下位机状态
- [ ] 解析状态并发布 `/joint_states`
- [ ] 如果暂时没有硬件，写一个 `fake_mcu.py` 或 `fake_mcu.cpp`
- [ ] 将 `embedded_bridge_node` 部署到树莓派 4B 上运行
- [ ] 电脑端运行 RViz2，树莓派端发布 `/joint_states`

节点结构：

```text
Laptop / Desktop
  -> RViz2
  -> ros2 topic pub /joint_position_cmd

Raspberry Pi 4B
  -> embedded_bridge_node
  -> serial write: CMD ...

/joint_position_cmd
  -> embedded_bridge_node
  -> serial write: CMD ...

serial read: STATE ...
  -> embedded_bridge_node
  -> /joint_states
```

知识点：

- ROS2 节点可以作为上位机通信桥
- 上层控制逻辑不应该直接关心串口细节
- 通信桥节点负责协议编码、解码、异常处理
- `/joint_states` 是标准状态接口，适合连接 RViz2 和 MoveIt2

验收：

- [ ] 能用 ROS2 topic 触发一条下发命令
- [ ] 能从模拟下位机收到状态
- [ ] 能把状态发布成 `/joint_states`
- [ ] 能解释 bridge node 的职责
- [ ] 能说明为什么 RViz2 可以放在电脑上，而控制节点可以放在树莓派上

### Day 4：STM32 基础外设学习路线

目标：明确 STM32 要学哪些，不陷入“所有外设都学”的坑。

任务：

- [ ] 安装或了解 STM32CubeIDE / STM32CubeMX
- [ ] 了解 STM32 工程结构：startup、HAL、main、interrupt
- [ ] 学习 GPIO 输出和输入
- [ ] 学习 Timer 基础
- [ ] 学习 PWM 输出
- [ ] 学习 UART 收发
- [ ] 学习 Encoder mode 或编码器读取思路
- [ ] 了解 CAN 的作用和基本报文结构

优先级：

| 外设 | 机器人项目用途 | 优先级 |
|---|---|---|
| GPIO | 使能、方向、限位开关 | 高 |
| Timer | 控制周期、PWM 基础 | 高 |
| PWM | 电机驱动命令 | 高 |
| Encoder | 关节位置 / 速度反馈 | 高 |
| UART | 上位机通信、调试 | 高 |
| CAN | 电机驱动器通信 | 中高 |
| ADC | 电流、电压、传感器采样 | 中 |
| I2C / SPI | IMU、磁编码器、外设芯片 | 中 |
| DMA | 高效通信和采样 | 后续 |

知识点：

- STM32 是下位机控制器，不是 ROS2 的替代品
- HAL 可以帮助快速上手，但要理解外设背后的基本机制
- Timer 是嵌入式控制周期的核心
- PWM 和编码器是电机控制最常见输入输出
- CAN 在机器人电机驱动器中很常见

验收：

- [ ] 能说出 STM32 最该优先学哪些外设
- [ ] 能解释 PWM 和 Encoder 在电机控制中的作用
- [ ] 能说明 UART 和 CAN 分别适合什么通信场景

### Day 5：FreeRTOS 和实时控制任务

目标：理解“为什么下位机控制要关心实时性”。

任务：

- [ ] 了解裸机循环和 RTOS 任务的区别
- [ ] 学习 FreeRTOS task
- [ ] 学习 queue
- [ ] 学习 semaphore / mutex 的基本用途
- [ ] 设计一个 100 Hz 控制任务
- [ ] 设计一个通信接收任务
- [ ] 设计一个状态发送任务

推荐任务划分：

```text
control_task      100 Hz  读取目标，计算输出，更新电机命令
comm_rx_task       50 Hz  接收上位机命令
comm_tx_task       50 Hz  回传关节状态
safety_task       100 Hz  检查限位、急停、超速
```

知识点：

- 实时性不是“速度快”，而是“周期稳定、响应可预期”
- 控制任务应该有固定周期
- 通信任务和控制任务应该解耦
- queue 适合任务之间传递数据
- mutex 用于保护共享资源，但控制任务中要谨慎使用阻塞

验收：

- [ ] 能解释 task、queue、semaphore 的作用
- [ ] 能设计一个简单机器人下位机任务结构
- [ ] 能说明为什么通信和控制最好不要写在一个大 while 里

### Day 6：micro-ROS 认知和适用边界

目标：知道 micro-ROS 是什么，以及什么时候该用、什么时候不该用。

任务：

- [ ] 阅读 micro-ROS 基本介绍
- [ ] 理解 micro-ROS agent 和 client 的关系
- [ ] 理解 MCU 上发布 / 订阅 ROS2 消息的基本思路
- [ ] 对比“自定义串口协议”和“micro-ROS”
- [ ] 判断当前机械臂项目是否需要 micro-ROS

对比：

| 方案 | 优点 | 缺点 | 适用场景 |
|---|---|---|---|
| 自定义串口 / CAN 协议 | 简单、可控、资源占用小 | 需要自己设计协议和解析 | 简单电机控制、状态回传 |
| micro-ROS | 和 ROS2 概念一致，支持 pub/sub | 移植和资源要求更高 | MCU 需要深度接入 ROS2 生态 |
| ros2_control hardware interface | 上层标准化好 | 需要写硬件接口 | 机械臂控制器接入 MoveIt2 |

知识点：

- micro-ROS 是把 ROS2 能力扩展到微控制器的一种方案
- micro-ROS 不等于所有项目都必须使用
- 简单下位机控制常常自定义协议更直接
- 如果 MCU 需要直接成为 ROS2 图中的节点，可以考虑 micro-ROS

验收：

- [ ] 能解释 micro-ROS 是什么
- [ ] 能说明 micro-ROS 和普通串口协议的区别
- [ ] 能判断当前项目先用哪种方案更合适

### Day 7：嵌入式扩展复盘和简历表达

目标：把嵌入式学习收束成求职可讲的项目能力。

任务：

- [ ] 整理 ROS2 上位机和 STM32 下位机架构图
- [ ] 整理通信协议设计
- [ ] 整理 fake MCU demo 或真实 STM32 demo 的运行步骤
- [ ] 整理 Raspberry Pi 4B 部署步骤：系统、SSH、ROS2、节点运行、日志查看
- [ ] 写 README 中的“嵌入式扩展”章节
- [ ] 写简历 bullet
- [ ] 准备面试讲解稿

README 建议结构：

```text
Embedded Extension
├── System Architecture
├── Raspberry Pi 4B Deployment
├── Communication Protocol
├── ROS2 Bridge Node
├── Fake MCU Demo
├── STM32 Migration Plan
└── Known Limitations
```

简历 bullet 示例：

```text
基于 Raspberry Pi 4B 搭建 ROS2 机器人上位机，设计 ROS2 与 STM32 下位机的通信链路，
使用自定义串口/CAN 协议完成关节命令下发、编码器状态回传和 /joint_states 发布，
形成从运动规划到下位机控制的机器人系统闭环。
```

面试讲解主线：

1. 上层 MoveIt2 或控制节点生成关节目标
2. 树莓派 4B 上的 ROS2 bridge node 将目标编码成通信协议
3. 下位机解析命令并执行周期控制
4. 下位机读取编码器并回传状态
5. ROS2 发布 `/joint_states`，供 RViz2、MoveIt2 和监控节点使用

验收：

- [ ] 能用 1 分钟讲清楚为什么补嵌入式
- [ ] 能用 1 分钟讲清楚树莓派 4B 在项目中的作用
- [ ] 能画出 ROS2 到 STM32 的控制链路
- [ ] 能说明当前实现、后续可扩展点和不足
- [ ] 有一段可以写进简历的嵌入式扩展描述

---


## 进阶内容优先级

如果时间有限，按这个顺序推进：

1. MoveIt2 基础规划
2. ros2_control + joint_trajectory_controller
3. 现代 C++ 和项目重构
4. Nav2 架构理解
5. PyTorch / YOLO ROS2 感知链路
6. 嵌入式 Linux / STM32 / micro-ROS 扩展
7. LibTorch C++ 推理
8. 更深入的数据结构算法

原因：

- MoveIt2 和 ros2_control 最贴近机械臂控制项目。
- 现代 C++ 会直接提升代码质量。
- Nav2 和视觉是横向扩展，适合做机器人系统认知。
- 嵌入式扩展能补上机器人上位机 / 下位机闭环，对求职有加分价值。
- LibTorch 价值高，但部署成本也高，不应压过主线。

---

## 学习资源选择

每个方向只选“一个主线资源 + 官方文档 + 项目实作”，避免资料过载。

| 方向 | 学习方式 |
|---|---|
| ROS2 基础 | 官方 tutorials + 自己写节点 |
| MoveIt2 | MoveIt2 tutorials + 自己的 simple arm 配置 |
| ros2_control | 官方 demos + 自己的 joint trajectory controller |
| Nav2 | Nav2 getting started + 架构笔记 |
| PyTorch / YOLO | 复用已有 YOLO 经验，补 ROS2 图像节点 |
| Raspberry Pi 4B | Ubuntu Server + ROS2 实机部署 + SSH/systemd/rosbag2 工程实践 |
| 嵌入式 Linux / STM32 | 树莓派串口/CAN 通信 + STM32 外设基础 + ROS2 bridge demo |
| micro-ROS | 先理解概念和适用边界，不急着替代自定义通信协议 |
| LibTorch | 先了解 TorchScript C++ 推理，不急着完整部署 |
| 现代 C++ | Effective C++ / 侯捷课程 + 项目重构 |
| 数据结构算法 | 机器人常用算法优先：A*、Dijkstra、RRT、队列、图 |

不建议现在做的事：

- [ ] 不要同时开多个大型项目
- [ ] 不要一开始就啃完整 MoveIt2 源码
- [ ] 不要把 Nav2 做成第二个主线
- [ ] 不要为了学 LibTorch 牺牲 MoveIt2 和 ros2_control
- [ ] 不要一开始就深挖 Linux 内核、Yocto 或复杂 bootloader
- [ ] 不要把 STM32 所有外设都学一遍，要围绕电机控制和通信
- [ ] 不要随机刷 C++ 面试题，要和项目代码绑定

---

## 新版简历项目定位

项目名称可以升级为：

> 基于 ROS2、MoveIt2 与 ros2_control 的机械臂控制与规划系统

简历描述方向：

```text
实现了一个基于 ROS2 的机械臂控制与规划项目，包含纯 C++ 控制库、ROS2 控制节点、URDF/RViz2 可视化、MoveIt2 运动规划和 ros2_control 控制器接口。项目中完成了速度到位置的控制闭环、关节状态发布、TF 可视化、OMPL 规划配置以及 FollowJointTrajectory 控制链路，并进一步扩展 Nav2 移动机器人导航、YOLO 视觉感知接入 ROS2，以及基于 Raspberry Pi 4B 的 ROS2 上位机与 STM32 下位机通信链路设计。
```

面试讲解升级版：

1. 我先用纯 C++ 写控制库，保证核心逻辑可测试。
2. 然后用 ROS2 节点把控制逻辑接入 topic、timer、parameter 和 launch。
3. 再用 URDF、JointState、robot_state_publisher 和 RViz2 做可视化闭环。
4. 接着加入 MoveIt2，让系统具备规划能力。
5. 再接 ros2_control，让规划轨迹能进入标准控制器接口。
6. 最后扩展学习 Nav2、YOLO 感知链路，以及 Raspberry Pi 4B + STM32 的上位机 / 下位机通信，形成对机器人系统的整体认识。

---

## 每天执行模板

每天开始前先写：

~~~markdown
## 2026-xx-xx

### 今日目标

- [ ] 

### 今日实现

- [ ] 

### 今日运行命令

```bash

```

### 今日问题

- 

### 明天第一步

- 
~~~

每天结束时必须留下三样东西：

1. 今天跑通了什么命令
2. 今天遇到的一个问题
3. 明天打开电脑后第一步做什么

---


## 项目解释主线

面试或复盘时按这条线讲：

1. 我先把控制逻辑从 ROS2 中解耦出来，做成纯 C++ 控制库。
2. 控制库里分了状态管理、安全限幅、轨迹插值、轨迹缓存和雅可比求解几个模块。
3. 然后我把纯 C++ 控制逻辑迁移成 ROS2 节点，实现速度命令到位置命令的转换。
4. 最后用 URDF、RViz2 和 TF2 做简单机械臂可视化，形成一个完整的控制链路。
5. 在基础闭环跑通后，加入 MoveIt2，让机械臂具备运动规划能力。
6. 再接入 ros2_control 和 joint trajectory controller，让规划轨迹进入标准控制器接口。
7. 最后横向补 Nav2、YOLO/PyTorch/LibTorch 和现代 C++，把项目扩展成机器人系统能力展示。

---

## 风险与调整

### 如果 C++ 卡住

不要补整本语法书，只补当前模块需要的知识：

- `class`
- `std::vector`
- `std::deque`
- `const &`
- exception
- CMake target

### 如果 ROS2 环境卡住

先继续完成前 2 周纯 C++ 部分。纯 C++ 控制库不依赖 ROS2，不能因为环境问题停掉主线。

### 如果时间不够

优先级如下：

1. 纯 C++ 控制库跑通
2. `vel_to_pos_node` 跑通
3. RViz2 可视化
4. README 写清楚
5. MoveIt2 基础规划
6. ros2_control 控制器接口
7. 现代 C++ 和项目重构
8. Nav2 / 视觉 / LibTorch 扩展

---

## 关联资料

- 原始 8 周任务书：`C:/Users/zlab/Documents/robot-learning/robot_arm_control_ros2_taskbook.md`
- 原始 4 周加速任务书：`C:/Users/zlab/Documents/robot-learning/robot_arm_control_ros2_accelerated_taskbook.md`
