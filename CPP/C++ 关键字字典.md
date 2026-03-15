## 📚 1. 类与类型系统 (Classes & Type System)

### explicit

**功能**：禁止构造函数的**隐式自动转换**。

- **作用对象**：通常用于修饰**单参数**构造函数。
- **背景**：默认情况下，`Date d = 2023;` 会触发隐式转换，即：`2023` $\rightarrow$ `Date(2023)临时对象` $\rightarrow$ `拷贝/移动构造给 d`。
- **加了 explicit 后**：必须显式调用构造函数 `Date d(2023);`，否则编译报错。
```cpp
class Date {
public:
    explicit Date(int year) : _year(year) {} // 禁止 Date d = 2023;
private:
    int _year;
};
```
![[隐式类型转换过程.png]]
**隐式类型转换**，我们经常会遇到，例如上图我们对一个浮点变量赋值一个整型；对一个对象赋值一个数字。
- 这个过程就会创建一个被赋值对象的临时变量，用提供的值来初始化
- 在此记住对象的初始化为` Data d1(2022); ` 而通过隐式类型转化的初始化时 `Data d2 = 2023`

### override

**功能**：安全重写虚函数。

- **核心价值**：它是编译器的**安全检查哨**。显式声明该函数是为了重写基类的虚函数。    
- **为什么需要**：如果手误写错了函数签名（例如参数类型 `int` 写成 `float`，或者漏掉 `const`），没有 `override` 时编译器会认为你创建了一个新函数，导致多态失效。有了 `override`，编译器会直接报错。
```cpp
class Base {
    virtual void func(int x) {}
};

class Derived : public Base {
    void func(float x) override {} // ❌ 编译报错：参数不匹配，并非重写
    void func(int x) override {}   // ✅ 正确
};
```
### using、 typename、 typedef

- **typedef** (老派的别名工具（C 语言遗老）)：
    - 给一个又长又臭的类型起一个短一点、好记一点的“**绰号**”（别名）
    - 语法有点像变量声明，容易混淆
    
- **using** (C++11 现代的别名工具)：
    - **定义别名**：比 `typedef` 更清晰，支持模板别名。
    - 例：`using IntPtr = int*;`
        
- **typename** (模板编程中的“类型消除歧义符”（告诉编译器这也是个类型）)：
	- **消除歧义**：在模板中，告诉编译器某个嵌套名称是**类型**而不是静态变量。
    - 场景：当依赖模板参数 `T` 时（如 `T::iterator`），必须加 `typename`。

🚀 既然**typedef和using实现的功能是一样的**，那么我们都使用一下，看一下两者的区别

1. 变量别名
```cpp
// 基础语法  
typedef unsigned long ulong; // 把 unsigned long 给个绰号叫 ulong
typedef std::shared_ptr<MyLongClassName> MyPtr; // 把 std::shared_ptr<MyLongClassName> 给个绰号叫 MyPtr

// 绰号 = 原名；（逻辑非常直观）
using ulong = unsigned long;
using MyPtr = std::shared_ptr<MyLongClassName>;
```
2. 函数指针别名
```cpp
// 函数指针别名
typedef void (*HandlerFunc)(int, int); // HandlerFunc 是别名 依然很难看，绰号夹在中间。
using HandlerFunc = void (*)(int, int); // 名字在左，类型在右
```
3. 模板变量别名
```cpp
// ✅ 使用 using (模板别名)
template <typename T>
using StringMap = std::map<std::string, T>;

StringMap<int> inventory; // 相当于 std::map<std::string, int>
StringMap<float> prices;  // 相当于 std::map<std::string, float>

// ❌ 使用 typedef (做不到！)
// 编译器会报错，typedef 没法带 template 参数。
```
综合来看using语法更为清晰，最重要的是`typedef` 不支持模板化，而 `using` 支持。
**💡 生存指南建议：** **喜新厌旧是美德。** 除非你在维护 10 年前的老代码，否则**一律使用 `using`**。它更清晰、更强大。

🚀 `typename` (模板里的捣乱分子)

typename作用就是声明成员变量是个类型，主要有两种用法
- 声明模板参数（作用和class一样）
```cpp
template <typename T> // 推荐用 typename，语义更准确
void func(T t) { ... }

template <class T>    // 和上面完全一样，旧习惯
void func(T t) { ... }
```
- 消除歧义（编译器很笨。当你在一个模板里访问另一个类型的内部成员时，编译器不知道那个成员**是“一个变量”还是“一个类型”**。）
```cpp
template <typename T>
void func() {
    // 编译器看到 T::iterator 时会困惑：
    // 情况 1: iterator 是 T 类里的一个【静态变量】？那这就变成了 乘法运算 (变量 * p)
    // 情况 2: iterator 是 T 类里的一个【类型】？那这就是 声明一个指针 p
    T::iterator * p; 
    
    // 告诉编译器：T::iterator 是个类型，请把它当声明语句处理 
    typename T::iterator * p;
}
```
**ROS 实战场景**： 你在写通用的算法，要处理 `std::vector` 或 `std::list` 的迭代器时：
```cpp
template <typename Container>
void printFirst(const Container& c) {
    // 必须加 typename，因为 const_iterator 是依赖于模板参数 Container 的类型
    typename Container::const_iterator it = c.begin();
    // ...
}
```

| **关键字**        | **核心作用**                   | **工程师评价**                            | **典型 ROS 场景**                           |
| -------------- | -------------------------- | ------------------------------------ | --------------------------------------- |
| **`typedef`**  | 定义类型别名 (C 风格)              | 👴 **过时**。虽然很常见，但写起来不直观。             | 旧的 C 库回调定义，或者 ROS1 的旧代码。                |
| **`using`**    | 定义类型别名 (C++11)             | 🚀 **推荐**。语法清晰 (`name = type`)，支持模板。 | `using Ptr = std::shared_ptr<MyClass>;` |
| **`typename`** | 1. 声明模板参数<br>2. **标示嵌套类型** | 🔧 **必修**。用于告诉编译器“这是个类型不是变量”。        | 泛型编程，或者在模板里使用迭代器时。                      |
**一句话总结：** 平时写别名用 **`using`**；写模板时参数用 **`typename`**；如果模板里报错说“缺少 typename”，就在那行代码的类型前加个 **`typename`** 试试。


---
## ⚠️ 2. 异常处理 (Exception Handling)

### noexcept

**功能**：标识或检查函数是否抛出异常。

1. **作为说明符**：`void func() noexcept;` 承诺函数不抛出异常。如果违规抛出，程序直接调用 `std::terminate()` 崩溃，不会进行栈展开（Stack Unwinding），因此编译器可以优化代码。
2. **作为运算符**：`noexcept(expr)` 在编译期检查表达式是否不抛出异常，返回 `true/false`。
3. **用法**
   - `返回类型 函数名(参数列表) noexcept;  // 简单形式`
   - `返回类型 函数名(参数列表) noexcept(常量表达式);  // 条件形式`

```cpp
// 如果c.pop_back()没有报错 则pop不会报错
void pop() noexcept(noexcept(c.pop_back())) /* strengthened */ {
    c.pop_back();
}
```
### **try / catch / throw**

标准异常处理流程可以参照[[C++生存指南]] #栈展开 ， 主要用到以下3中关键字

- **throw**：抛出异常对象（发现问题的哨兵）。
- **try**：包裹可能出错的代码块（防爆盾/试验田）。
- **catch**：捕获并处理特定类型的异常（排爆专家）。

🔧 throw 是吹哨人，发现问题就报警；try 是防火墙，把危险代码圈起来；catch是清洁工，处理报警后的烂摊子，保证程序不崩

**⚠️ 铁律 1：别在析构函数里 `throw`** 析构函数是用来清理战场的（RAII）。如果析构函数再扔出手雷，而此时外面已经有一个手雷在飞（双重异常），C++ 会直接崩溃（`std::terminate`）。
 
**⚠️ 铁律 2：按资排辈写 `catch`** `catch` 是按顺序匹配的。
 - **先写子类**：`catch (std::runtime_error)`
 - **后写父类**：`catch (std::exception)` 
 - **最后写万能**：`catch (...)` 如果你把 `catch (...)` 写在第一个，后面的特定错误处理就永远抓不到了。

**⚠️ 铁律 3：配合 RAII 食用** 当 `throw` 发生时，程序会跳出当前作用域。
 - 如果你用了裸指针 `new`，还没来得及 `delete` 就抛异常了 -> **内存泄漏**。  
 - 如果你用了 `std::shared_ptr` 或 `std::lock_guard` -> **自动安全释放**。 **结论：异常处理必须搭配智能指针使用。**


标准异常体系 (std::exception)

| **异常类型**                | **描述**               |
| ----------------------- | -------------------- |
| **std::logic_error**    | **逻辑错误** (理论上编程时可避免) |
| `std::invalid_argument` | 参数无效                 |
| `std::domain_error`     | 数学域错误                |
| `std::out_of_range`     | 越界 (如 `vector::at`)  |
| **std::runtime_error**  | **运行时错误** (难以预料)     |
| `std::overflow_error`   | 数学上溢                 |
| `std::range_error`      | 计算结果超范围              |
| **std::bad_alloc**      | `new` 失败 (内存不足)      |
示例
```cpp
#include <iostream>
#include <stdexcept> // 必须包含这个，里面有 standard exceptions

// 1. 定义一个会【抛出】异常的函数
void connectToCamera(int camera_id) {
    if (camera_id < 0) {
        // 发现 ID 不对，直接扔出一个 runtime_error
        // throw 后面可以是任何类型，但建议用 std::exception 的子类
        throw std::runtime_error("Invalid Camera ID!"); 
    }
    
    if (camera_id > 10) {
        throw std::out_of_range("Camera ID out of range!");
    }

    std::cout << "相机 " << camera_id << " 连接成功！" << std::endl;
}

int main() {
    // 2. 【尝试】运行危险代码
    try {
        connectToCamera(1);  // 正常
        connectToCamera(-5); // 💣 这一行会抛出异常！
        
        // ⚠️以此为界：上面抛出异常后，程序直接跳到 catch，
        // 下面这行代码【永远不会】被执行！
        std::cout << "这一行看不见..." << std::endl;
    }
    // 3. 【捕获】特定类型的异常
    catch (const std::out_of_range& e) {
        std::cerr << "捕获到越界错误: " << e.what() << std::endl;
    }
    catch (const std::runtime_error& e) {
        std::cerr << "捕获到运行时错误: " << e.what() << std::endl;
        // 在 ROS 里，这里通常是 ROS_ERROR(e.what());
    }
    // 4. 【捕获】所有漏网之鱼 (万能捕获)
    catch (...) {
        std::cerr << "捕获到了未知错误！" << std::endl;
    }

    std::cout << "程序仍然活着，继续执行其他任务..." << std::endl;
    return 0;
}
```

### static_cast

`static_cast` 是 C++ 中**最常用、最基础的类型转换运算符**，属于**静态类型转换**（编译期完成转换检查），设计目的是替代 C 语言风格的强制转换（如 `(int)3.14`），让转换更安全、可读性更高，仅适用于编译器能判定的 “合理且安全” 的类型转换场景。

`static_cast` 在**编译阶段**完成类型转换，编译器会检查转换的合法性，但**不做运行时类型验证**（这是它和 `dynamic_cast` 的核心区别）。它适用于 “逻辑上相容” 的类型转换，比如基本数据类型互转、类的向上转型等。
```cpp
static_cast<目标类型>(源表达式/变量)
```

- 目标类型：你想要转换成的类型（如 `int`、`double*`、`Base&` 等）；
- 源表达式：待转换的变量、常量或表达式。

应用场景：
- **基本数据类型之间的转换**： 隐式转换的 “显式化”、合理的 “窄化转换”
```cpp
int num = 10;
double d_num = static_cast<double>(num); // int → double
int i_num = static_cast<int>(d_num); // double → int
cout << static_cast<int>('a') << endl; // char → int
```
- **类的向上转型（派生类 → 基类）**：派生类指针 / 引用转换为基类指针 / 引用（“向上转型”），属于 `is-a` 关系
```cpp
class Base{
public:
	int x = 0;
};
class Drived : public Base{
public:
	int y = 10;
};
Drived d;// 派生类对象
Base* d1 = static_cast<Base*>(&d);// 场景1：派生类指针 → 基类指针（向上转型）
Base& d2 = static_cast<Base&>(d);// 场景2：派生类引用 → 基类引用
```
- **void* 与其他类型指针的转换**
```cpp
int a = 10;
void* a_ptr = &a;
int* a_ptrr = static_cast<int*>(a_ptr);
```
- **显式调用类的转换函数 / 单参数构造函数**
```cpp
class MyNum
{
public:
	int val;
	MyNum(int a = 0) : val(a) {}
	operator int() { return this->val; }
};

MyNum num = static_cast<MyNum>(12);
int intV = static_cast<int>(num);
```
- **空指针（nullptr）转换为任意类型的指针**
```cpp
int* a = static_cast<int*>(nullptr);
base* b = static_cast<base*>(nullptr);
```

重点避坑点：
- **不能移除 `const/volatile` 修饰符**
```cpp
const int a = 10;
double b = static_cast<double>(a);  // ❌ 不能移除const属性
```
- **类的向下转型（基类 → 派生类）不安全**

`static_cast` 允许基类指针 / 引用转换为派生类指针 / 引用（“向下转型”），但**无运行时检查**—— 如果基类指针指向的不是派生类对象，访问派生类独有成员会直接崩溃
```cpp
Base* b_ptr = new Base();
Drived* d_ptr = static_cast<Drived*>(b_ptr); // // 编译通过，但逻辑错误：b_ptr 不指向 Derived 对象
// 运行时崩溃：d_ptr->y 访问不存在的成员（b_ptr 只有 x，没有 y） 
// cout << d_ptr->y << endl;
```
- **不能转换无关的指针类型**
`static_cast` 禁止在无继承关系、无逻辑关联的指针类型间转换（如 `int*` ↔ `double*`），这是它比 C 风格转换更安全的核心
```cpp
int* int_ptr = new int(213);
double* double_ptr = static_cast<double>(int_ptr); // ❌ 不能无关系的转换
```
- **不支持函数指针 / 成员函数指针的转换**
`static_cast` 不能用于函数指针、类成员函数指针的转换（需用 `reinterpret_cast`）。

### dynamic_cast

`dynamic_cast` 是 C++ 中专门用于**类的指针 / 引用类型转换**的**动态类型转换运算符**—— 它的核心特点是**运行期检查类型合法性**，而非编译期，这也是它和 `static_cast` 最本质的区别。`dynamic_cast` 主要用于实现安全的 “向下转型”（基类→派生类），是多态场景下类型转换的 “安全卫士”。

`dynamic_cast` 在**程序运行阶段**检查待转换的指针 / 引用**实际指向的对象类型**，判断是否能安全转换为目标类型：

- 转换合法（如基类指针实际指向派生类对象）→ 转换成功，返回目标类型的指针 / 引用；
- 转换非法（如基类指针指向基类对象）→ 指针返回 `nullptr`，引用抛出 `std::bad_cast` 异常。
**仅适用于包含虚函数的类**（因为它依赖 C++ 的 RTTI 机制（运行时类型信息），而 RTTI 仅对有虚函数的类生效）；若类无虚函数，使用 `dynamic_cast` 会直接编译报错。
```cpp
dynamic_cast<目标类指针类型>(源指针); // 指针转换
dynamic_cast<目标类引用类型>(源引用); // 引用转换;
```

应用场景
- **基类指针 → 派生类指针（向下转型，核心场景）**
需要注意的基类指针指向对象不是基类对象，而是一个派生类（指针可以转化的范围为派生类及以上类的指针）

```cpp
Base* ptr = new Drived1();
Drived1* ptr1 = dynamic_cast<Drived1*>(ptr); // ✅
Drived2* ptr2 = dynamic_cast<Drived2*>(ptr); // ❌ 
Base* ptrr = new Base();
Drived1* ptr1 = dynamic_cast<Drived1*>(ptrr); // ❌
```
- **基类引用 → 派生类引用（向下转型）**
引用没有 “空引用” 的概念，因此 `dynamic_cast` 转换引用失败时，会抛出 `std::bad_cast` 异常
和指针原理相同，注意指向对象的继承关系
```cpp
Drived1 obj1;
Base& obj1_ref = obj1;
try{
	Drived1& obj1_reff = dynamic_cast<Drived1&>(obj1_ref); // ✅
}catch(const bad_cast& e){
	cout << "转化失败：" << e.what() << endl;
}
Base obj2;
Base& obj2_ref = obj2;
try{
	Drived2& obj2_reff = dynamic_cast<Drived2&>(obj2_ref); // ❌
}catch(const bad_cast& e){
	cout << "转化失败：" << e.what() << endl;
}
```
- **转换为 void*（获取对象实际起始地址）**
`dynamic_cast` 可将任意类指针转为 `void*`，返回的是对象**实际类型**的起始地址（而非基类子对象地址），这是一个特殊且实用的场景：
```cpp
Derived1 d1_obj; 
Base* b_ptr = &d1_obj;
void* void_ptr = dynamic_cast<void*>(b_ptr);
```
- **派生类 → 基类（向上转型）**
`dynamic_cast` 也支持向上转型（和 `static_cast` 效果一致），但完全没必要
```cpp
Derived1 d1_obj; // 向上转型：合法，但不如 static_cast 高效 
Base* b_ptr = dynamic_cast<Base*>(&d1_obj);
```
- **识别对象的实际类型**
```cpp
void checkType(Base* b_ptr) {
    if (dynamic_cast<Derived1*>(b_ptr)) {
        cout << "实际类型：Derived1" << endl;
    } else if (dynamic_cast<Derived2*>(b_ptr)) {
        cout << "实际类型：Derived2" << endl;
    }
}
```
- **交叉转型（兄弟类之间的转换）**
```cpp
Derived1* d1_ptr = new Derived1();
Base* b_ptr = d1_ptr; // 先向上转型为基类
    
// 交叉转型：Derived1 → Derived2（通过基类中转）
Derived2* d2_ptr = dynamic_cast<Derived2*>(b_ptr);
if (d2_ptr == nullptr) {
    cout << "交叉转型失败（实际是Derived1）" << endl;
}
```
dynamic_cast 的核心价值 ——**在类型未知的多态场景下，保证转换的安全性**。
并不是像静态转化那样，我已知几种类的继承关系
往往的情况是直接传进来一个指针或者引用，我们并不知道指向的是那个对象

---
## 🧬 3. 泛型与现代特性 (Generics & Modern Features)

### Templates (模板)

💥 痛点分析：相同的功能，逻辑完全相同，只是数据类型发生了改变
**✅ 模板的各种美好：** 我只写一份“模具”，把**类型**留个空位（占位符），让编译器帮我填！
**功能**：泛型编程的基础，生成类或函数的蓝图。

- **函数模板**：`template <typename T> T add(T a, T b) { ... }` 
- **类模板**：`template <class T> class Box { ... };`
	- **`template`**：告诉编译器，下面这坨代码是模板。   
	- **`<...>`**：尖括号里放参数。
	- **`typename T`**：定义一个“类型参数”叫 `T`。
    - `T` 只是个名字，你可以叫 `Type`, `Data`, `U`, `V`，但习惯上用 `T`。

- _有时候你会看到 `template <class T>`，它和 `<typename T>` **完全通用，没区别**。_

🔧 应用：
1. 发布者/订阅者
	`ros::Publisher pub = nh.advertise<std_msgs::String>("topic", 10);`
	`<std_msgs::String>` 就是模板参数，告诉 `advertise` 函数：“我要发布的消息类型是 `String`”
2. **PCL 点云库 (模板重灾区)** PCL 是 C++ 模板使用的巅峰。因为点云有很多种（有的只有 `XYZ`，有的有 `RGB`，有的有法线）。
	- `pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);`
	- `pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud_rgb(new pcl::PointCloud<pcl::PointXYZRGB>);`
	- 分别定义了只有坐标的点云和带颜色的点云

#### ⚠️ 工程师必读：模板的“分身”原理与代价

**原理：编译期实例化 (Instantiation)** 模板**不是**真正的代码，它只是**图纸**
- 当你没用到 `max<int>` 时，编译器根本不会生成 `int` 版的 `max` 函数代码。
- 当你写了 `max<int>(1, 2)`，编译器在编译时，会根据模板，**现场抄写**一份 `int` 版的代码。
- 当你又写了 `max<float>(1.1, 2.2)`，编译器再**现场抄写**一份 `float` 版的代码。
_这就是“多重影分身”。_

**代价 (Cons)：**
1. **代码膨胀 (Code Bloat)**：如果你用了 100 种不同类型的 `Box<T>`，最终生成的二进制文件里就会有 100 个 Box 类的定义，程序体积会变大。
2. **编译慢**：因为每次都要现场生成代码，PCL 库编译慢就是因为模板太多了。
3. **报错天书**：模板报错是 C++ 程序员的噩梦。报错信息往往几百行，因为编译器展示的是展开后的内部错误。

#### 💀 致命陷阱：模板不能分文件写！

请在笔记里画个骷髅头！这是新手必踩的坑。

- **普通类**：声明写 `.h`，实现写 `.cpp`。✅
- **模板类**：声明和实现**必须都写在 `.h` 头文件里**！❌ 不能分开！

**为什么？**（可以查看[[程序开发知识]] #编译链接过程)

因为模板需要在编译时“现场抄写”代码, 根据模板现场写给定类型的函数实现。如果实现写在 `.cpp` 里，编译器在处理 `main.cpp` 时看不到模板的具体实现逻辑，就没法抄写生成代码，最后会报 **"Undefined Reference"** 错误。

> **解决方案：**
> 1. 全写在 `.h` 里。
> 2. 或者写在 `.hpp` 文件里（专门约定俗成给模板用的后缀）。

#### **🚀 总结**

> **💡 Templates 一句话攻略：**
> 
> 1. 它是 **“类型参数化”** 的工具。把类型当变量传进去。
> 2. 看到尖括号 `< >`，就要反应过来：“这是在指定模板类型”。
> 3. **自己写模板时**：代码全部塞进头文件，别分 `.cpp` 写。
> 4. **阅读源码时**：`template <typename T>` 里的 `T` 就是个未知数 $x$，带入你实际使用的类型去理解就行。


### Lambda 表达式

**功能**：定义匿名函数对象，常用于回调或 STL 算法。 
**语法**：`[捕获列表](参数列表) mutable -> 返回类型 { 函数体 }`
- **`[]` 捕获列表 (Capture Clause)**：**灵魂所在**。决定了 Lambda 内部如何访问**外部**的变量（是拷贝还是引用？）。
- **`()` 参数列表 (Parameters)**：和普通函数一样。
- **`->` 返回值 (Return Type)**：通常可以省略，编译器会自动推导。
- **`{}` 函数体 (Body)**：具体的逻辑。
- 默认情况下，`[=]` 捕获进来的变量是 **const (只读)** 的。如果你非要在 Lambda 内部修改这个副本，需要加 `mutable`。

```cpp
int base = 2;
// 捕获 base 用于计算
auto logic = [base](int a) { return a % base; };
```
**捕获列表**：

|**写法**|**含义**|**潜台词**|**场景**|
|---|---|---|---|
|**`[]`**|**不捕获**|“我与世隔绝，只用传进来的参数。”|纯函数计算|
|**`[=]`**|**按值捕获所有**|“把外面的变量全都**复印**一份给我。原来的变了跟我没关系。”|安全，但拷贝有开销|
|**`[&]`**|**按引用捕获所有**|“我拿着外面变量的**遥控器**。我改了，外面也变。”|**快，但危险！** (见下文)|
|**`[this]`**|**捕获当前对象**|“我要访问类的成员变量和函数。”|**ROS 回调必用**|
|**`[x, &y]`**|**混合捕获**|“x 拷贝一份，y我要引用。”|精细化控制|

**几种应用场景**：

- ROS 2 定时器 (访问类成员)
```cpp
// 在类的构造函数里
timer_ = this->create_wall_timer(
    500ms, 
    [this]() { // 👈 注意这里捕获了 this
        this->count_++; // 访问成员变量
        RCLCPP_INFO(this->get_logger(), "Count: %d", this->count_);
    }
);
```

- 多线程 (std::thread)
```cpp
int data = 100;
// 启动一个线程，去后台修改 data
std::thread t([&data]() { // 👈 必须按引用捕获，否则改的是副本！
    data = 200; 
    std::cout << "后台任务完成" << std::endl;
});
t.join();
std::cout << "Main: " << data << std::endl; // 输出 200
```

### std::function

#### 核心痛点：为什么需要它？

在 C++ 中，"能被调用的东西"（Callable）长得千奇百怪，这给编程带来了很大的困扰：

1. **普通函数**：`void print(int a)`
2. **函数指针**：`void (*func_ptr)(int)`
3. **Lambda 表达式**：`[](int a) { ... }`
4. **类成员函数**：`obj.print(int a)` （最麻烦，因为带有 `this` 指针）
5. **仿函数 (Functor)**：重载了 `operator()` 的类对象。
    
**痛点**：如果你想写一个通用的任务管理器 `TaskScheduler`，想把上面这些乱七八糟的东西都存进同一个 `std::vector` 列表里，该怎么办？
- 用函数指针？存不了 Lambda 和类成员。
- 用模板？类型太复杂，很难存进同一个容器。
    
**✅ 解决方案**：`std::function`。 不管你本来是什么（普通函数、Lambda、类方法），只要你的**输入参数**和**返回值**符合要求，我都能把你“打包”装进这个统一的容器里。

#### 语法解剖

- **头文件**：`#include <functional>`
- **格式**：`std::function< 返回值类型 (参数类型1, 参数类型2...) >`
```cpp
// 它可以存一个：返回 void，接收两个 int 的任何东西
std::function<void(int, int)> callback;
```
应用举例
```cpp
#include <iostream>
#include <functional>
#include <vector>

// 定义一个“万能容器”类型，它要求：返回 void，接收 int
using CallbackType = std::function<void(int)>;

// 1. 普通函数
void normalFunction(int x) {
    std::cout << "我是普通函数: " << x << std::endl;
}

// 2. 类成员函数 (ROS 中最常见！)
class Robot {
public:
    void move(int speed) {
        std::cout << "我是机器人成员函数，速度: " << speed << std::endl;
    }
};

int main() {
    CallbackType func; // 定义一个空的容器

    // -------------------------------------------
    // 情况 A: 存普通函数
    func = normalFunction;
    func(10); // 调用就像调函数一样

    // -------------------------------------------
    // 情况 B: 存 Lambda 表达式 (现代 C++ 首选)
    func = [](int x) {
        std::cout << "我是 Lambda: " << x << std::endl;
    };
    func(20);

    // -------------------------------------------
    // 情况 C: 存类成员函数 (ROS 经典场景)
    Robot my_robot;
    
    // ⚠️ 注意：成员函数不能直接赋值！
    // 因为成员函数隐含了一个参数：this 指针。
    // 必须使用 Lambda 包装一下（推荐），或者用 std::bind
    func = [&my_robot](int x) { 
        my_robot.move(x); 
    };
    func(30);
    
    // -------------------------------------------
    // 情况 D: 放入容器
    std::vector<CallbackType> task_list;
    task_list.push_back(normalFunction);
    task_list.push_back(func); // 把刚才的 lambda 放进去
    
    return 0;
}
```

#### 工程师视角的“代价” (Performance Note)

虽然它好用，但不是免费的。

- **Type Erasure (类型擦除)**：为了能装下各种不同类型的函数，`std::function` 内部使用了一种叫“类型擦除”的技术（通常包含虚函数调用）。这意味着它比普通的函数指针调用要**稍微慢一点点**（指针跳转开销）。
- **内存分配**：如果存进去的 Lambda 捕获了太多变量（比如 `[=]` 捕获了一个大数组），`std::function` 可能会在堆（Heap）上分配内存。
- **对象大小**：`std::function` 对象本身通常比一个指针大（通常是 32 或 48 字节）。
    

> **💡 结论**： 在 **`1000Hz` 以上的硬实时控制循环**或者**图像处理像素级循环**中，尽量少用 `std::function`（可以用模板代替，虽然代码会难写）。
> 但在 **消息回调、状态机切换、任务调度** 等绝大多数 ROS 业务逻辑中，**放心大胆地用**，这点损耗对于毫秒级的任务来说完全可以忽略不计。


---
## 🛠️ 4. STL 工具与内存管理 (STL & Memory)

### **std::pair**

有时候，你不想为了存两个相关的数据（比如坐标 x 和 y，或者 名字 和 分数）专门去写一个 `struct` 或 `class`。这时候，`std::pair` 就是你的救星。

- **特点**：只有两个成员变量，永远叫 `first` 和 `second`。
- **灵活性**：这两个成员的类型可以不一样（比如一个 `int` 一个 `string`）。
- **功能**：将两个可能不同类型的数据“捆绑”在一起。
	- 场景：函数需返回两个值、Map 的键值对。
	- 访问：`p.first`, `p.second`。
	- 构造：`make_pair(v1, v2)` 可以自动推导类型。

极速创建法：make_pair

在 C++11 之前，写 `std::pair<int, std::string>(1, "a")` 太啰嗦了。 大家更喜欢用 `std::make_pair`，因为它能**自动推导类型**。
```cpp
// 编译器自动看出这是 <int, float>
auto data = std::make_pair(1, 3.14f); 

// 在函数传参时特别好用
void process(std::pair<int, int> p) { ... }

// 调用时直接造一个
process(std::make_pair(10, 20));
```


**ROS 与 C++ 中的高频应用场景**

**场景 A：代替 Struct 做简单的返回值** 有些函数需要返回两个结果（比如：是否成功 + 错误信息）。写个结构体太麻烦，直接返回 pair。

```cpp
// 返回值：<是否成功, 错误信息>
std::pair<bool, std::string> checkSensor() {
    if (sensor_ok) return std::make_pair(true, "OK");
    else return std::make_pair(false, "Connection Failed");
}

int main() {
    auto result = checkSensor();
    if (result.first) {
        // ...
    }
}
```

**场景 B：std::map 的基石 (重点)** 这是你在 ROS 代码中最容易遇到 pair 的地方。 **`std::map` (字典/哈希表) 里的每一个元素，本质上就是一个 `std::pair`。**
- `first` = **Key (键)**    
- `second` = **Value (值)**

```cpp
std::map<std::string, int> robot_params;
robot_params["speed"] = 5;
robot_params["battery"] = 90;

// 遍历 map 时，iter 就是一个指向 pair 的迭代器
for (auto& item : robot_params) {
    // item.first 是 "speed", "battery"
    // item.second 是 5, 90
    std::cout << item.first << ": " << item.second << std::endl;
}
```

**场景 C：简单的 2D 坐标** 虽然 ROS 有 `geometry_msgs/Point`，但在做简单的算法（比如 A* 寻路的格子索引）时，用 `pair<int, int>` 存 `(x, y)` 更加轻量级。

### **std::shared_ptr 与 make_shared**

**功能**：共享所有权的智能指针。(智能指针参考[[C++生存指南]] #智能指针)

- **原理**：内部维护**控制块 (Control Block)**，包含：
    
    - **强引用计数** (Strong Count)：为 0 时析构对象。
        
    - **弱引用计数** (Weak Count)：为 0 时释放控制块内存。
        
- **make_shared 优势**：
    
    - **一次分配**：将“对象内存”和“控制块内存”合并为一次 `new` 分配，效率更高，且防止内存碎片。
        
    - **异常安全**：防止 `new T()` 完成但 `shared_ptr` 构造前抛异常导致的内存泄漏。
        

### **std::for_each**

**功能**：遍历容器并对每个元素执行操作。

- **特点**：比 range-based for 循环更灵活之处在于它可以返回最终的函数对象（如果函数对象有状态）。
    
- **配合**：常与 Lambda 搭配使用。

```cpp
std::vector<int> nums = {1, 2, 3};
std::for_each(nums.begin(), nums.end(), [](int &n){ n++; }); // 原地修改
```

### `std::bind`

`std::bind` 是 C++11 引入的一个**函数适配器 (Function Adapter)**，定义在 `<functional>` 头文件中。 它的核心作用是**“部分套用” (Partial Application)**：将一个函数的某些参数“固定”下来（绑定死），从而生成一个新的可调用对象。

简单来说，如果你有一个需要 3 个参数的函数，你可以用 `bind` 提前填好其中 1 个参数，然后生成一个新的只需要传 2 个参数的函数。

要使用 `std::bind`，核心在于理解 **占位符 (Placeholders)**。

- **固定参数**：直接传入具体的值。
- **占位符**：`std::placeholders::_1`, `_2`, `_3`... 表示新函数被调用时传入的第 1、第 2、第 3 个参数。

```cpp
#include <functional>
#include <iostream>

using namespace std::placeholders; // 方便使用 _1, _2

void func(int a, int b, int c) {
    std::cout << "a=" << a << ", b=" << b << ", c=" << c << std::endl;
}

int main() {
    // 场景：我们需要把 func 的第一个参数 a 固定为 100
    // 生成一个新函数 new_func，它只需要两个参数
    auto new_func = std::bind(func, 100, _1, _2);
    
    // 调用 new_func(20, 30)
    // 实际执行 func(100, 20, 30)
    new_func(20, 30); 
    
    return 0;
}
```
`_1`, `_2`, `_3` ... 是定义在 `std::placeholders` 命名空间下的特殊对象。 它们的含义是：**“将来调用新函数时，请把第 N 个参数填到这里。”**

**数字 1, 2 代表的是“新函数”被调用时参数的顺序**，而不是原函数的参数顺序。
- **`_1`**：代表 **新函数** 被调用时传入的 **第 1 个参数**。
- **`_2`**：代表 **新函数** 被调用时传入的 **第 2 个参数**。
- 以此类推...

##### 一些用法

**参数重排**

`std::bind` 甚至可以改变参数的顺序，这在适配某些接口时非常有用。
```cpp
// 这里的 _2 填入原函数的第一个位置，_1 填入第二个位置
// 实际上实现了参数位置互换
auto reverse_func = std::bind(func, _2, _1, 88);

// 调用 reverse_func(10, 20)
// 映射关系：
// 原函数 a = _2 (即 20)
// 原函数 b = _1 (即 10)
// 原函数 c = 88 (固定)
reverse_func(10, 20); // 输出: a=20, b=10, c=88
```

**绑定成员函数**

绑定类的成员函数时，情况稍微复杂一点。因为成员函数默认有一个隐藏的 `this` 指针参数，所以 `bind` 的**第一个参数必须是成员函数的地址**，**第二个参数必须是对象的实例（或指针）**。
```C++
struct Hero {
    void attack(int damage) {
        std::cout << "Hero deals " << damage << " dmg!" << std::endl;
    }
};

Hero h;
// 1. &Hero::attack -> 成员函数指针
// 2. &h -> 对象的地址 (作为 this 指针传入)
// 3. _1 -> 这里的 _1 代表 attack 的参数 damage
auto hero_atk = std::bind(&Hero::attack, &h, _1);

hero_atk(999); // 输出: Hero deals 999 dmg!
```

##### ⚠️ 关键坑点：bind 默认是拷贝！

**这是 `std::bind` 最容易出错的地方。** 当你把参数“绑定”进去时，`bind` 内部会把这个参数**拷贝**一份保存起来。如果你希望绑定的是引用，必须使用 **`std::ref`**。

```C++
void update(int& n) { n++; }

int n = 10;
auto f1 = std::bind(update, n); // ❌ n 被拷贝进去了，外部 n 不会变
f1(); // 外部 n 还是 10

auto f2 = std::bind(update, std::ref(n)); // ✅ 存的是引用
f2(); // 外部 n 变成 11
```

| **特性**   | **std::bind**     | **Lambda 表达式**            |
| -------- | ----------------- | ------------------------- |
| **可读性**  | 较差 (占位符 `_1` 容易晕) | 极好 (代码就在眼前)               |
| **灵活性**  | 有限                | 极高 (甚至可以写复杂的逻辑)           |
| **效率**   | 可能有额外开销 (虚函数/指针)  | 编译器极易内联优化 (Zero overhead) |
| **引用处理** | 必须手动 `std::ref`   | 简单的 `[&]` 即可              |
```cpp
// 旧式 bind 写法
auto f_bind = std::bind(func, 100, _1, _2);

// 现代 Lambda 写法 (推荐)
auto f_lambda = [](int b, int c) { 
    func(100, b, c); 
};
```

> **在你的字典中可以标注：** 虽然现代 C++ 开发中推荐优先使用 **Lambda**，但 `std::bind` 依然大量存在于遗留代码库中。理解 `std::bind` 的参数占位逻辑和拷贝机制，对于阅读旧代码至关重要。

### std::move (移动语义)

**核心痛点**：默认的“傻瓜式拷贝” （左值右值及其引用 参照[C++生存指南] #左值右值及其引用 ）
**场景**：你有一个装着 100 万个点的 `std::vector` (点云数据)，你想把它传给另一个变量。

```cpp
std::vector<int> A = { ... 100万个点 ... };
std::vector<int> B = A; // 发生了什么？
```

- **默认行为 (Copy)**：
    1. 系统为 B 申请一块新的内存。
    2. 把 A 里的 100 万个数据，一个一个**复制**到 B 里。
    3. **结果**：你有了两份完全一样的数据。
- **痛点**：很慢！而且如果你以后再也不用 A 了（比如 A 只是个临时变量），那刚才的复制就是纯粹的浪费。

- **移动行为 (Move)**：
    
    1. `vector` 本质上包含三个指针（指向堆内存的头、尾、容量）。
    2. `std::move` 触发了 B 的**移动构造函数**。
    3. B 直接把 A 的那三个指针**偷**了过来。
    4. A 被置空（指针指向 `nullptr`）。
    5. **结果**：没有发生数据的搬运，只是交换了所有权（指针）。速度极快！

> **💡 生活比喻：**
> **拷贝 (Copy)**：我想看你的书。我拿去复印店，把整本书复印了一份，我有了一本新书，你手里还有旧书。
> **移动 (Move)**：我想看你的书，反正你也不看了。你直接把**书的持有权转让**给我。现在书归我了，你手里是空的。


### **智能指针重置 (`.reset()`)**

 - **什么时候会遇到？**
	当你定义了一个全局变量（如 `PlanningScenePtr`），但一开始没法初始化，只能在 `main` 函数里初始化时。
	或者当你想要销毁旧对象，重新加载一个新对象时
- **人话翻译**：
	- `ptr.reset(new Object())` = **“扔掉旧的，换个新的”**
    - 等同于普通指针的：`delete old_ptr; old_ptr = new Object();` 
-  **典型代码（ROS 常用）**：
```cpp
	// 全局定义空的
	robot_model::RobotModelPtr model;

	int main() {
	     // 只能在这里“填肉”
	    // ❌ model = new ... (报错)
	    // ✅ 用 reset
	    model.reset(new robot_model::RobotModel(loader.getModel()));
	}
```
 - **坑点提醒**：
    千万不要对普通指针（Raw Pointer）用 `reset`，这是智能指针专属技能。

___
## 💡 5. 算法技巧 (Algorithmic Idioms)

#### **`0x3f3f3f3f` (灵茶山艾府 无穷大)**

这是一个在算法竞赛（如 `ACM/ICPC`）中常用的技巧常量。

- **数值**：十进制约 $1.06 \times 10^9$。
- **两大优势**：
    1. **防止溢出**：两个 `INF` 相加 (`0x3f3f3f3f + 0x3f3f3f3f`) 约为 $2.12 \times 10^9$，略小于 `int` 最大值 ($2.14 \times 10^9$)。这在图论算法（如 Dijkstra、Floyd）松弛操作中非常关键。
    2. **memset 友好**：`0x3f` 的二进制是 `00111111`。由于 `memset` 按**字节**赋值，`memset(arr, 0x3f, sizeof(arr))` 刚好能把 `int` 数组的每个整数初始化为 `0x3f3f3f3f`。

---

## 🧵 6.并发与多线程 (Concurrency & Multithreading)

#### `std::thread`

C++11 引入了 `<thread>` 头文件，使得 C++ 在语言层面原生支持多线程编程，不再依赖平台特定的 API（如 `pthread` 或 Windows API）。`std::thread` 用于创建一个新的线程执行流。

**基本用法：** `std::thread` 接受一个**可调用对象**（函数、Lambda 表达式、函数对象）作为参数来启动线程。
**关键操作：**

1. **join()**:
    - **含义**: “汇合”。主线程等待子线程执行完毕。
    - **作用**: 确保子线程在主线程结束前完成，防止访问已销毁的资源。
    - **注意**: 如果不调用 join 也未调用 detach，线程对象析构时会调用 `std::terminate` 导致程序崩溃。
        
2. **detach()**:
    - **含义**: “分离”。将子线程与主线程分离，子线程在后台独立运行。
    - **作用**: 也就是常说的“守护线程”。
    - **注意**: 分离后，主线程失去了对子线程的控制权。必须确保子线程访问的资源在子线程运行期间一直有效（例如不能访问主线程栈上的局部变量）。
        
**注意事项：**
- `std::thread` 对象是**不可拷贝**的（`non-copyable`），但可以**移动**（movable）。这意味着你不能将一个 thread 对象赋值给另一个，但可以移交所有权。
- 需要注意线程间的资源竞争，通常需要配合 `std::mutex`（互斥锁）或 `std::atomic`（原子操作）使用。

举例：
⚠️ `std::thread` 的构造函数非常“傻”，它默认是**拷贝 (Copy)** 参数的。

```cpp
void modify(int& n) { n++; } // 接收引用

int main() {
    int x = 0;
    
    // ❌ 编译报错！或者静默拷贝失败
    // std::thread t(modify, x); 
    
    // ✅ 正确写法：使用 std::ref() 包装
    // 明确告诉线程："给我传引用，别拷贝！"
    std::thread t(modify, std::ref(x));
    
    t.join();
    // 此时 x 变成了 1
}
```

##### 🔒 **为什么默认是拷贝参数呢？**

**安全原因**：防止“空悬引用” (Dangling Reference)

线程是**异步**运行的。主线程创建子线程后，主线程可能瞬间就结束了，或者 `n` 超出作用域被销毁了。

- 如果 `std::thread` 默认允许传引用：
    1. 主线程启动子线程 `t`。
    2. 主线程函数结束，变量 `n` 被销毁（内存释放）。
    3. 子线程 `t` 还在跑，试图访问 `n`。
    4. **BOOM!** 子线程访问了非法内存（野指针/空悬引用），程序崩溃。
        
为了防止这种低级且致命的错误，C++ 标准委员会决定：**`std::thread` 默认将所有参数“拷贝”一份保存到子线程的独立空间里**。

**技术原因**：参数的“退化” (Decay)

`std::thread` 是一个变参模板。在 C++ 中，当你把参数传给这种模板时，参数类型会发生 **Decay（退化）**：

- 数组会退化成指针。
- **引用会退化成值（也就是发生了拷贝）**。
    
即使你的函数签名是 `void func(int& n)`，`std::thread` 在内部构造时，已经把你的 `n` 拷贝成了一个**临时副本**（Rvalue）。 然后，它试图把这个**临时副本**传递给你的函数 `func(int& n)`。

在 C++ 规则中：**非 `const` 的左值引用（`int&`）不能绑定到临时对象（右值）上**。
- 编译器觉得：你想修改这个变量，但你修改的只是线程内部拷贝的一个临时副本，这通常不是你想要的（你会以为修改了原来的 `n`），所以编译器直接报错，禁止你这么做。

所以我们需要借助于std::ref来处理
##### 🎣 在 ROS 中，你什么时候需要自己写 `std::thread`？

**场景 A：防止卡死回调函数** ROS 的回调函数（Callback）必须跑得快。如果你在回调里做复杂的路径规划（耗时 5 秒），整个节点就会卡死 5 秒，收不到其他消息。
- **解法**：回调收到消息 -> 丢给一个子线程去计算 -> 回调立刻结束。
    
**场景 B：多线程处理点云** 如果你收到一帧 30万个点的点云，单线程处理太慢。
- **解法**：开 4 个线程，每个线程处理 1/4 的数据（分块处理）。
    
**场景 C：监听非 ROS 设备** 你需要读取一个串口或者 TCP Socket 的数据，这通常是一个死循环 `while(true)`。
- **解法**：不能放在主线程里（会卡死 `ros::spin`），必须开一个独立的线程去 `while(true)`。

#### std::ref (引用包装器)

`std::ref` (reference) 和 `std::cref` (const reference) 是 C++11 引入的辅助函数，定义在 `<functional>` 头文件中。

它的核心作用是“**伪装**”：将一个引用包装成一个对象（`std::reference_wrapper`），使其能够通过那些“默认按值拷贝”的模板函数（如 `std::thread`, `std::bind`）传递，或者存入无法存储原生引用的容器（如 `std::vector`）中。

##### **为什么需要它？**

在 C++ 中，**引用 (Reference)** 不是对象，它只是变量的别名。因此它有两大限制：

1. **无法拷贝**（引用的“拷贝”是拷贝它指向的值）。
2. **无法重新赋值**（一旦绑定，终身不换）。

这导致很多泛型操作（如多线程传参、容器存储）无法直接处理引用，只能进行拷贝。`std::ref` 就是为了解决这个问题而生的。

 **`std::ref` 到底做了什么？**

既然 `std::thread` 强行要拷贝参数，`std::ref` 就耍了个花招：

1. `std::ref(n)` 创建了一个小对象（`reference_wrapper`）。
2. 这个小对象**内部存的是 `n` 的指针**。
3. 这个小对象是**可以拷贝**的（这就骗过了 `std::thread` 的拷贝机制，拷贝了一个存着指针的小对象）。
4. 当线程真正执行函数时，这个小对象会利用运算符重载，假装自己是引用，从而把原始的 `n` 传递给函数。

**☠️ 危险警告 (Lifetime Safety)** `std::ref` **不负责**延长变量的生命周期。 如果你传递了局部变量的引用，必须确保该变量的存活时间 **长于** 线程的执行时间（通常通过 `t.join()` 来保证）。 如果无法保证，请使用 **传值** 或 **`std::shared_ptr`**。
##### 三大典型用法

 **① 配合 `std::thread` 修改外部变量 (最常用)**

`std::thread` 为了线程安全，默认会将所有参数拷贝到子线程的独立栈空间。如果你希望子线程修改主线程的变量，必须用 `std::ref` 显式声明“我是传引用的”。

```cpp
#include <thread>
#include <functional> // 必须包含
#include <iostream>

void add_one(int& n) {
    n++;
}

int main() {
    int num = 0;

    // ❌ 错误：std::thread 尝试拷贝 num，但 add_one 需要引用，导致编译错误或无法修改原值
    // std::thread t1(add_one, num); 

    // ✅ 正确：使用 std::ref 包装，传递“引用的包装器”
    std::thread t2(add_one, std::ref(num)); 
    t2.join();

    std::cout << num << std::endl; // 输出 1
    return 0;
}
```

 **② 配合 `std::bind` (函数绑定)**

虽然现代 C++ 推荐使用 Lambda，但在使用 `std::bind` 时，`std::ref` 是必不可少的，因为 `bind` 默认也是将参数按值拷贝存储的。
```cpp
void print(int& n) { cout << n << endl; }

int n = 10;
// ❌ 这里的 n 被拷贝存储在 f1 内部
auto f1 = std::bind(print, n); 
n = 20;
f1(); // 输出 10 (打印的是拷贝时的快照)

// ✅ std::ref 让 f2 存储 n 的引用
auto f2 = std::bind(print, std::ref(n)); 
n = 30;
f2(); // 输出 30 (打印的是最新的 n)
```

 **③ 让容器存储“引用”**

标准容器（如 `vector`, `list`）要求元素必须是“可赋值”和“可拷贝”的对象。原生的引用 `int&` 不满足条件，因此 `vector<int&>` 是非法的。 使用 `std::reference_wrapper` (即 `std::ref` 的返回类型) 可以实现“在容器里存引用”的效果。
```cpp
int a = 1, b = 2, c = 3;

// ❌ 编译报错：无法创建引用的 vector
// std::vector<int&> v; 

// ✅ 正确：存储引用的包装器
std::vector<std::reference_wrapper<int>> v;
v.push_back(std::ref(a));
v.push_back(std::ref(b));

// 修改 a 的值
a = 100;

// v[0] 也会随之改变，因为它本质上指向 a
std::cout << v[0] << std::endl; // 输出 100
```

##### 兄弟函数：`std::cref`

如果你只想传递引用以避免拷贝，但**不希望被修改**（即 `const T&`），应该使用 `std::cref` (Const Reference)。
```cpp
void print_large_data(const BigObject& data);

BigObject obj;
// 避免拷贝 obj，同时保证 print_large_data 无法修改 obj
std::thread t(print_large_data, std::cref(obj));
```

| **关键字**       | **全称**                  | **作用**       | **适用场景**           |
| ------------- | ----------------------- | ------------ | ------------------ |
| **std::ref**  | Reference Wrapper       | 包装非 const 引用 | 线程传参修改值、bind、容器存引用 |
| **std::cref** | Const Reference Wrapper | 包装 const 引用  | 线程传参只读（避免大对象拷贝）    |

---

## 💻 7. ROS相关函数

#### subscribe 节点的耳朵

ROS **发布/订阅 (Pub/Sub)** 模型中负责“听”的那一端。如果没有它，你的机器人就是聋子，感知不到世界的任何变化。

##### 核心模型：收音机原理

- **Publisher (电台)**：只管向空气中发送信号，不知道谁在听。
- **Topic (频道)**：比如 "FM 98.7" 或者 `/camera/image_raw`。
- **Subscriber (收音机)**：调到指定频道，一旦收到信号，就播放出来（执行回调函数）。

`subscribe` 函数通常由 `ros::NodeHandle` 对象调用。它长得像这样：

```cpp
ros::Subscriber sub = nh.subscribe<MsgType>(topic_name, queue_size, callback_function, transport_hints);
```

- **`MsgType` (模板参数)**：
    - **含义**：你要听什么语言？是 `std_msgs::String` 还是 `sensor_msgs::Image`？
    - **作用**：ROS 会自动帮你把二进制流反序列化成这个类型的对象。
    - **注意**：如果你写错了类型（比如发的是 Image 你非要收 `LaserScan`），ROS 会报错（`MD5 Sum` 不匹配）。

- **`topic_name` (话题名)**：
    - **含义**：你要听哪个频道？
    - **技巧**：可以是绝对路径 `/cmd_vel`，也可以是相对路径 `cmd_vel`（会自动加上节点的命名空间）。

- **`queue_size` (队列大小)**：
    - **含义**：**缓冲区**。如果你的处理速度（回调函数）比发送速度慢，能暂存多少条消息？
    - **举例**：设为 `10`。如果一瞬间来了 100 条消息，你的回调函数还在处理第 1 条，那么最新的 10 条会存进队列，**中间的 89 条会被无情丢弃**。
    - **生存法则**：对于最新的传感器数据（如图像），通常设为 `1`（我只要最新的，旧的没用）；对于控制指令，可以设大一点。
        
- **`callback_function` (回调函数)**：
    - **含义**：收到信后，你想怎么处理？
    - **本质**：这就是我们在前几节反复讨论的 `std::function` 或函数指针。


##### 应用场景

**Level 1: 普通函数 (新手村)** 最简单，但没法访问类的成员变量，实际项目中几乎不用。
```cpp
void callback(const std_msgs::String::ConstPtr& msg) {
    ROS_INFO("I heard: %s", msg->data.c_str());
}

int main() {
    // ...
    // 直接传函数名
    ros::Subscriber sub = nh.subscribe("chatter", 1000, callback); 
}
```
**Level 2: 类成员函数 (工业标准)** **这是 ROS 开发中最常用的写法！** 结合了 `this` 指针。
```cpp
class Listener {
public:
    void callback(const std_msgs::String::ConstPtr& msg) {
        // 可以访问类的成员 count_
        count_++;
        ROS_INFO("Heard: %s", msg->data.c_str());
    }
    
    void init(ros::NodeHandle& nh) {
        // 关键语法：&类名::函数名, this
        // 意思：当有消息时，请在 "我(this)" 这个对象上调用 callback 函数
        sub_ = nh.subscribe("chatter", 1000, &Listener::callback, this);
    }

private:
    ros::Subscriber sub_;
    int count_ = 0;
};
```
**Level 3: Lambda 表达式 (现代 C++ 风)** 适合写逻辑简单的回调，不需要专门在头文件声明一个函数。
```cpp
ros::Subscriber sub = nh.subscribe<std_msgs::String>(
    "chatter", 
    10, 
    [this](const std_msgs::String::ConstPtr& msg) {
        ROS_INFO("Lambda heard: %s", msg->data.c_str());
    }
);
```
**Level 4: 带参数的回调 (std::bind 高级玩法)** 如果你的回调函数需要额外的参数（比如标记这是第几个相机的回调）。
```cpp
void callback(const MsgPtr& msg, int camera_id) { ... }

// 绑定额外的参数 1
auto f = boost::bind(callback, _1, 1); 
sub = nh.subscribe<Msg>("cam1", 10, f);
```

##### 注意点

请注意这行代码： `ros::Subscriber sub = nh.subscribe(...)`

这个 `sub` 变量非常重要，它是 **RAII** 的典型应用。
- **生**：`nh.subscribe` 创建了连接，返回这个句柄 `sub`。
- **活**：只要 `sub` 还在作用域内（还活着），订阅就一直有效。
- **死**：当 `sub` 超出作用域（被析构）时，ROS 会自动**取消订阅 (Unsubscribe)**，断开网络连接。
    
**💀 常见坑点**： 如果你在函数里这样写：
```cpp
void setup() {
    // 错误！sub 是局部变量
    ros::Subscriber sub = nh.subscribe(...); 
} // 函数结束，sub 被销毁，订阅断开！你永远收不到消息。
```
**✅ _修正**：把 `sub` 变成**类成员变量**，让它陪着节点对象一起活下去。_

**消息类型的 const 指针 (ConstPtr)**

你会发现回调函数的参数通常长这样： `const std_msgs::String::ConstPtr& msg`

为什么要写这么复杂？

1. **`ConstPtr` (智能指针)**：ROS 为了效率，在节点内传递消息时不会拷贝数据，而是传递 `boost::shared_ptr`。
2. **`const` (只读)**：ROS 是一对多的。一条消息可能发给 10 个回调函数。为了防止回调函数 A 偷偷改了数据，害得回调函数 B 读到脏数据，ROS 强制要求**只读**。
