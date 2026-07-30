# ROS1 pluginlib 插件机制

> 适用范围：ROS1/catkin 的 `pluginlib`。MoveIt 的规划器、控制器管理器等可通过这一机制在运行时按配置加载。

## 结论

`pluginlib` 解决的是“主程序只依赖稳定接口，具体实现由配置选择”的问题。它不是自动热更新：修改或替换插件后，通常仍需重新构建并重启加载该插件的进程。

## 一个插件由哪些部分组成

| 部分 | 作用 |
| --- | --- |
| 抽象基类 | 定义主程序依赖的稳定接口 |
| 派生类 | 提供具体实现 |
| 导出宏 | 将派生类注册到共享库 |
| 插件 XML | 声明库路径、类名和基类类型 |
| `package.xml` 导出 | 让 `pluginlib` 能找到 XML |
| `ClassLoader` | 在运行时创建实例 |

## 实现流程

1. 在独立的接口包中定义抽象基类，并使用虚析构函数。
2. 在实现包中继承该基类，完成具体逻辑。
3. 将实现编译为共享库，并在 `.cpp` 末尾导出类：

```cpp
#include <pluginlib/class_list_macros.h>
PLUGINLIB_EXPORT_CLASS(my_plugins::MyPlanner, my_base::PlannerBase)
```

4. 编写插件描述文件：

```xml
<library path="lib/libmy_planner">
  <class name="my_plugins/MyPlanner"
         type="my_plugins::MyPlanner"
         base_class_type="my_base::PlannerBase">
    <description>Example planner plugin.</description>
  </class>
</library>
```

5. 在 `package.xml` 的 `<export>` 中导出 XML；属性名应与加载方查找的属性一致。
6. 使用 `pluginlib::ClassLoader<Base>` 创建实例，并捕获 `pluginlib::PluginlibException`。

## CMake 与 package.xml 检查点

```cmake
find_package(catkin REQUIRED COMPONENTS pluginlib)
add_library(my_planner src/my_planner.cpp)
target_link_libraries(my_planner ${catkin_LIBRARIES})
```

`package.xml` 需要声明 `pluginlib` 依赖，并导出插件 XML。库名、XML 中的 `path`、`type`、`base_class_type` 与代码命名空间必须一致。

## 排错顺序

1. 重新构建后是否 `source devel/setup.bash`。
2. 共享库是否实际生成在工作空间的库目录。
3. XML 路径、类名、基类全限定名是否与代码一致。
4. `rospack plugins --attrib=plugin <base_package>` 能否列出插件声明。
5. 运行时异常是“找不到 XML/库”还是“类未导出/基类不匹配”。

## 与 MoveIt 的关系

MoveIt 借助插件加载规划器、规划请求适配器和部分控制器管理器。源码阅读入口见 [[../MoveIt/OMPL源码阅读/ROS Pluginlib|MoveIt 中的 pluginlib]]；通用的规划链路见 [[../MoveIt/OMPL源码阅读/move group Node|move_group]]。
