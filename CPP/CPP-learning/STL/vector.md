## C++ STL vector容器

原文已经发布至CSDN[https://blog.csdn.net/searcon/article/details/151836609]

## vector容器基本概念

vector是标准模板库（STL）提供的一种动态数组容器，能够存储相同类型的元素。类似于数组，vector是一种内存中连续存储的结构。但同时又区别于普通数组，vector 的大小可以动态调整，支持在运行时高效地插入或删除元素。

通俗的说vector就是数组的进阶版，可以像数组那样通过索引，进行直接访问元素，实现时间复杂度为 o(1)。功能上不仅支持容量大小的动态调整，还提供了多种多样的方法，方便对vector进行操作。

## vector容器的创建和赋值

### vector容器的创建

**创建方法**主要可以分为：

- 通过默认构造函数进行创建，即无任何参数输入
- 通过(初始化)列表进行显式、隐式创建
- 通过迭代器进行创建
- 通过指定初始元素个数进行创建
- 通过拷贝构造进行创建

**默认构造函数**：创建一个空的vector对象，不包含任何元素。适用于需要从零动态添加元素的情况。

```cpp
std::vector<int> v; // 默认构造函数，v为空vector
```

通过**初始化列表**进行显式和隐式创建
**显式**创建：使用花括号{ }直接初始化，直接在声明时指定元素列表。
**隐式**创建：使用赋值运算符=结合花括号列表初始化，语法更简洁，但效果相同。 这两种方式都利用初始化列表（C++11特性），创建一个包含指定元素的vector。

```cpp
// 显式创建：直接使用花括号
std::vector<int> v_explicit{1, 2, 3}; // 包含元素1,2,3   
// 隐式创建：通过赋值运算符
std::vector<int> v_implicit = {4, 5, 6}; // 包含元素4,5,6
```

通过指**定初始元素个数**进行创建。指定vector的初始大小（元素个数），所有元素使用默认值初始化（如int类型默认为0）。也可以指定初始值。

```cpp
// 指定大小：创建大小为5的vector，元素默认为0
std::vector<int> v_size(5); // {0, 0, 0, 0, 0}   
 
// 指定大小和初始值：创建大小为3的vector，所有元素初始化为100
std::vector<int> v_size_value(3, 100); //{100, 100, 100}
```

**拷贝构造**进行创建, 使用拷贝构造函数基于另一个vector创建新对象，复制所有元素。适用于需要独立副本的场景。

```cpp
std::vector<int> original = {1, 2, 3};
// 拷贝构造：创建original的副本
std::vector<int> copy_vector(original); // 1,2,3
```

在实际编程中，需要根据场景的需要灵活选择构造方法。例如，优先使用初始化列表以简化代码，或使用迭代器以避免不必要的拷贝。

### vector容器的赋值

在C++中，vector的赋值操作用于修改已有vector对象的内容，而不是创建新对象（拷贝构造则用于创建新对象时初始化）。赋值操作主要分为两种方式：使用赋值运算符（=）和assign成员函数。赋值操作与拷贝构造不同：拷贝构造发生在对象创建时，如`vector<int> target = source`, 而赋值操作发生在对象已存在时。

通过**赋值运算符进行赋值** 使用赋值运算符=将另一个vector的内容(初始化列表)复制到新的vector中。

```cpp
std::vector<int> source = {7, 8, 9};
// 赋值运算符：创建新vector或覆盖现有vector
std::vector<int> target;
target = source; // target现在包含元素7,8,9
target = {1, 2, 3}; // 初始化列表
```

通过**assign方法赋值**，assign成员函数用于重置现有vector的内容，而不是创建新对象。它可以指定元素个数和值，使用迭代器范围来填充内容或指定元素列表。此操作会销毁原有元素并重新分配内存。

```cpp
std::vector<int> v;
// 使用assign指定元素个数和值
v.assign(4, 50);         // v现在包含四个50: {50, 50, 50, 50}
    
// 使用assign指定迭代器范围
int arr[] = {60, 70, 80};
v.assign(arr, arr + 3);  // v现在包含元素60,70,80
 
// 使用assign指定列表
v.assign({12, 23, 12});  // v现在包含元素12,23,12
```

## vector容器的插入和删除

- 插入方法: insert、push_back
- 删除方法：erase\clear、pop_back

**插入操作**(**insert、push_back**)允许在vector的任意位置添加新元素。常见方法包括：

- push_back方法：在vector末尾添加一个元素。这是一个高效的操作，平均时间复杂度为o(1)（摊销时间）。由于vector使用动态扩容策略，当触发扩容时时间复杂度为o(n)。
- insert方法：在指定位置插入一个或多个元素。例如，在索引处插入元素，需要移动后续元素，平均时间复杂度为o(n)，其中n是vector的大小。需要注意的是insert函数中，第一个参数并不是int类型的索引，而是迭代器。

插入操作可能导致内存重新分配，从而增加开销。在C++中，使用reserve方法可以预分配内存以减少扩容次数。(在vector空间预留部分，我们会解释为什么会增加开销)

```cpp
std::vector<int> vec = {10, 20, 30};  // 容器内容：10,20,30
 
vec.push_back(40);         // 末尾插入40 容器内容：10,20,30,40
// 在索引1处插入15 
vec.insert(vec.begin() + 1, 15); // 容器内容：10,15,20,30,40
```

**删除操作**(**pop_back、erase**)移除vector中的元素，常见方法包括：

- pop_back方法：删除vector末尾的元素。时间复杂度为o(1)，因为它只需减小大小，容量大小不变。
- erase方法：删除指定位置或范围的元素。删除索引处的元素，需要移动后续元素，平均时间复杂度为o(n)。
- clear方法：删除容器中的所有元素。
        
删除操作不会自动释放所有内存，vector保留容量以备后续使用。如果需要彻底释放内存，可以使用shrink_to_fit方法，或参照vector内存交换部分，使用swap函数实现内存释放。

```cpp
std::vector<int> vec = {10, 20, 30};
std::cout << vec.size() << " " << vec.capacity() << std::endl; // 3 3
 
vec.pop_back();         // 删除末尾元素 10 20
std::cout << vec.size() << " " << vec.capacity() << std::endl; // 2 3
 
vec.erase(vec.begin()); // 删除索引0处的元素  20
std::cout << vec.size() << " " << vec.capacity() << std::endl; // 1 3
 
vec.clear();            // 删除所有元素
std::cout << vec.size() << " " << vec.capacity() << std::endl; // 0 3
 
vec.shrink_to_fit();    // 释放冗余内存
std::cout << vec.size() << " " << vec.capacity() << std::endl; // 0 0
```

## vector容器扩充机制
       
“扩容”（resizing）指的是当某个容器（如数组、列表、哈希表等）的存储空间不足以容纳新数据时，动态增加其容量的过程。 vector同顺序表一样都存在两个变量size和capacity，size表示当前元素的数量，可以通过size()函数获取；而capacity表示当前分配的内存可容纳的元素上限，可以通过capacity()函数获取。当size == capacity时，再新增元素，当前分配的内存不足以再存储新的元素，就会触发扩容的操作。
过程我们可以参照下图。 扩容过程可以描述为：当size==capacity时，再想添加新元素到容器，发现容量已满，强行插入会造成下标越界，这时需要找一块更大内存的空间。**先计算新的内存块的大小，分配新的内存块，将旧的内存块上进行遍历将元素逐个复制到新的内存块上，销毁旧的内存空间**。可以发现，**每次扩容都会有新内存的创建、对vector容器进行遍历复制、旧内存的释放，且容器元素越多所花费时间就越多，时间复杂度为o(n)，是一个非常费时的操作**。
![](https://i-blog.csdnimg.cn/direct/5c352af252124027bb3b408a983d7227.jpeg)
我们具体来分析一下扩容的过程：

```cpp
std::vector<int> vec;  // 初始capacity=0
cout << vec.size() << " " << vec.capacity() << endl; // 0 0   
 
vec.push_back(1);      // 首次分配
cout << vec.size() << " " << vec.capacity() << endl; // 1 1
 
vec.push_back(2);      // capacity=2
cout << vec.size() << " " << vec.capacity() << endl;  // 2 2
 
vec.push_back(3);vec.push_back(3);vec.push_back(3);vec.push_back(3);      
cout << vec.size() << " " << vec.capacity() << endl; // 6 6
 
vec.push_back(3);
cout << vec.size() << " " << vec.capacity() << endl; // 7 9
```

1.先**创建一个空的vector**，这时vector的**size和capacity的大小都为0**；
2.通过push_back函数，在**vector中添加新元素**，这时**size和capacity都扩容至1**；
3.再通过push_back函数，在**vector中添加新元素**，这时**size和capacity都扩容至2**；
4.通过push_back函数，反复添加，**添加4个元素**，这时**size和capacity都扩容至6**；
5.最后通过push_back函数，**添加1个元素**，可以发现**size和capacity分布变为7 9**

这个过程中存在多次扩容，1到2：capacity从0到1，2到3：capacity从1到2，4到5：capacity从6到9。**我们可以通过分析push_back的函数源码实现来了解扩容的机制，通过进入bush_back的源码实现。**
![push_back源码实现](https://i-blog.csdnimg.cn/direct/c42b6b1833f44632a63b944ba25d6d85.jpeg)
我们可以发现当`_Mylast != _My_data._Myend`时候，即(size < capacity)的时候，会调用`_Emplace_back_with_unused_capacity` 函数进行添加元素。反之则（size == capacity）需要进行扩容操作，则调用`_Emplace_reallocate` 函数。
![_Emplace_reallocate函数](https://i-blog.csdnimg.cn/direct/bf0da9ca62b94f23a720679c8e178518.jpeg)

```cpp
const size_type _Newsize = _Oldsize + 1;
size_type _Newcapacity   = _Calculate_growth(_Newsize);
```

我们重点关注这个函数的以上两行，**计算新的capacity的大小存储到_Newcapacity变量中**，传入参数为`_Oldsize + 1`，即**原容量+1**。进入`_Calculate_growth`函数，`_Oldsize + 1`（原容量+1）作为形参`_Newsize`。
![](https://i-blog.csdnimg.cn/direct/57de8c95236147e1bfb1bde13ae00c64.jpeg)
可以看到 `const size_type _Geometric = _Oldcapacity + _Oldcapacity / 2;` 采用1.5倍的乘法因子（扩容因子），当然由于是整除会向下取整。

同时可以关注到`if (_Geometric < _Newsize) `判断条件，**1.5倍得到的容量会和_Newsize进行比较，也就是会和_Oldsize + 1（原容量+1）进行比较**，取较大的值作为新的容量的大小。
因此在1到2的过程（capacity从0到1）中：**_Oldsize + 1 = 1** ，通过扩容因子得到的新容量为**0（0  + 0/2）**,因此返回**1（_Oldsize + 1）**。
2到3的过程（capacity从1到2）中：**_Oldsize + 1 = 2** ，通过扩容因子得到的新容量为**1（1  + 1/2）**,因此返回**2（_Oldsize + 1）**。
4到5的过程（capacity从6到9）中：**_Oldsize + 1 = 7** ，通过扩容因子得到的新容量为**9（6  + 6/2）**,因此返回9（扩容因子计算）。

## vector随机访问

随机访问是指能够以常数时间直接获取容器中任意位置的元素（随机访问的时间复杂度是常数时间，即O(1)。）。在vector中，每个元素都有一个唯一的索引（从0开始），用户可以通过索引立即访问该元素，无需像链表那样从头开始顺序查找。在包含n个元素的vector中，访问第i个元素的操作是即时的。这表示无论vector的大小如何，访问任何元素所需的时间基本相同。相比之下，顺序访问结构（如链表）的时间复杂度为O(n)，因为需要遍历元素。
vector支持随机访问的核心原因是其内存布局：元素在内存中是连续存储的。这意味着所有元素被分配在相邻的内存地址上，因此可以通过简单的数学计算直接定位元素地址。具体来说：

随机访问主要有两种方式：**通过[ ]访问（下标访问/索引访问）**，通过运算符重载实现；**通过at()函数实现（安全索引访问）**。用法也很简单

```cpp
std::vector<int> vec = {10, 20, 30};
    
std::cout << "[]访问: vec[1] = " << vec[1] << std::endl; // 输出: 20 
std::cout << "at访问: vec.at(1) = " << vec.at(1) << std::endl; // 输出: 20
```

**两者的区别主要在于是否存在异常检测**，当索引超出容器的大小会导致索引的越界，若没有异常检测，程序可能访问无效内存地址，导致崩溃。同样，我们看两个函数实现的源码。

![运算符重载](https://i-blog.csdnimg.cn/direct/857f605b4ca142db8851423f494485d8.jpeg)
![at函数实现](https://i-blog.csdnimg.cn/direct/445cf122a3ef450c8fd6e8b22d782728.jpeg)
可以发现只有`_CONTAINER_DEBUG_LEVEL > 0` 条件满足的时候，[]重载访问函数才会有异常检测，而at函数只要调用就会有异常检测。可知[]访问追求速度，但风险更高；at访问强调安全性，代价是轻微性能损失。因此在能确保索引不越界的情况下尽可能的使用更高效率的[]访问。

```cpp
std::vector<int> vec = {10, 20, 30};
    
// []访问示例：高效但无检查
std::cout << "[]访问: vec[1] = " << vec[1] << std::endl; // 输出: 20 (索引1有效)
// 越界访问可能崩溃: vec[3] 未定义
// at访问示例：安全有检查
try {
    std::cout << "at访问: vec.at(1) = " << vec.at(1) << std::endl; // 输出: 20
    std::cout << "at访问越界测试: vec.at(3) = ..." << std::endl;
    vec.at(3); // 索引3无效，抛出异常
} catch (const std::out_of_range& e) {
    std::cout << "错误: " << e.what() << std::endl; // 捕获并处理异常
}    
```

## vector内存交换

内存交换（memory swapping）是vector的一个重要操作，用于高效地交换两个vector的内部存储，从而避免不必要的元素复制，优化性能并减少内存开销。主要借助于swap()成员函数实现vector的swap成员函数用于交换两个vector的全部内部数据，包括：

- 存储元素的内存缓冲区地址（指针）；
- size和capacity的值。

交换操作是常数时间 O(1) 的，因为它仅交换内部指针和属性，不涉及元素的复制或移动（除非元素类型本身的交换有额外开销）。

常见应用场景：

- 快速交换内容：在算法中（如排序或数据迁移），交换两个vector的内容而不复制元素。
- 优化空间：当vector容量过大时，通过交换缩小内存占用。使用交换技巧可以立即释放未使用的内存（避免内存泄漏）。

### 快速交换内容

swap()函数最基本的功能，vector的内存交换（通过swap成员函数）是一个仅交换内部管理信息的轻量操作，其核心是通过交换两个vector的 "元数据"（而非元素本身）来实现内存转移。下面从vector的内部结构出发，详细拆解内存交换的完整过程。vector类似于顺序表，内部有一块连续内存块的地址和对应的size、capacity。

```cpp
// 逻辑上的vector内部结构（简化版）
template <typename T>
struct vector {
    T* data;       // 指向存储元素的内存缓冲区（堆上）
    size_t size;   // 当前元素数量
    size_t capacity;// 缓冲区容量（可容纳的最大元素数）
};
```

vector的swap成员函数的核心功能是：交换两个vector对象的data指针、size和capacity这三个核心成员。

```cpp
std::vector<int> v1 = { 1, 2, 3, 4, 5 };
std::vector<int> v2 = { 2, 5, 2 };
 
v1.swap(v2);
 
std::cout << v1.size() << " " << v1.capacity() << std::endl; // 3 3
std::cout << v2.size() << " " << v2.capacity() << std::endl; // 5 5
```

### 释放内存

当vector的capacity远大于size时（冗余内存），可通过与一个临时vector交换，让临时对象带走冗余内存并在销毁时释放。原理如下：

1.用当前vector的元素（size个）初始化一个临时vector（临时对象的capacity会等于size，因为它是新创建的，仅分配刚好够用的内存）。
2.用swap交换当前vector和临时vector的内容 —— 此时当前vector会拥有临时对象的size和capacity（即capacity= size），而临时对象会拥有原来的冗余内存。
3.临时对象在作用域结束时被销毁，自动释放其持有的冗余内存。

```cpp
std::vector<int> v;
    
// 插入1000个元素，触发扩容，capacity会大于等于1000
for (int i = 0; i < 1000; ++i) {
    v.push_back(i);
}
std::cout << v.size() << ", " << v.capacity() << std::endl; // 1000 1066
 
    
// 删除900个元素，size减小，但capacity不变
v.erase(v.begin(), v.begin() + 900);
std::cout << v.size() << ", " << v.capacity() << std::endl; // 100 1066
 
    
// 用swap释放冗余内存
std::vector<int>(v).swap(v);  // 匿名对象 关键操作
std::cout << v.size() << ", " << v.capacity() << std::endl; // 100 100
```

我们分析上述代码的运行过程，创建一个容器，不断向容器中添加新元素，容器的size和capacity也不短的扩充。**通过erase删除900个元素，size减小至100个元素，capacity保持不变**，因capacity远大于size，**（size=100，capacity=1000）有大量内存冗余**。

想要释放冗余内存可以借助内存交换，**通过创建一个临时vector（借助拷贝构造，将原容器中的数据拷贝到临时vector中**，同时新创建的vector满足capacity=size）。借助swap()函数，将临时vector与原容器进行内存交换，这时原容器中的数据保证不变，capacity缩小至size大小，同时临时对象也会随着该语句的结束，占用的内存会被释放。

## vector空间预留

空间预留（Space Reservation）是一种优化技术，通过预先分配内存来避免频繁的内存重新分配（扩容），从而提高程序性能。在vector扩充机制部分，我们介绍了当size=capacity时，再添加元素会触发扩容。而每次扩容都会有大量的时间消耗，如果对一个容量小的容器持续的添加元素会频繁扩容，有大量时间消耗。比如如果对一个空vector添加100个元素，会频繁的触发扩容。例如capacity0到1，1到2，2到3，3到4，4到6.。。。
若分配足够大的capacity的话，就不会触发size=capacity，也就不会频繁的扩容。即预留足够capacity大小后，添加元素不会触发重新分配，直到元素数量超过预留容量。空间预留主要借助于reserve()函数实现。如果预留的容量小于当前容量，reserve()可能无效；如果大于当前容量，则分配新内存。预留不会初始化元素：内存是“未使用”的，实际元素需通过bush_back或emplace_back添加。

```cpp
std::vector<int> v;
for (int i = 0; i<100; ++i)
{
    std::cout << v.capacity() << " " << v.size() << std::endl;
    v.push_back(i);
}
std::vector<int> v2;
v2.reserve(100);
for (int i = 0; i<100; ++i)
{
    std::cout << v2.capacity() << " " << v2.size() << std::endl;
    v2.push_back(i);
}
```

通过reserve()方法预先分配内存，显著提升插入操作的效率（避免过多扩容触发）。它适用于需要高性能和可预测内存使用的场景。建议在代码中根据需求合理使用，以平衡内存开销和性能增益。

## vector高效删除

在vector数据的插入和删除部分，我们介绍了erase函数，该函数可以实现指定索引进行删除，但是该删除方法时间复杂度为o(n)，需要删除索引后的元素不断的覆盖前一元素。如果不考虑容器中元素的顺序，那么我们可以采用一种更为高效的删除方法：删除索引元素与尾部元素进行交换，使用pop_back()函数，删除交换后的索引元素。该方法的时间复杂度为o(1)，但是会对vector中数据的顺序产生影响。

```cpp
void delVector(std::vector<int>& v, int index)
{
    std::swap(v[index], v.back());
    v.pop_back();
}
void printVector(std::vector<int>& v)
{
    for (int i = 0; i < v.size(); ++i)
        std::cout << v[i] << " ";
    std::cout << std::endl;
}
 
std::vector<int> v1 = { 1, 2, 3, 4, 5 };
printVector(v1);        // 1 2 3 4 5
 
delVector(v1, 1);
printVector(v1);        // 1 5 3 4
```

## vecotr数据排序        

vector中的数据进行排序主要借助algorithm头文件中的sort函数。sort函数是一个高效、通用的排序函数，能够对vector这类容器中的元素进行快速排序，且支持自定义排序方向，平均时间复杂度为 o (n log n)。
参数1、参数2为容器的其实迭代器，即排序的范围。参数3为可选参数，自定义比较函数，用于定义元素排序的顺序方向。具体实现如下，利用整数列表初始化一个容器，同时定义一个比较函数，该函数返回值为布尔类型，调用sort函数对该容器进行排序。

```cpp
void printVector(const std::vector<int>& v)
{
	for (const int& n : v)
		std::cout << n << " ";
	std::cout << std::endl;
}
bool cmp(int a, int b)
{
	return a < b;
}
 
std::vector<int> v = {12, 232, 34, 1, 75};
 
printVector(v);
sort(v.begin(), v.end(), cmp);
printVector(v);
```

我们分析一下sort函数的源码实现，具体每个类型不详细介绍。

```cpp
template <class _RanIt, class _Pr>
_CONSTEXPR20 void _Sort_unchecked(_RanIt _First, _RanIt _Last, _Iter_diff_t<_RanIt> _Ideal, _Pr _Pred) {
    // order [_First, _Last)
    for (;;) {
        if (_Last - _First <= _ISORT_MAX) { // small
            _STD _Insertion_sort_unchecked(_First, _Last, _Pred);
            return;
        }
 
        if (_Ideal <= 0) { // heap sort if too many divisions
            _STD _Make_heap_unchecked(_First, _Last, _Pred);
            _STD _Sort_heap_unchecked(_First, _Last, _Pred);
            return;
        }
 
        // divide and conquer by quicksort
        auto _Mid = _STD _Partition_by_median_guess_unchecked(_First, _Last, _Pred);
 
        _Ideal = (_Ideal >> 1) + (_Ideal >> 2); // allow 1.5 log2(N) divisions
 
        if (_Mid.first - _First < _Last - _Mid.second) { // loop on second half
            _STD _Sort_unchecked(_First, _Mid.first, _Ideal, _Pred);
            _First = _Mid.second;
        } else { // loop on first half
            _STD _Sort_unchecked(_Mid.second, _Last, _Ideal, _Pred);
            _Last = _Mid.first;
        }
    }
}
```

`_First、_Last`：排序范围的起始和结束迭代器（左闭右开区间`[_First, _Last)`）

`_Pred`：具体的比较函数（如之前写到的 `bool cmp(int a, int b)`函数，控制排序方向）。

对`[_First, _Last)`范围内的元素进行排序，内部根据数据规模自动选择最优排序算法。

1、小规模数据：插入排序

```cpp
if (_Last - _First <= _ISORT_MAX) { // small
    _STD _Insertion_sort_unchecked(_First, _Last, _Pred);
    return;
}
```

当待排序元素数量`（_Last - _First）`小于等于_ISORT_MAX（通常是一个小常数如 32）时，使用插入排序。插入排序的时间复杂度是O(n²)，但在小规模数据（n较小时）上，其常数因子极小（内存访问局部性好，几乎无额外开销），实际性能优于快速排序（快速排序有递归和分割的额外开销）。

