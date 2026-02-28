主要是写一个介绍信，方便`pluginlib::ClassLoader`去查找到我们。

📝 通用的模版如下：
```xml
<library path="lib/libYOUR_LIBRARY_NAME"> <!-- 库文件路径 -->

  <class name="your_namespace/YourPluginName"                     
         type="your_namespace::YourClassName" 
         base_class_type="planning_interface::PlannerManager"><!-- 插件名称 类的名称 父类名称 -->
         
    <description><!-- 描述 -->
      这里写一段简短的描述，说明这个规划器是做什么的。
      例如：这是一个自定义的线性插值规划器。
    </description>
    
  </class>

</library>
```

⚡️主要分为以下几个关键信息：
- 库文件的路径
- 我们插件的名称
- 我们类的名称
- 继承的基类
- 一段简短的描述

---

🥊 **库文件路径**就是我们生成的**动态链接库**
💡 我需要将我们写的程序在`CMakelist`中指定生成库，生成的库会**自动添加前缀和后缀**
```txt
add_library(moveit_lerp_planner_plugin
  src/lerp/lerp_planner_manager.cpp
  src/lerp/lerp_interface.cpp
  src/lerp/lerp_planning_context.cpp)
```

- 将三个源程序打包成我们所需要的库文件
- 库文件名称会自动加前缀(lib)和后缀(.so)
- 例如会生成库文件`libmoveit_lerp_planner_plugin.so`

⚡️ `base_class_type` 插件名称就是`pluginlib::ClassLoader`所要查找的名称(父类名称)

⚡️ `type` 就是用C++写的库文件中那个类的名称