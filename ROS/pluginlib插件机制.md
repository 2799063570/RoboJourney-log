

参考资料：[ ROS pluginlib 插件](https://blog.csdn.net/liam_dapaitou/article/details/88956555)\\ [无处不在的小土](http://gaoyichao.com/Xiaotu/?book=ros&title=ROS%E6%8F%92%E4%BB%B6%E7%B3%BB%E7%BB%9Fpluginlib)\\ [土豆西瓜大芝麻](https://blog.csdn.net/jinking01/article/details/79414343)

动态链接库是一段包含可执行代码和数据的文件，它不会被直接打包死在你的主程序里，而是在程序运行的时候才被操作系统“动态”加载进来。

想象你要写一篇论文（主程序），里面需要引用一段很长的百科全书资料（库函数）：

- **静态链接（Static Linking）：** 你把那段百科全书的内容**原封不动地抄进**你的论文里。
	- 在之前写一些C语言的程序，我们都是将一些标准库的程序复制到我们的仓库中，用include直接引用
	- _缺点：_ 论文会变得非常厚（程序体积大）；如果百科全书更新了，你必须撕掉重抄一遍（重新编译整个主程序）；如果十个人都要引用这段话，这段话在世界上就存在了十份（浪费内存）。
- **动态链接（Dynamic Linking）：** 你在论文里**只写一个引用注脚**：“详情请见某图书馆某书第 X 页”。
	- 这里的程序，我们只需要包含基类头文件和加载插件的头文件即可
	- 相关的功能函数都已在基类中声明好
	- _优点：_ 论文很薄（程序体积小）；百科全书更新了，你的论文完全不用动，读者自己去查最新版就行（热更新/插件化）；十个人可以共用这一本百科全书（节省内存）。

具体来说优势在于：
1. **可扩展性**：我们可以源源不断地写新的 `.so` 文件（比如三角形插件、圆形插件），主程序可以在不重新编译的情况下，按需加载这些新功能。
2. **独立性**：只要接口（头文件里定义的函数签名）不发生改变，你可以随时替换掉旧的 `.so` 文件来修复 Bug 或优化性能，依赖它的程序甚至不需要重启（或仅需重启，无需重新编译）。
3. **空间**：多项程序可以借用动态链接共用一个库

下面我们介绍一下插件创建和使用的过程：
1. 创建一个抽象基类, 定义统一通用接口，（一个规定了接口的类，后续插件都以他为模板）
2. 创建plugin类, 继承基类, 实现统一的接口并注册插件
3. 

首先我们需要**规定统一的接口**，动态加载，也是加载规定格式下的各种对象。因此第一步就是创建一个基类，来规定统一的接口。例如下面，我们想要动态加载各种各样的图形（例如，三角形，圆形），我们需要先抽象出一个类，规定一下各个动态加载的类，他们要实现哪些功能，以方便我们的调用。
```cpp
namespace polygon_base{
class RegularPolygon
{
public:
	virtual void initialize(double param) = 0;
	virtual double area() = 0;
	virtual ~RegularPolygon() {}
protected:
	RegularPolygon() {}
};
};
```
我们需要根据基类去创建我们所需要的接口，这个时候就需要继承基类，来实现基类的各个函数。
```cpp
namespace polygon_plugins{
class Triangle : public polygon_base::RegularPolygon
{
public:
	virtual void initialize(double param) override
	{
		side_length_ = param;
		hight_ = side_length_ * 1.732;
	}
	virtual double area() override
	{
		return hight_ * side_length_ * 0.5;
	}
	
private:
	double hight_;
	double side_length_;
};
class Square : public polygon_base::RegularPolygon
{
public:
	virtual void initialize(double param) override
	{
		ridous = param;
	}
	virtual double area() override
	{
		return ridous*ridous*3.14;
	}
private:
	double ridous;
};
};
```
注册插件：说明一下创建的两个派生类是插件
需要注意的是 
	要调用声明插件的宏，要引入头文件，因此包要依赖pluginlib
	要声明子类和父类，因此也要引入对应的头文件，不同的包要加入对应包的依赖
```cpp
#include <pluginlib/class_list_macros.h>
#include <pluginlib_tutorials_/polygon_base.h>
#include <pluginlib_tutorials_/polygon_plugins.h>
PLUGINLIB_EXPORT_CLASS(polygon_plugins::Triangle, polygon_base::RegularPolygon);
PLUGINLIB_EXPORT_CLASS(polygon_plugins::Square, polygon_base::RegularPolygon);
```

在写完插件后我们需要生成对应的动态链接库`.so`
在CMakeList中声明
```cmake
include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)
add_library(plugin_test
  src/plugin_test.cpp
)
```
我们可以看到的对应工作空间下的devel下的lib生成对应的动态链接库文件`.so`

上面的步骤使得插件的实例一旦加载就可以创建插件的实例对象，但是插件加载器仍然需要一种方法来查找该库并知道该库中的引用内容。为此，我们还将创建一个XML文件，并修改`package.xml`。该文件与`package.xml`一起，可以为ROS提供有关我们的插件的所有必要信息。

```xml
<library path="lib/libpolygon_plugins">
  <class type="polygon_plugins::Triangle" base_class_type="polygon_base::RegularPolygon">
    <description>This is a triangle plugin.</description>
  </class>
  <class type="polygon_plugins::Square" base_class_type="polygon_base::RegularPolygon">
    <description>This is a square plugin.</description>
  </class>
</library>
```
path：动态链接库的地址
type：插件名称
base_class_type：继承的基类的名称
```xml
<export>
  <pluginlib_tutorials_ plugin="${prefix}/polygon_plugins.xml"/>
</export>
```
pluginlib_tutorials_：基类所在的包
plugin：插件信息描述文件的地址

我们可以通过以下指令来查看插件的情况
```
rospack plugins --attrib=plugin pluginlib_tutorials_
```

插件的使用
```cpp
#include <pluginlib/class_loader.h>
#include <pluginlib_tutorials_/polygon_base.h>
int main(int argc, char** argv)
{
  pluginlib::ClassLoader<polygon_base::RegularPolygon> poly_loader("pluginlib_tutorials_", "polygon_base::RegularPolygon");
  try
  {
    boost::shared_ptr<polygon_base::RegularPolygon> triangle = poly_loader.createInstance("polygon_plugins::Triangle");
    triangle->initialize(10.0);

    boost::shared_ptr<polygon_base::RegularPolygon> square = poly_loader.createInstance("polygon_plugins::Square");
    square->initialize(10.0);

    ROS_INFO("Triangle area: %.2f", triangle->area());
    ROS_INFO("Square area: %.2f", square->area());
  }
  catch(pluginlib::PluginlibException& ex)
  {
    ROS_ERROR("The plugin failed to load for some reason. Error: %s", ex.what());
  }
  return 0;
}
```