# C++ 面向对象复习

来源：侯捷 C++ 面向对象课程  
目标：形成正规的 C++ class 编写习惯，理解对象模型、资源管理和类之间的关系。

## 复习路线

1. 先理解 OOP / OOD 的区别。
2. 掌握不带指针成员的类：以 `Complex` 为例。
3. 掌握带指针成员的类：以 `String` 为例。
4. 熟悉构造、析构、拷贝构造、拷贝赋值。
5. 理解 `const`、`static`、`friend`、`inline`。
6. 区分继承、复合、委托。
7. 理解 stack / heap、`new` / `delete`、对象生命周期。

## 一页速查

| 主题           | 一句话记忆                          |
| ------------ | ------------------------------ |
| OOP          | 把数据和操作数据的函数封装成对象               |
| OOD          | 设计类与对象之间的协作关系                  |
| 封装           | 隐藏实现细节，只暴露稳定接口                 |
| 继承           | is-a，子类是一种父类                   |
| 复合           | has-a，一个类拥有另一个类对象              |
| 委托           | has-a by pointer，一个类把任务交给另一个对象 |
| 构造函数         | 对象出生时初始化                       |
| 析构函数         | 对象死亡时清理资源                      |
| 拷贝构造         | 用已有对象创建新对象                     |
| 拷贝赋值         | 已存在对象之间赋值                      |
| `const` 成员函数 | 承诺不修改对象状态                      |
| `static` 成员  | 属于类，不属于某一个对象                   |
| `friend`     | 允许外部函数或类访问 private 成员          |
| `new`        | 分配内存 + 调用构造函数                  |
| `delete`     | 调用析构函数 + 释放内存                  |

## OOP 和 OOD

| 概念 | 本质 | 核心目标 |
|---|---|---|
| OOP | 面向对象编程 | 用代码实现封装、继承、多态 |
| OOD | 面向对象设计 | 设计高内聚、低耦合、可扩展的类结构 |

面向过程关注“步骤”：先做什么，再做什么。  
面向对象关注“角色”：谁拥有数据，谁负责行为，谁和谁协作。

适合用 OOP 的场景：

- 需求会长期变化。
- 多个模块需要协作。
- 数据和操作天然绑定。
- 需要控制复杂度和维护成本。

不一定需要 OOP 的场景：

- 一次性脚本。
- 很短的线性流程。
- 数据结构和行为非常简单。

## Class 的两种难度

学习 C++ class 可以分成两类：

| 类型 | 代表 | 难点 |
|---|---|---|
| 不带指针成员 | `Complex` | 运算符重载、`const`、传参返回 |
| 带指针成员 | `String` | 资源管理、深拷贝、析构、自我赋值 |

记忆重点：

- 不带指针成员的类，编译器默认生成的拷贝通常够用。
- 带指针成员的类，必须认真写拷贝构造、拷贝赋值和析构函数。

## 头文件基本结构

头文件要有防卫式声明，避免重复包含。

```cpp
#ifndef COMPLEX_H
#define COMPLEX_H

#include <iostream>

class Complex {
public:
    Complex(double r = 0.0, double i = 0.0);

    double real() const;
    double imag() const;

private:
    double re_;
    double im_;
};

#endif
```

现代 C++ 也常用：

```cpp
#pragma once
```

复习时先掌握传统写法，因为它能帮助理解“防止重复包含”的本质。

## 类声明和类定义

类声明说明“有什么接口”。  
类定义说明“接口怎么实现”。

```cpp
class Complex {
public:
    Complex(double r = 0.0, double i = 0.0) : re_(r), im_(i) {}

    double real() const { return re_; }
    double imag() const { return im_; }

private:
    double re_;
    double im_;
};
```

写在 class body 内部的短函数，会自动成为 `inline` 候选。

## 构造函数

构造函数负责初始化对象。

推荐使用初始化列表：

```cpp
class Complex {
public:
    Complex(double r = 0.0, double i = 0.0)
        : re_(r), im_(i) {}

private:
    double re_;
    double im_;
};
```

为什么优先用初始化列表：

- 成员在进入构造函数函数体前就已经初始化。
- 对 `const` 成员和引用成员，必须用初始化列表。
- 通常比先默认构造再赋值更高效。

## Private 构造函数和 Singleton

如果构造函数放在 `private`，外部无法直接创建对象。可以用于单例模式。

```cpp
class A {
public:
    static A& getInstance() {
        static A instance;
        return instance;
    }

    void setup() {}

private:
    A() = default;
    A(const A&) = delete;
    A& operator=(const A&) = delete;
};

int main() {
    A::getInstance().setup();
}
```

复习重点：

- `static A instance;` 只会初始化一次。
- 禁止拷贝和赋值，避免产生多个实例。

## Const 成员函数

`const` 成员函数承诺不修改对象状态。

```cpp
class Complex {
public:
    double real() const { return re_; }
    double imag() const { return im_; }

private:
    double re_;
    double im_;
};
```

规则：

- `const` 对象只能调用 `const` 成员函数。
- 非 `const` 对象可以调用 `const` 和非 `const` 成员函数。
- 只要函数不修改成员，就尽量加 `const`。

| 对象类型 | 可调用 `const` 函数 | 可调用非 `const` 函数 |
|---|---|---|
| `const` object | 可以 | 不可以 |
| non-const object | 可以 | 可以 |

示例：

```cpp
const Complex c(1, 2);
c.real();      // OK
// c.setReal(3); // Error，如果 setReal 不是 const 函数
```

## 参数传递和返回值

优先级记忆：

1. 小型内置类型，如 `int`、`double`，按值传递。
2. 大对象，优先 `const T&` 传递。
3. 需要修改实参，用 `T&`。
4. 返回局部对象，不要返回引用。
5. 返回 `*this`，可以返回引用。

示例：

```cpp
Complex& Complex::operator+=(const Complex& rhs) {
    re_ += rhs.re_;
    im_ += rhs.im_;
    return *this;
}
```

这里返回 `Complex&`，是因为返回的是当前对象本身，不是局部临时对象。

错误示例：

```cpp
Complex& makeComplex() {
    Complex c(1, 2);
    return c; // 错误：返回局部变量引用
}
```

## Friend 友元

`friend` 允许外部函数访问类的 `private` 成员。

```cpp
class Complex {
public:
    Complex(double r = 0.0, double i = 0.0) : re_(r), im_(i) {}

    friend Complex operator+(const Complex& lhs, const Complex& rhs);

private:
    double re_;
    double im_;
};

Complex operator+(const Complex& lhs, const Complex& rhs) {
    return Complex(lhs.re_ + rhs.re_, lhs.im_ + rhs.im_);
}
```

注意：

- `friend` 会打破封装边界，不要滥用。
- 常用于运算符重载、输出流、需要高效访问内部数据的辅助函数。

## 运算符重载

成员函数形式：

```cpp
class Complex {
public:
    Complex& operator+=(const Complex& rhs) {
        re_ += rhs.re_;
        im_ += rhs.im_;
        return *this;
    }

private:
    double re_;
    double im_;
};
```

非成员函数形式：

```cpp
Complex operator+(const Complex& lhs, const Complex& rhs) {
    Complex result(lhs);
    result += rhs;
    return result;
}
```

输出运算符通常写成非成员函数：

```cpp
std::ostream& operator<<(std::ostream& os, const Complex& c) {
    return os << "(" << c.real() << ", " << c.imag() << ")";
}
```

为什么返回 `std::ostream&`：

- 支持连续输出：`std::cout << a << b;`
- 避免复制流对象。

## Class Template

模板让类支持不同类型。

```cpp
template <typename T>
class Complex {
public:
    Complex(T r = T{}, T i = T{}) : re_(r), im_(i) {}

    T real() const { return re_; }
    T imag() const { return im_; }

private:
    T re_;
    T im_;
};

Complex<double> c1(1.2, 3.4);
Complex<int> c2(1, 3);
```

注意原始笔记中的一个常见错误：模板类里成员类型也应该用 `T`，不要仍然写死成 `double`。

## 带指针成员的类：String

只要类里自己管理资源，就必须考虑三件事：

- 拷贝构造函数
- 拷贝赋值函数
- 析构函数

这就是经典的 Rule of Three。

```cpp
#include <cstring>
#include <iostream>

class String {
public:
    String(const char* cstr = "") {
        if (cstr) {
            size_ = std::strlen(cstr);
            data_ = new char[size_ + 1];
            std::strcpy(data_, cstr);
        } else {
            size_ = 0;
            data_ = new char[1];
            data_[0] = '\0';
        }
    }

    String(const String& other) {
        size_ = other.size_;
        data_ = new char[size_ + 1];
        std::strcpy(data_, other.data_);
    }

    String& operator=(const String& other) {
        if (this == &other) {
            return *this;
        }

        delete[] data_;

        size_ = other.size_;
        data_ = new char[size_ + 1];
        std::strcpy(data_, other.data_);
        return *this;
    }

    ~String() {
        delete[] data_;
    }

    const char* c_str() const {
        return data_;
    }

private:
    char* data_;
    std::size_t size_;
};

std::ostream& operator<<(std::ostream& os, const String& s) {
    return os << s.c_str();
}
```

复习重点：

- 默认拷贝是逐位复制，会导致两个对象指向同一块内存。
- 带指针成员时要做深拷贝。
- 赋值运算符必须处理自我赋值。
- 析构函数必须释放 `new[]` 得到的资源。

## 浅拷贝和深拷贝

浅拷贝：

```text
a.data_ ----> "hello"
b.data_ ----> 同一块 "hello"
```

问题：

- 一个对象析构后，另一个对象的指针悬空。
- 两个对象析构时可能重复释放同一块内存。

深拷贝：

```text
a.data_ ----> "hello"
b.data_ ----> 另一块 "hello"
```

优点：

- 每个对象拥有自己的资源。
- 生命周期互不影响。

## Rule of Three / Five / Zero

| 规则 | 适用场景 |
|---|---|
| Rule of Three | 自己写析构、拷贝构造、拷贝赋值中的一个，通常三个都要写 |
| Rule of Five | C++11 后加上移动构造、移动赋值 |
| Rule of Zero | 尽量使用标准库资源管理类，让自己不写这些函数 |

现代 C++ 更推荐：

```cpp
#include <string>

class Person {
public:
    explicit Person(std::string name) : name_(std::move(name)) {}

private:
    std::string name_;
};
```

因为 `std::string` 自己负责资源管理，所以 `Person` 不需要手写析构和拷贝函数。

## Static 成员

类中常见四类成员：

| 类型 | 归属 |
|---|---|
| 成员变量 | 每个对象各有一份 |
| 静态成员变量 | 整个类共享一份 |
| 成员函数 | 通过对象调用，有 `this` 指针 |
| 静态成员函数 | 通过类调用，没有 `this` 指针 |

示例：

```cpp
class Account {
public:
    static void setRate(double x) {
        rate_ = x;
    }

    static double rate() {
        return rate_;
    }

private:
    static double rate_;
};

double Account::rate_ = 8.0;

int main() {
    Account::setRate(4.0);
}
```

复习重点：

- 静态成员变量需要在类外定义。
- 静态成员函数没有 `this`，不能直接访问普通成员变量。
- 静态成员适合表示“所有对象共享”的状态。

## Stack 和 Heap

| 区域 | 特点 |
|---|---|
| Stack | 自动分配和释放，离开作用域就销毁 |
| Heap | 手动或由智能指针管理，生命周期更灵活 |

```cpp
void f() {
    Complex c1(1, 2);              // stack object
    Complex* p = new Complex(1, 2); // heap object

    delete p;
}
```

注意：

- `new` 出来的对象必须释放。
- `new[]` 对应 `delete[]`。
- `new` 和 `delete` 不要混用数组版本。

## New 和 Delete 的本质

`new Complex(1, 2)` 大致分成三步：

```cpp
void* mem = operator new(sizeof(Complex)); // 分配内存
Complex* p = static_cast<Complex*>(mem);   // 转型
p->Complex::Complex(1, 2);                 // 调用构造函数
```

`delete p` 大致分成两步：

```cpp
p->~Complex();     // 调用析构函数
operator delete(p); // 释放内存
```

如果是数组：

```cpp
String* arr = new String[3];
delete[] arr;
```

`delete[]` 会对数组中每个元素调用析构函数。

## 内存示意图

如果原图仍在附件中，可配合查看：

![[内存分配.png#img_center]]

![[内存分配2.png#img_center]]

复习时不需要死记不同编译器的内存头尾细节，重点记住：

- 对象本身只包含非静态成员变量。
- 动态分配可能有额外管理信息。
- 数组形式需要记录元素个数，方便析构多个对象。

## 类之间的三种关系

### 继承：Inheritance

继承表示 is-a。

```cpp
class Shape {
public:
    virtual double area() const = 0;
    virtual ~Shape() = default;
};

class Circle : public Shape {
public:
    explicit Circle(double r) : r_(r) {}

    double area() const override {
        return 3.14159 * r_ * r_;
    }

private:
    double r_;
};
```

判断标准：

- `Circle` 是一种 `Shape`，所以适合继承。
- 父类如果要被多态删除，析构函数应为 `virtual`。

### 复合：Composition

复合表示 has-a。

```cpp
class Engine {
public:
    void start() {}
};

class Car {
public:
    void start() {
        engine_.start();
    }

private:
    Engine engine_;
};
```

判断标准：

- `Car` 拥有一个 `Engine`。
- 成员对象的生命周期跟随外层对象。

### 委托：Delegation

委托表示通过指针或引用把任务交给另一个对象。

```cpp
class StringRep {
public:
    explicit StringRep(const char* s);
};

class String {
public:
    explicit String(const char* s) : rep_(new StringRep(s)) {}
    ~String() { delete rep_; }

private:
    StringRep* rep_;
};
```

判断标准：

- 外层对象不直接做所有事情。
- 真实工作交给内部实现对象。
- 常用于 Pimpl、共享实现、降低编译依赖。

## 继承、复合、委托怎么选

| 关系 | 判断问题 | 例子 |
|---|---|---|
| 继承 | A 是一种 B 吗 | `Circle` is a `Shape` |
| 复合 | A 拥有 B 吗 | `Car` has an `Engine` |
| 委托 | A 是否把工作交给 B | `String` delegates to `StringRep` |

优先级建议：

1. 能用复合就不要急着用继承。
2. 继承用于稳定的抽象层次。
3. 委托用于隐藏实现和降低耦合。

## 多态

多态依赖：

- 基类指针或引用
- `virtual` 函数
- 运行时动态绑定

```cpp
void printArea(const Shape& s) {
    std::cout << s.area() << std::endl;
}

Circle c(2.0);
printArea(c);
```

复习重点：

- 非虚函数是静态绑定。
- 虚函数是动态绑定。
- 基类析构函数通常要写成 `virtual`。

## This 指针

`this` 是编译器为非静态成员函数隐式传入的对象地址。

```cpp
class Test {
public:
    void set(int x) {
        this->x_ = x;
    }

private:
    int x_;
};
```

记忆重点：

- 普通成员函数有 `this`。
- 构造函数和析构函数也有 `this`。
- 静态成员函数没有 `this`。
- `const` 成员函数中的 `this` 可以理解为 `const T* const this`。

## Object Model：vptr 和 vtbl

当类中存在虚函数时，对象通常会多一个隐藏指针：`vptr`。

```text
object
├── vptr  ---> virtual table
├── data member 1
└── data member 2
```

虚函数表 `vtbl` 中存放虚函数地址。通过基类指针或引用调用虚函数时，程序会根据对象实际类型查表调用。

```cpp
Shape* p = new Circle(2.0);
p->area(); // 运行时调用 Circle::area
delete p;
```

动态绑定成立条件：

1. 基类函数是 `virtual`。
2. 派生类 override 该函数。
3. 通过基类指针或引用调用。

## Reference 复习

引用常用于参数和返回类型。

```cpp
void f1(Complex obj);        // 复制一份
void f2(const Complex& obj); // 不复制，且不修改
void f3(Complex& obj);       // 不复制，可以修改
```

注意：

- `const T&` 是传大对象的常用方式。
- 引用必须初始化。
- 引用本质上像别名，不要返回局部变量引用。
- `void f(double)` 和 `void f(const double&)` 不能作为一组清晰的重载来依赖，调用时容易产生歧义。

## Namespace 复习

命名空间用于避免名字冲突。

```cpp
namespace mylib {
class String {};
}

mylib::String s;
```

三种使用方式：

```cpp
using namespace std; // 打开整个命名空间，不推荐在头文件中使用
using std::cout;     // 只引入一个名字
std::cin >> x;       // 使用完整限定名
```

头文件里不要写 `using namespace std;`，否则会污染包含它的所有文件。

## Template 复习

模板用于把类型参数化。

```cpp
template <typename T>
T maxValue(const T& a, const T& b) {
    return a < b ? b : a;
}
```

常见模板概念：

| 概念 | 作用 |
|---|---|
| 函数模板 | 生成一组函数 |
| 类模板 | 生成一组类 |
| 成员模板 | 类内某个成员函数也是模板 |
| 模板特化 | 为某个具体类型提供特殊实现 |
| 偏特化 | 为一类类型提供特殊实现 |
| 模板模板参数 | 模板参数本身也是模板 |
| 可变参数模板 | 接收任意数量模板参数 |

成员模板示例：

```cpp
template <class T1, class T2>
struct Pair {
    T1 first;
    T2 second;

    template <class U1, class U2>
    Pair(const Pair<U1, U2>& p) : first(p.first), second(p.second) {}
};
```

模板特化示例：

```cpp
template <class Key>
struct Hash {};

template <>
struct Hash<int> {
    size_t operator()(int x) const { return x; }
};
```

模板模板参数要特别注意“参数个数是否匹配”。例如标准库容器常常不只一个模板参数，因为还带 allocator。

## New/Delete 和 Malloc/Free

| 对比 | `malloc/free` | `new/delete` |
|---|---|---|
| 本质 | C 库函数 | C++ 运算符 |
| 返回类型 | `void*` | 具体类型指针 |
| 构造析构 | 不调用 | 会调用 |
| 失败处理 | 返回 `NULL` | 默认抛异常 |
| 数组 | 手动计算大小 | `new T[n]` |

记忆：

- C++ 对象优先用 `new/delete`，现代 C++ 更优先用智能指针和标准库容器。
- 不要用 `free` 释放 `new` 得到的对象。
- 不要用 `delete` 释放 `malloc` 得到的内存。

## 常见易错点

| 错误 | 后果 | 正确做法 |
|---|---|---|
| 忘记 `const` 成员函数 | `const` 对象无法调用 | 查询函数加 `const` |
| 返回局部变量引用 | 悬空引用 | 返回值或返回安全对象引用 |
| 带指针成员但不写拷贝控制 | 浅拷贝、重复释放 | 写 Rule of Three |
| `new[]` 配 `delete` | 析构次数错误 | `new[]` 配 `delete[]` |
| 基类析构非 virtual | 多态删除不完整 | 基类析构写 `virtual` |
| 静态成员变量只声明不定义 | 链接错误 | 类外定义一次 |
| 过度使用 friend | 破坏封装 | 只在必要处使用 |

## 面试/复习问题

- OOP 和 OOD 的区别是什么？
- 为什么构造函数推荐使用初始化列表？
- `const` 成员函数的作用是什么？
- 为什么 `operator<<` 通常写成非成员函数？
- 什么是浅拷贝？什么是深拷贝？
- 为什么带指针成员的类需要析构函数？
- 拷贝构造和拷贝赋值有什么区别？
- `new` 和 `delete` 分别做了哪些事情？
- `static` 成员函数为什么不能访问普通成员变量？
- 继承、复合、委托分别适合什么场景？
- 为什么基类析构函数常常需要是 `virtual`？

## 面试/复习问题答案

### 1. OOP 和 OOD 的区别是什么？

OOP 是面向对象编程，关注“怎么用代码实现对象”。核心是封装、继承、多态。

OOD 是面向对象设计，关注“类应该怎么设计、对象之间怎么协作”。核心是高内聚、低耦合、可扩展。

简单记：

```text
OOD 是设计思想
OOP 是代码实现
```

### 2. 为什么构造函数推荐使用初始化列表？

因为成员变量在进入构造函数函数体之前就已经被初始化了。

如果在函数体里赋值，本质是“先默认初始化，再赋值”：

```cpp
Complex::Complex(double r, double i) {
    re = r;
    im = i;
}
```

初始化列表更直接：

```cpp
Complex::Complex(double r, double i)
    : re(r), im(i) {}
```

而且有些成员只能用初始化列表，例如：

```cpp
const int x;
int& ref;
```

### 3. `const` 成员函数的作用是什么？

`const` 成员函数承诺：这个函数不会修改对象内部状态。

```cpp
double real() const {
    return re;
}
```

作用：

- 让 `const` 对象也能调用这个函数。
- 明确告诉使用者：这个函数只是读取，不会修改对象。
- 帮助编译器检查误修改。

例如：

```cpp
const Complex c(1, 2);
c.real(); // OK
```

如果 `real()` 没有加 `const`，这里就不能调用。

### 4. 为什么 `operator<<` 通常写成非成员函数？

因为左操作数是 `ostream`，不是你的类对象。

```cpp
std::cout << c;
```

等价于：

```cpp
operator<<(std::cout, c);
```

如果写成成员函数，会更像：

```cpp
c.operator<<(std::cout);
```

这不符合正常输出语义。

所以通常写成非成员函数：

```cpp
std::ostream& operator<<(std::ostream& os, const Complex& c) {
    return os << c.real() << "+" << c.imag() << "i";
}
```

返回 `std::ostream&` 是为了支持连续输出：

```cpp
std::cout << c1 << c2 << std::endl;
```

### 5. 什么是浅拷贝？什么是深拷贝？

浅拷贝是只复制指针值，不复制指针指向的内容。

```text
a.data ----> "hello"
b.data ----> 同一块 "hello"
```

问题是两个对象共享同一块内存，析构时可能重复释放。

深拷贝是重新分配一块内存，并复制内容。

```text
a.data ----> "hello"
b.data ----> 另一块 "hello"
```

这样两个对象互不影响，各自管理自己的资源。

### 6. 为什么带指针成员的类需要析构函数？

因为类自己申请了堆内存，就必须自己释放。

```cpp
class String {
private:
    char* data;
};
```

如果构造函数里有：

```cpp
data = new char[100];
```

析构函数里就应该有：

```cpp
~String() {
    delete[] data;
}
```

否则对象销毁时，只会销毁指针变量本身，不会释放它指向的堆内存，造成内存泄漏。

### 7. 拷贝构造和拷贝赋值有什么区别？

拷贝构造是“用已有对象创建新对象”。

```cpp
String s1("hello");
String s2(s1); // 拷贝构造
```

拷贝赋值是“两个已经存在的对象之间赋值”。

```cpp
String s1("hello");
String s2("world");
s2 = s1; // 拷贝赋值
```

简单记：

```text
拷贝构造：对象还没出生，用别人初始化自己
拷贝赋值：对象已经存在，把自己的内容改成别人
```

拷贝赋值还需要特别处理自我赋值：

```cpp
if (this == &rhs) {
    return *this;
}
```

### 8. `new` 和 `delete` 分别做了哪些事情？

`new` 做两件事：

```text
1. 分配内存
2. 调用构造函数
```

大致相当于：

```cpp
void* mem = operator new(sizeof(Complex));
Complex* p = static_cast<Complex*>(mem);
p->Complex::Complex(1, 2);
```

`delete` 也做两件事：

```text
1. 调用析构函数
2. 释放内存
```

大致相当于：

```cpp
p->~Complex();
operator delete(p);
```

数组要对应：

```cpp
new[] -> delete[]
new   -> delete
```

### 9. `static` 成员函数为什么不能访问普通成员变量？

因为 `static` 成员函数属于类，不属于某个具体对象。

普通成员变量属于对象，每个对象都有自己的一份。

```cpp
class Account {
private:
    double balance;
    static double rate;
};
```

`balance` 必须依赖某个具体对象：

```cpp
Account a;
// a.balance;
```

但 `static` 函数没有 `this` 指针，不知道你要访问哪个对象的 `balance`。

所以它只能直接访问 `static` 成员变量。

### 10. 继承、复合、委托分别适合什么场景？

继承表示 is-a：

```text
Circle is a Shape
```

适合表达“子类是一种父类”。

```cpp
class Circle : public Shape {};
```

复合表示 has-a：

```text
Car has an Engine
```

适合表达“一个类拥有另一个类对象”。

```cpp
class Car {
private:
    Engine engine;
};
```

委托表示把任务交给另一个对象：

```text
String delegates to StringRep
```

适合隐藏实现、降低依赖、共享实现。

```cpp
class String {
private:
    StringRep* rep;
};
```

简单记：

```text
继承：我是你
复合：我有你
委托：我让你帮我做
```

### 11. 为什么基类析构函数常常需要是 `virtual`？

如果你会通过基类指针删除派生类对象，基类析构函数必须是 `virtual`。

例如：

```cpp
Shape* p = new Circle();
delete p;
```

如果 `Shape` 的析构函数不是虚函数，可能只调用 `Shape` 的析构函数，不调用 `Circle` 的析构函数，导致派生类资源没有正确释放。

正确写法：

```cpp
class Shape {
public:
    virtual ~Shape() = default;
};
```

只要一个类打算作为多态基类使用，析构函数通常就应该写成 `virtual`。

## 最小复习模板

复习一个 C++ 类时，按这个顺序检查：

```text
1. 类负责什么？
2. 成员变量是否应该 private？
3. 构造函数是否完成初始化？
4. 查询函数是否加 const？
5. 是否管理资源？
6. 如果管理资源，Rule of Three/Five 是否完整？
7. 参数是否用 const reference？
8. 返回值是否安全？
9. 是否需要 static / friend / operator overload？
10. 类之间关系是继承、复合还是委托？
```
