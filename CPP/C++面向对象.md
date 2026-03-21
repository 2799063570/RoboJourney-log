#CPP面向对象

计划：
- 按照侯捷老师的[视频课程](https://www.bilibili.com/video/BV1r6h5zgE2i?spm_id_from=333.788.videopod.sections&vd_source=bcc6342073e3898af13f7271c68eaf89)进行


目标就是培养正规的、大气的编程习惯（基于对象、面向对象）

- 以良好的方式编写C++ class (这两类对象的**内存管理策略**和**生命周期**有着天壤之别)
	- class without pointer members(不带有指针成员) -- Complex
	- class with pointer members -- String
- Classes之间的关系
	- 继承（inheritance）
	- 复合（composition）
	- 委托（delegation）

常见的版本是C++98 C++11

我想这可能就是我一直 要寻找的答案吧！
什么是面向对象？
面向对象分为面向对象编程(OOP)和面向对象设计(OOD)，面向对象设计作为思想指导面向对象编程，面向对象编程是具体的实践。

|概念|本质|核心目标|
|---|---|---|
|OOP（编程）|基于 “对象” 的编程范式|用代码实现 “封装、继承、多态”|
|OOD（设计）|面向对象的设计思想|设计出 “高内聚、低耦合、可扩展” 的类 / 对象结构|

面向过程编程以**功能实现**为核心导向，聚焦 “如何按步骤完成任务”，将需求拆解为一系列连续的函数 / 步骤，数据往往作为全局变量或参数在函数间传递，与操作数据的行为是分离的；而面向对象编程则以**数据（对象）** 为核心，把数据和操作数据的行为封装为一个整体（类 / 对象），聚焦 “由谁来完成任务”，通过对象之间的协作实现功能。

相比之下，面向对象编程凭借封装、继承、多态等核心特性，能够实现更灵活的代码复用（如继承复用共性逻辑、组合扩展新能力），其 “对扩展开放、对修改关闭” 的设计特性也让程序具备更强的扩展性（新增功能仅需扩展类 / 对象，无需修改原有代码），同时封装机制降低了代码耦合度，让程序的维护成本更低、可读性更好。因此，面向对象编程更适配需求复杂、易变更、需要长期维护或多人协作的大型软件系统；而面向过程编程更适合简单、线性、一次性的小型任务，能以更简洁的方式快速实现功能。



### 头文件和类的声明

complex.h **guard (防卫式声明)**
```cpp
#ifndef __COMPLEX__
#define __COMPLEX__

// ... 实现的内容

#endif
```
头文件中的布局
```cpp
#ifndef __COMPLEX__
#define __COMPLEX__

#include <cmath>
// 前置声明 forward declaration
class ostream;
class complex;

complex& __doapl(complex& ths, const complex& r);
// 类声明 class declaration
class complex
{
// ...
};

// 类定义 class definition
complex::function ... 

#endif
```

**class声明示例**
	有些直接在body内进行定义，便自动成为inline的候选人
	有些在body之外进行定义
```cpp
class complex
{
public:
	complex(double r = 0, double i = 0) : re(r), im(i) { };
	complex& operator +=(const complex&);
	double real() const {return re};  // 常量成员函数 保证不改变程序内容
	double imag() const {return im};
private:
	double re, im;
	friend complex& __doapl(complex*, const complex&);
 };
 
complex c(1, 3);
complex c2;
```
当对象是 const 时：

- 只能调用 const 成员函数
- 不能调用会修改对象状态的函数
    
编译器认为：
double real() 可能会修改成员变量（即使你没有）
所以拒绝在 const 对象上调用。

class模板示例
```cpp
template<typename T>
class complex
{
public:
	complex(T r = 0, T i = 0) : re(r), im(i) { };
	complex& operator += (const complex&);
	T real() {return re};
	T imag() {return im};
private:
	double re, im;
	friend complex& __doapl(complex*, const complex&);
}

complex<double> c(1.2, 1.3);
complex<int> c2;
```

构造函数：可以设置初值列(initialization list)，相对于函数体内进行赋值，该方法则是在初始化过程提供了一个默认值，拥有更高的效率，对于一些常量和引用的变量只能使用初值列的方法
构造函数放置到private区域，对象将不能正常的创建, 只能通过静态函数进行创建，可以用来保证只有一个对象创建
```cpp
Class A{
public:
	static A& getInstance();
	void setup() { }
private:
	A();
	A(const A& rhs);
};
A& A::getInstance()
{
	static A a;
	return a;
}
A::getInstance().setup();
```
同一个类下的各个成员之间互为友元
返回引用和返回值的区别就在于，是返回本身还是返回一个临时对象
对于返回引用，不必在意接收端的形式，如果使用引用的话效率更高，使用对象的话会出现拷贝构造

带指针成员的类
```cpp
#ifndef __STRING__
#define __STRING__
class String1
{
public:
	String1() : m_data(nullptr), size(0) {};
	String1(const char* cstr = 0);
	String1(const String1& str);
	friend ostream& operator<<(ostream& os, const String1& s);
	String1& operator=(const String1& str);
	~String1();

private:
	char* m_data;
	int size;
};

String1::String1(const char* cstr)
{
	cout << "cstr constructor called" << endl;
	if (cstr)
	{
		size = strlen(cstr);
		m_data = new char[size + 1];
		strcpy(m_data, cstr);
	}
	else
	{
		m_data = new char[1];
		*m_data = '\0';
	}
}
String1::String1(const String1& str)
{
	cout << "copy constructor called" << endl;
	m_data = new char[str.size + 1];
	size = str.size;
	strcpy(m_data, str.m_data);
}
ostream& operator<<(ostream& os, const String1& s)
{
	return os << s.m_data;
}
String1& String1::operator=(const String1& str)
{
    if (this == &str) // 判断是否为自我赋值 
	    return *this;
	cout << "operator= called" << endl;
	if (m_data) delete[] m_data;
	m_data = new char[str.size + 1];
	size = str.size;
	strcpy(m_data, str.m_data);
	return *this;
}
String1::~String1()
{
	if (m_data) delete[] m_data;
}
#endif
```

默认的**拷贝构造** 和 **拷贝赋值**实现是每个位置的复制
带指针成语的对象需要重点关注三个函数：
- 拷贝构造函数
- 拷贝赋值函数
- 析构函数


栈stack 堆heap
- **Stack**，是存在于某作用域scop的一块内存空间memory space，例如当我们调用函数，函数本身就是形成一块stack用来放置它所接收的参数，以返回地址。例如再函数body中声明的任何变量，其所用的内存块都取自上述Stack
- **Heap**，system heap是指由操作系统提供的一块global内存空间，程序可动态分配，从中获取若干区域块

```cpp
{
	Complex c1(1, 2);                       // c1 所占用的空间来自于stack
	Complex& p = new Complex(1, 2);         // Complex(1, 2)是一个临时对象 所占用空间以new自heap空间 p指向 
}
```
new可以分解为三个动作
- `void* mem = operator new( sizeof(Complex) );`  分配内存 相当于C中的`malloc(n)`
- `p = static_cast<Complex*>(mem);` 转型
- `p->Complex::Complex(1, 2);`构造函数

delete也可以进行分解
- `Complex::~Complex(p);` 析构函数
- `operator delate(p);` 释放内存 调用`free(p)`


![[内存分配.png#img_center]]

在内存中，除了对象的成员（8个字节）还要加上首尾（4\*2个字节），调试模式下还要加上（32 + 4），若不为16的倍数，还要添加补充，进行内存的对齐
![[内存分配2.png#img_center]]
对于数组需要额外加上一个4位的数记录数组个数
`delete[]` 相对于`delete`可以告诉编译器 要调用多次析构函数

### 补充static

一个类中主要分为四类
- 成员变量
- 静态成员变量
- 成员函数
- 静态成员函数

成员变量是创建的每个对象所单独拥有的各自的变量
而静态成员变量是该类所创建的所有对象所共有的变量
举个例子，对于银行来说，每个用户的存款是成员变量，但是利率是静态成员变量，所有人所共用的

成员函数具有默认对象的指针，可以访问对象的地址
而静态成员函数不可以访问成员变量，因此只能访问静态成员变量

```cpp
class Account {
public:
	static double m_rate;
	static void set_rate(const double& x) {m_rate = x;}
};
double Account::m_rate = 8.0; // 需要在类外定义初始化 类内声明
int main()
{
	Account::set_rate(4.0);
	Account a;
	a.set_rate(4.0);
}
```
调用静态成员函数有两种方法：
- 通过object调用
- 通过class name调用

|                                               | const object（data members 不可改变） | non-const object（data members 可改变） |
| --------------------------------------------- | ------------------------------- | ---------------------------------- |
| const member function（保证不更改 data members）     | √                               | √                                  |
| no-const member function（不保证 data members 不变） | ×                               | √                                  |
const可作为区别函数重载的辨识符号
COW ：copy on write（调用可修改）
区别常量对象和非常量对象调用
当成员函数的const和no-const版本同时存在，const object只会调用const版本，non-const object只会调用non-const版本

补充namespace

将一些内容包装在命名空间下
```cpp
namespace std
{
	...
}
```
使用的几种方法如下
- `using namespace std;` 将整个封锁打开 using directive
- `using std::cout;` 对单个成员 using declaration
- `std::cin>>a;` 全名调用

考虑类和类之间的关系，面向对象的思想
Classes之间的关系
- 继承（inheritance）
- 复合（composition）
- 委托（delegation）

### composition

复合就是一个类包含其他的类的对象，A类包含B类，且A的功能实现依赖于B的功能实现（**adapter**）例如queue通过deque

composition下的构造和析构
- 构造由内而外：Container的构造函数首先调用Component的**default构造函数**，然后执行自己(编译器自动加的，如需修改手动修改构造函数即可)
- `Container::Container() : Component() {...};`
- 析构由外而内：Container的析构函数首先执行自己，然后再调用Component的析构函数
- `~Container::~Container() {... ~Component() };`

```cpp
template <typename T>
class queue{

protected:
	deque<T> c;   // 底层容器
public:
	// 以下完全利用c的成员函数实现
	bool empty() const { return c.empty(); }
	T front() const { return c.front(); }
	T back() const { return c.back(); }
	void push(const T& val) { c.push_back(val); }
	void pop() { c.pop_front(); }
	int size() { return c.size(); }
}
```
### delegation

delegation委托就是一个类包含其他类的对象的指针（compostion by reference）
注意生命周期和复合是不同的，复合是外部对象创建的过程中创建了内部对象
Pimpl（Pointer to Implementation，**指向实现的指针**）是 C++ 中一种经典的设计技巧（也常归为 “编译防火墙” 模式），核心思想是：**将类的私有成员（数据 / 函数）封装到一个独立的 “实现类” 中，对外只暴露一个指向该实现类的指针**。

![[delegation图解.png#img_center]]
```cpp
class StringRep;
class String{
public:
	String();
	String(const char* s);
	String(const String& str);
	String& operator= (const String& str);
	~String();
...
private:
	StringRep* rep;
};

class StringRep{
friend class String;
private:
	StringRep(const char* s);
	~StringRep();
	int count;
	char* rep;
};
```

### inheritance

inheritance继承，分为三种继承方式（`public`、`private`、`protected`）
inheritance下的构造和析构
- **构造由内而**外：derived的构造函数首先调用base的**default构造函数**，然后执行自己(编译器自动加的，如需修改手动修改构造函数即可)
- Derived::Derived() : base() {...};`
- **析构由外而内**：Derived的析构函数首先执行自己，然后再调用base的析构函数
- `~Derived::~Derived() {... ~base() };`

对于继承 有三种函数
- **non-virtual 函数**：你不希望derived重新定义(override)
- **virtual 函数**：你希望derived class重新定义(override)，并且对他已经有了默认的定义
- **pure virtual 函数**：你希望 derived class一定要重新定义(override)，对他没有默认定义

```cpp
class shape{
public:
	virtual void draw() const = 0;   // pure virtual
	virtual void error(const std::string& msg);  // virtual
	int objectID() const;  // non-virtual
};
class Rectangle : public shape{...};
class Ellipse : public shape{...};
```

对于non-virtual函数派生类又定义了一个相同的函数**不是 “真正的重写（override）”，只是 “隐藏（hide）”**
C++ 中「重写（override）」是有严格定义的：**只有基类的虚函数被派生类同名、同参数、同返回值的函数覆盖，才叫 override**；如果基类函数没有 `virtual`，派生类的同名函数只是「隐藏」了基类函数，而非重写。

- 父类指针 `p` 指向 `Circle` 对象，但调用 `show()` 时，**编译器只看指针类型（`Shape*`）**，直接绑定 `Shape::show()`，完全忽略对象的实际类型（`Circle`）；
- 这种绑定是「静态绑定（编译期绑定）」，本质是 “同名函数隐藏”，而非 “重写”。

`virtual` 是给编译器的 “提示”：这个函数允许派生类重写，且调用时要走「动态绑定」逻辑，而非「静态绑定」。

- 不加 `virtual`：编译器认为该函数是 “静态的”，调用时直接根据指针 / 引用的类型确定调用哪个版本；
- 加 `virtual`：编译器会为类生成「虚函数表（vtable）」，运行时通过对象的「虚表指针（vptr）」找到实际要调用的函数版本。

| 函数类型 | 编译器 / 程序的判断依据                 | 绑定时机 | 是否看指针类型 |
| ---- | ----------------------------- | ---- | ------- |
| 非虚函数 | 指针 / 引用的**声明类型**（比如 `Shape*`） | 编译期  | ✅ 必须看   |
| 虚函数  | 对象的**实际类型**（比如 `Circle`）      | 运行期  | ❌ 完全不看  |
- 非虚函数：编译器 “认指针不认对象”，编译时就根据指针类型定死调用哪个函数；
- 虚函数：编译器 “认对象不认指针”，运行时才根据对象实际类型找函数，指针类型只是 “壳子”。

```cpp
class Shape {
public:
    virtual void show() { // 虚函数
        cout << "Shape::show()" << endl;
    }
    void printHide() { // 隐藏
	    cout << "Shape::printHide()" << endl;
    }
};

class Circle : public Shape {
public:
    void show() override { // 重写
        cout << "Circle::show()" << endl;
    }
    void printHide() {
	    cout << "Circle::printHide()" << endl;
    }
};

int main() {
    Shape* p1 = new Shape();   // 指针=Shape*，对象=Shape
    Shape* p2 = new Circle();  // 指针=Shape*，对象=Circle
    Circle* p3 = new Circle(); // 指针=Circle*，对象=Circle

	// 对象
    p1->show(); // 运行期：对象是Shape → 调用Shape::show()
    p2->show(); // 运行期：对象是Circle → 调用Circle::show()（不管指针是Shape*）
    p3->show(); // 运行期：对象是Circle → 调用Circle::show()
	// 指针
	p1->printHide(); // 指针 Shape::printHide()
	p2->printHide(); // 指针 Shape::printHide() 
	p3->printHide(); // 指针 Circle::printHide()

    delete p1; delete p2; delete p3;
    return 0;
}
```

### **template method**
![[虚函数的使用.png]]

如图所示，在子类中对父类的虚函数进行了重新定义
在程序中，利用子类对象对父类函数(`OnFileOpen`)进行了调用，父类函数又调用了通过虚函数overide的子类函数
从而达到一种在一个方法中定义一个算法的骨架，而将一些步骤延迟到子类中。模板方法使得子类可以在不改变算法结构的情况下，重新定义算法中的某些步骤。
一种思想：不变的流程由框架控制，可变的细节由子类实现

值得注意的是`myDoc.OnFileOpen(); `实际的调用为 `CDocument::OnFileOpen(&myDoc);`即直接调用的父类的函数
同时呢在函数内部的`Serialize()` 实际的调用为 `this->Serialize()`


![[继承复合构造顺序.png#img_center]]

当创建一个包含「继承(inheritance) + 成员对象(compostion)」的派生类对象时，构造函数的执行遵循**固定优先级**：

1 先执行「基类的构造函数」
- 派生类 `circle` 继承自 `shape`，因此首先调用基类 `shape` 的默认构造函数；
- 如果有多个基类，按**继承声明的顺序**执行（而非初始化列表顺序）。
2 再执行「成员对象的构造函数」
- `circle` 类中包含成员对象 `person p`，基类构造完成后，调用成员对象 `person` 的构造函数；
- 如果有多个成员对象，按**类内声明的顺序**执行（而非初始化列表顺序）。
3 最后执行「派生类自身的构造函数」
- 基类和成员对象都构造完成后，才执行 `circle` 自己的构造函数体。

而派生类继承基类，基类包含成员对象的时候，构造顺序就很明显了：成员对象-基类-派生类
那么析构的顺序正好是和构造的顺序是相反的

delegation委托 + inheritance继承 （**observer**）
```cpp
class Subject
{
	int m_value;
	vector<observer*> m_views;
public:
	void attach(observer* obs) { m_views.push_back(obs); }
	void set_val(int val) {m_value = val;}
	void nodify()
	{
		for (int i = 0; i<m_views.size(); ++i)
			m_views[i]->update(this, m_value);
	}
};
class observer
{
public:
	virtual void update(Subject* s, int val) = 0;
};
```
通过vector存入多个observer对象的地址，同时添加许多的函数，实现相关的功能，达到可以控制很多对象的目的

![[composite.png#img_center]]

```cpp
class Component
{
	int value;
public:
	Component(int val) { value = val; }
	virtual void add(Component*) { } 
};
class Primitive : public Component
{
public:
	Primitive(int val) : Component(val) {}
};
class Composite : public Component
{
vector<Component*> c;
public:
	Composite(int val) : Component(val) {}
	void add(Component* elem)
	{
		c.push_back(elem);
	}
};
```

### 组合模式（Composite Pattern）

| 角色                         | 说明                                                                   |
| :------------------------- | :------------------------------------------------------------------- |
| **Component（抽象组件）**        | 定义了所有对象（包括叶子和组合对象）的通用接口，图中包含一个成员变量 `int` 和 `add()` 方法，用于统一管理子组件。     |
| **Primitive（叶子节点 / 基本对象）** | 代表树形结构中的 “部分”，没有子节点，只实现业务逻辑，不包含子组件管理。                                |
| **Composite（组合节点 / 容器对象）** | 代表树形结构中的 “整体”，包含一个 `vector<Component*>` 来存储子组件，并实现 `add()` 等方法来管理它们。 |

组合模式的核心是“**统一对待单个对象和组合对象**”：
- 客户端通过 `Component` 接口与所有对象交互，无需关心它是叶子还是容器。
- 当对一个 `Composite` 对象执行操作时，它会递归地对所有子组件执行相同的操作，从而实现对整个树形结构的统一处理。

### 原型模式（Prototype Pattern）


![[原型模式图解.png#img_center]]

| 符号 / 标注        | 含义                                                                     |
| -------------- | ---------------------------------------------------------------------- |
| 空心三角箭头         | 继承（泛化）关系，箭头指向父类（抽象原型）                                                  |
| 空心菱形箭头         | 组合关系，箭头指向被包含的对象（Image 类包含自身类型的静态数组）                                    |
| 前缀`-`          | private（私有）成员，仅类内可访问                                                   |
| 前缀`#`          | protected（保护）成员，子类可访问，外部不可访问                                           |
| 紫色 / 带下划线的成员   | 静态成员（`static`），属于类而非对象，全局唯一                                            |
| 方法名(参数):返回值    | UML 标准方法表示，比如`clone(): Image*`表示 clone 方法返回 Image 类型的指针                |
| `virtual ctor` | 虚拟构造函数，C++ 中构造函数不能是虚函数，原型模式用`clone()`虚函数实现了 “运行时动态创建对象” 的效果，等价于虚拟的构造函数 |

```cpp
#include <iostream>
using namespace std;

// 抽象原型类 Image
class Image {
private:
    // 原型注册表：对应图中的 prototypes[10]: Image*
    static Image* prototypes[10];
    // 注册表当前存储的索引
    static int nextSlot;
protected:
    // 给子类注册原型用的接口
    virtual int addPrototype(Image* p) {
        if (nextSlot >= 10) return -1;
        prototypes[nextSlot] = p;
        return nextSlot++;
    }
public:
    // 核心克隆接口：对应图中的 clone(): Image*
    virtual Image* clone() = 0;
    // 对外工厂方法：对应图中的 findAndClone(i): Image*
    static Image* findAndClone(int index) {
        if (index < 0 || index >= nextSlot) return nullptr;
        return prototypes[index]->clone();
    }
    // 业务测试方法
    virtual void showType() = 0;
    virtual ~Image() = default;
};

// 静态成员类外定义
Image* Image::prototypes[10] = { nullptr };
int Image::nextSlot = 0;

// 具体原型类 LandSatImage
class LandSatImage : public Image {
private:
    // 静态原型实例：对应图中的 _LSAT: LandSatImage
    static LandSatImage _LSAT;
    // 私有默认构造函数：仅用于创建静态原型实例
    LandSatImage() {
        cout << "LandSatImage 原型实例创建，自动注册" << endl;
        addPrototype(this);
    }
protected:
    // 保护带参构造：给clone()创建新对象使用
    LandSatImage(int) {}
public:
    // 实现克隆接口
    Image* clone() override {
        cout << "克隆 LandSatImage 新对象" << endl;
        return new LandSatImage(0);
    }
    // 业务方法
    void showType() override {
        cout << "我是 LandSat 卫星图像" << endl;
    }
};

// 静态原型实例类外定义（触发自动注册）
LandSatImage LandSatImage::_LSAT;

// 具体原型类 SpotImage
class SpotImage : public Image {
private:
    // 静态原型实例：对应图中的 _SPOT: SpotImage
    static SpotImage _SPOT;
    // 私有默认构造函数
    SpotImage() {
        cout << "SpotImage 原型实例创建，自动注册" << endl;
        addPrototype(this);
    }
protected:
    // 保护带参构造
    SpotImage(int) {}
public:
    // 实现克隆接口
    Image* clone() override {
        cout << "克隆 SpotImage 新对象" << endl;
        return new SpotImage(0);
    }
    // 业务方法
    void showType() override {
        cout << "我是 Spot 卫星图像" << endl;
    }
};

// 静态原型实例类外定义（触发自动注册）
SpotImage SpotImage::_SPOT;

// 客户端代码
int main() {
    cout << "===== 客户端开始创建对象 =====" << endl;

    // 客户端完全不用知道LandSatImage/SpotImage，只需要调用Image的工厂方法
    Image* img1 = Image::findAndClone(0);
    Image* img2 = Image::findAndClone(1);

    if (img1) img1->showType();
    if (img2) img2->showType();

    // 释放内存
    delete img1;
    delete img2;

    return 0;
}
```

通过在基类中定义静态的基类指针数组作为全局地址注册表，后续各派生类借助其静态成员变量 “程序启动阶段自动初始化” 的特性，在自身静态原型实例的构造过程中，将该实例的地址自动注册到基类的这一数组中；最终使基类成为所有派生类原型实例地址的统一存储载体，实现基类对所有派生类地址的集中管理，进而支持通过基类接口创建任意派生类对象的解耦设计。

当然有许多需要注意的点
基类 `Image` 定义**静态的基类指针数组**，存储派生类地址的核心容器
- 「**静态**」保证：数组属于 `Image` 类本身，全局唯一，所有派生类共享这一个仓库（而非每个对象有独立数组）；
- 「**基类指针**」保证：能兼容所有派生类地址（派生类 is-a 基类，地址可隐式转换为基类指针）；
-   配套**静态索引**（如 `nextSlot`）：记录数组中已注册的派生类地址数量，避免越界。
-   同时作为基类，定义了一些纯虚函数，基类本身不能实例化，只能借助于静态成员变量\函数实现相关功能

每个派生类（`LandSatImage`/`SpotImage`）通过静态成员完成**自动注册**
-  派生类定义「静态的自身类型实例」（如 `_LSAT: LandSatImage`）：C++ 中静态成员会在 `main` 函数执行前、程序启动阶段**自动初始化**，触发派生类的私有构造函数；
-  构造函数内调用基类的 `addPrototype(this)`：把当前派生类实例的地址（`this` 指针）存入基类的静态数组中；
-  关键约束：派生类构造函数设为 `private`，只有静态成员能访问 —— 既保证原型实例能创建，又禁止外部直接 `new` 派生类对象，强制通过克隆创建。

在派生类的clone函数中，可以发现会new一个新的对象，这样必然要触发构造函数
但是我们不能让其使用默认构造函数，因为再静态对象定义的时候已经将对象加入到了基类数组中，这样会重复定义
因此我们采用了protected下的带参的构造函数（权限不能是public即可）

构造函数的访问权限是封装性的重要体现：

- 派生类的 “创建规则”（只能克隆、不能直接 new）是类自身的设计意图，将构造函数私有化，意味着 “对象该如何创建” 完全由类自己决定，外部无法干预；借助于基类统一管理
- 如果构造函数公开，外部可能以不规范的方式创建对象（比如未初始化必要成员、未注册到基类数组），导致程序出现野指针、注册表缺失等问题，破坏设计的完整性。



泛型编程（Generic Programming）和面向对象编程（Object-Oriented Programming）虽然分属不同思维，但是都是C++真正的技术主站
深入探索面向对象继承关系所形成对象模型（Object Model），包括隐藏于底层的this指针、vptr（虚指针）、vtbl（虚表）、virtual mechanism（虚机制）、以及虚函数（virtual function）所造成的polymorphism多态效果。

### 转换函数 conversion

转换函数的格式是固定的，**必须遵循 “无返回值类型 + operator 目标类型 ()”**，具体规则：
```cpp
// 转换函数的标准格式
operator 目标类型() const {
    // 函数体必须返回一个“目标类型”的值
    return 目标类型的值;
}
```
需要注意以下事项：
- **通常加 const**：转换函数一般不会修改对象成员
- **禁止隐式转换**：如果不想让`int`隐式转为`MyNum`，可给单参数构造函数加`explicit`
- **转换函数的本质**：是类的成员函数，只能定义在类内部，不能定义在类外

例如以下转换为浮点数
```cpp
class Fraction
{
public:
	Fraction(int num, int den=1) : m_numerator(num), m_denominator(den) {}
	operator double() const { return (double)(m_numerator/m_denominator); }
	
private:
	int m_numerator;  // 分子
	int m_denominator; // 分母
}
Fraction f(3, 5);
double d = 4 + f; // 调用operator double () 将f转为0.6
```


```cpp
template<class Alloc>
class vector<bool, Alloc>
{
public:
	typedef _bit_reference reference;
protected:
	reference operator[] (size_type n){
		return *(begin() + difference_type(n));
	}
};
struct _bit_refernce {
	unsigned int* p;
	unsigned int mask;
public:
	operator bool() const { return !(!(*p & mask)); }
};
```


pointer-like classes 开放智能指针 类的行为像一个指针
```cpp
template<class T>
class shared_ptr
{
public:
	T& operator*() const { return *px; }
	T* operator->() const { return px; }

private:
	T*    px;
	long* pn;
}
struct Foo
{
	void method(void);
};
shared_ptr<Foo> sp(new Foo);
Foo f(*sp);
sp->method();
```

需要注意的是箭头会持续性作用下去，例如sp->将返回一个地址，已经消耗了一个箭头，这里会自动加上箭头去调用成员函数

```cpp
template <class T, class Ref, class Ptr>
struct __list_iterator{
	typedef __list_node<T>* link_type;
	link_type node;
	
	reference operator* () const { return (*node).data; }
	pointer operator-> () const { return &(operator*()); }
};
template <class T>
struct __list_node{
	void* prev;
	void* next;
	T data;
};
```
这个就是迭代器的范式，想想我们使用迭代器的时候，确实使用过`*`来实例化，使用

function-like classes 所谓仿函数
```cpp
template <class T>
struct identipy {
	const T& operator() (const T& x) const { return x; }
};

template <class Pair>
struct select1st {
	const typename Pair::first_type& operator() (const Pair& x) const { return x.first; }
};
template <class Pair>
struct select2ed {
	const typename Pair::second_type& operator() (const Pair& x) const { return x.second; }
}

template <class T1, class T2>
struct pair
{
	T1 first;
	T2 second;
	pair() : first(T1()), second(T2()) {}
	pair(const T1& a, const T2& b) : first(a), second(b) {}
};
```

需要注意的是这里的`T1()` 、`T2()`
pair 作为模板，要适配**任意可默认构造的类型**，而 `T1()` 是 “不依赖具体类型” 的通用初始化方式：

- 不管 T1 是 int、string，还是你自定义的类，`T1()` 都能给出一致的 “默认空状态”；
- 不需要为不同类型写不同的默认构造（比如专门给 int 写一个初始化 0 的版本，给 string 写初始化空字符串的版本），符合泛型编程 “一次编写，适配所有” 的核心思想。

成员模板的使用 member template
```cpp
class Base1 {};
class Derived1: public Base1 {};

class Base2 {};
class Derived2: public Base2 {};
```

```cpp
pair<Derived1, Derived2> p;
pair<Base1, Base2> p2(p);

pair<Base1, Base2> p2(pair<Derived1, Derived2>());
```

```cpp
template<class T1, class T2>
struct pair
{
	T1 first;
	T2 second;
	pair() : first(T1()), second(T2()) {}
	pair(const T1& a, const T2& b) : first(a), second(b) {}

	template<class U1, class U2>
	pair(const pair<U1, U2>& p) : first(p.first), second(p.second) {}
};
```
我们用基类来声明数据类型，用派生类的对象来进行赋值（通过模板的形式可以接收任意类型的数据）
举个例子，用鱼类、鸟类声明的对象 通过 鲫鱼、麻雀来进行赋值，反正当然就不可以了

###  模板特化
```cpp
template <class Key> // 泛化
struct hash{ };

template <>
struct hash<char>{
	size_t operator() (char x) const { return x; }
};
template <>
struct hash<int>{
	size_t operator() (int x) const { return x; }
};
template <>
struct hash<long>{
	size_t operator() (long x) const { return x; }
};

cout << hash<long>(10000) << endl;
```

模板的偏特化 从左边到右边上的偏特化
```cpp
template<typename T, typename Alloc=...>
class vector
{
...
};
template<typename Alloc=...>
class vector<bool, Alloc>
{
...
};
```

范围上的偏特化 从任意类型到任意指针
```cpp
template <typename T>
class C
{
 ...
};
template <typename T>
class C<T*>
{
 ...
};
```

### 模板模板参数 template template parameter
模板里面嵌套模板

| 类型       | 核心特点                             | 例子                                                |
| -------- | -------------------------------- | ------------------------------------------------- |
| 普通类型模板参数 | 传**具体类型**（如 `int`/`vector<int>`） | `template <typename T> class A;`                  |
| 普通数值模板参数 | 传**常量值**（如 `5`/`10`）             | `template <int N> class B;`                       |
| 模板模板参数   | 传**模板本身**（如 `vector`/`list`）     | `template <template <typename> class C> class D;` |

```cpp
template <typename T>
class person {  
public:
    T name;
    int age;
};

template <typename T, template <typename T> class newType>
class house
{
public:
	T data;
    newType<string> tt;
};
house<int, person> t;
t.data = 100;
t.tt.age = 18;
t.tt.name = "zhangsan";
```

我们首先定义了一个单参数模板类 `person`：该类的 `name` 成员变量类型可通过模板参数 `T` 动态指定（注：`person` 模板仅包含一个类型模板参数），而 `age` 成员变量则固定为 `int` 类型。

接着，我们将 `person` 这个模板类本身作为 “模板级别的参数”，用于定义高阶模板类 `house`：`house` 包含两个模板参数，第一个是普通类型参数 `T`（对应 `house` 的 `data` 成员类型），第二个是模板模板参数 `newType`（用于接收如 `person` 这类单参数模板类）；在 `house` 内部，通过 `newType<string>` 实例化出具体的 `person<string>` 类型对象 `tt`。

```cpp
template <typename T,
    template <typename T> class Container >
class Wrapper {
public:
    Container<T> data;
};
```

需要注意的是上述代码是不可以的
它要求传入的模板**只能有一个类型参数**，但 `std::vector` 的实际模板定义是（C++ 标准）：
```cpp
// vector的真实模板：两个参数，第二个有默认值（分配器） 
template <typename T, typename Allocator = std::allocator<T>> class vector;
```

你要求 “传入 1 个参数的模板”，但 `vector` 是 “2 个参数（第二个有默认值）的模板”，参数数量不匹配 → 编译器无法匹配，直接报错。

解决方法有：
- 设置模板的时候定义两个参数
- 通过模板封装传入的具体类型

```cpp
template <typename T,
    template <typename T, typename Allocator = allocator<T>> class Container> // 定义两个参数
class Wrapper {
public:
    Container<T> data;
};
Wrapper<int, vector> w;
```

```cpp
template <typename T>
using Vec = vector<T, allocator<T>>; // 通过模板封装两个参数
Warpper<int, Vec> w;
```

因此需要注意的是模板模板参数 我们需要注意模板参数中参数的个数
例如容器类的模板，常常不是一个模板参数
指针类的容器，常常为一个模板参数

```cpp
template <typename T, class type = deque<T>>
class test {
public:
	type c;
};
test<int> s1;
test<int, list<int>> s2;
```
 注意区别的是以上的写法，这是一种**带默认值的 “普通类型模板参数”**。和 “模板模板参数” 的核心区别在于：**参数接收的是 “具体类型” 还是 “模板本身”** —— 前者是 “成品”，后者是 “模具”。

可以对比我们前面写的模板模板参数，是在模板中再加入一个模板而不是加入一个参数。前面的容器时可以自己定义类型的，后面的是输入指定类型的容器。

#### C++标准库

数据结构和算法组成了程序，而标准库就包含了他们

### variaidic templates 模板参数数量可变

```cpp
// 变参函数模板：处理任意数量、任意类型的参数，递归打印
template<typename T, typename... Types>
void print(const T& firstArg, const Types&... args) {
    cout << firstArg << endl;          // 打印第一个参数
    cout << sizeof...(args) << endl;   // 获取剩余参数包的大小
    if (sizeof...(args) > 0)
        print(args...);                // 递归展开参数包
}
print(12, "hello", 3.14);    // 12 hello 3.14
``` 

`...`就是一个所谓的park（包）
用于template parameters，就是template parameters park（模板参数包）
用于function parameter types，就是function parameter type pack（函数参数类型包）
用于function parameters，就是function parameter pack（函数参数包）


### reference

reference通常不用于声明变量，而用于参数类型和返回类型的描述
```cpp
void func1(Cls* pobj) { pobj->xxx(); }
void func2(Cls obj) { obj.xxx(); }
void func3(cls& obj) { obj.xxx(); }
...
Cls obj;
func1(&obj); // 接口不同 需要一步取地址
func2(obj); // 调用端的接口一致 相同输入即可
func2(obj);
```

以下被视为 “same signature” (二者不能同时存在 不能为编译器识别调用那个)
```cpp
double imag(const double& im) {...}
double imag(const double im) {...}  // Ambiguity
```

需要注意的是 const可以作为类中成员函数重载的区别

## 对象模型(Object Model) : 关于vptr 和 vtbl


```cpp
class A{
public:
	virtual void vfunc1();
	virtual void vfunc2();
			void func1();
			void func2();
private:
	int m_data1, m_data2;
};
class B : public A{
public:
	virtual void vfunc1();
			void func2();
private:
	int m_data3;
};
class C : public B{
public:
	virtual void vfunc1();
			void func2();
private:
	int m_data4;
}
```
![[vtbl-vptr.png]]

我们可以发现，当存在虚函数的时候，对象内部会存储一块地址，指向一个指针列表
列表中的指针指向对应的虚函数实现
当p指向一个对象，当调用虚函数时 `(*(p->vptr)[n])(p));` 或者 `(* p->vptr[n])(p);` 


`list<A*> myList;`
我们想让一个容器存储各种类型的内容（一个大类下的），例如猴子、狗、猫、老虎等等，他们的创建的具体的大小肯定时不一样的，我们为将其放到容器中只能使用指针，那么指针的地址就是基类指针类型
借助于虚函数的特点，调用的虚函数为指针实际指向对象的函数

**静态绑定**和**动态绑定**，核心是抓住「**绑定时机**」和「**判断依据**」两个核心维度 —— 静态绑定是「编译期定死」，动态绑定是「运行期才定」。

|类型|通俗比喻|技术定义|
|---|---|---|
|静态绑定|提前订好餐厅（编译时确定吃哪家）|编译阶段就确定调用哪个函数，绑定到「变量声明类型」|
|动态绑定|到店再选菜品（运行时确定吃什么）|运行阶段才确定调用哪个函数，绑定到「对象实际类型」|

编译器在**编译代码时**，就根据「变量的声明类型」（而非实际指向的对象类型）确定要调用的函数，函数地址直接 “写死” 在编译后的可执行文件里，运行时不会改变。
- 调用**非虚函数**（无论是否有继承关系）；
- 通过**对象本身**（而非指针 / 引用）调用函数（即使是虚函数）；
- 调用静态函数、构造函数、普通全局函数 / 成员函数；
- 函数重载（编译器根据参数类型 / 个数在编译期匹配）。

编译器在编译时**无法确定**调用哪个函数，只有程序运行时，才根据「指针 / 引用指向的实际对象类型」，通过虚函数表（vtable）和虚函数指针（vptr）找到对应的函数地址。（下列条件必须同时满足）
- 基类中声明函数为 `virtual`（生成 vtable 和 vptr 的前提）；
- 派生类重写（override）该虚函数（vtable 中替换函数地址）；
- 通过**基类指针 / 引用**调用该虚函数（避免对象切片）。

| 对比维度 | 静态绑定                  | 动态绑定                     |
| ---- | --------------------- | ------------------------ |
| 绑定时机 | 编译阶段                  | 运行阶段                     |
| 判断依据 | 变量的**声明类型**（比如 Base*） | 对象的**实际类型**（比如 Derived）  |
| 触发条件 | 无特殊要求（非虚函数 / 对象调用）    | 虚函数 + 派生类重写 + 基类指针 / 引用  |
| 底层实现 | 直接写入函数地址，无额外开销        | 通过 vptr 查找 vtable，轻微运行开销 |
| 核心用途 | 普通函数调用、函数重载           | 实现多态（基类指向不同派生类）          |
| 灵活性  | 低（编译后无法改变）            | 高（运行时根据对象类型调整）           |

## 关于this指针

template method

`this`指针是编译器自动为**非静态成员函数**添加的**隐藏第一个参数**，调用函数时，编译器会自动把调用对象的地址传入`this`，你可以在函数内部显式使用`this`指针访问对象成员。
在**非静态成员函数**中，直接访问本类的非静态成员变量、调用非静态成员函数时，编译器会**自动、隐式地补充 `this->`** —— 也就是默认以当前 `this` 指针指向的对象作为操作主体。

| 函数类型                | 是否传入对象指针（this 指针） | 原因                                            |
| ------------------- | ----------------- | --------------------------------------------- |
| 普通非静态成员函数           | ✅ 是（隐式传入）         | 函数属于**具体对象**，需要 this 指针指向调用函数的那个对象            |
| 静态成员函数（static）      | ❌ 否               | 函数属于**类本身**，不关联任何具体对象，因此不需要 this 指针           |
| 构造 / 析构函数           | ✅ 是（隐式传入）         | 构造函数初始化具体对象，析构函数销毁具体对象，需要 this 指向当前对象         |
| const/volatile 成员函数 | ✅ 是（隐式传入）         | 本质还是非静态成员函数，只是 this 指针被限定为`const Test*`（不可修改） |
![[template_method.png]]

在上述模式上，可以看到当`myDoc`对象对父类的函数进行调用时，会传入调用对象的地址
在函数中调用函数的时候会默认加上this指向
这时就触发了动态绑定（this指针指向（调用对象的指针）、存在继承关系、虚函数）实现对派生类中函数的调用

## 关于new和delete

前面提及
new可以分解为三个动作
- `void* mem = operator new( sizeof(Complex) );`  分配内存 相当于C中的`malloc(n)`
- `p = static_cast<Complex*>(mem);` 转型
- `p->Complex::Complex(1, 2);`构造函数

delete也可以进行分解
- `Complex::~Complex(p);` 析构函数
- `operator delate(p);` 释放内存 调用`free(p)`

|维度|malloc/free|new/delete|
|---|---|---|
|本质|库函数（需包含头文件）|C++ 运算符（无需头文件）|
|类型转换|返回 `void*`，需手动强转|自动返回对应类型指针，无需强转|
|初始化 / 析构|只分配 / 释放内存，不调用构造 / 析构函数|new 调用构造函数，delete 调用析构函数|
|失败处理|返回 `NULL`|默认抛 `bad_alloc` 异常（可指定 nothrow 返回 NULL）|
|数组操作|`malloc(n*sizeof(T))` + `free(ptr)`|`new T[n]` + `delete[] ptr`（必须加 []）|
|内存大小|需手动计算字节数|自动计算（`new int[5]` 自动算 5*4 字节）|

```cpp
void* myAlloc(size_t size)
{ return malloc(size); }
void myFree(void* ptr)
{ free(ptr); }

inline void* operator new(size_t size)
{ cout << "reconstruct global new()" << endl; return myAlloc(size); }
inline void* operator new[](size_t size)
{ cout << "reconstruct global new[]()" << endl; return myAlloc(size); }
inline void operator delete(void* ptr)
{ cout << "reconstruct global delete()" << endl; myFree(ptr); }
inline void operator delete[](void* ptr)
{ cout << "reconstruct global delete[]()" << endl; myFree(ptr); }
```
![[per-class allocator.png]]


