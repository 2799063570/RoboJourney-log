# C++ STL set 容器

## 一、set容器的基本概念

set(有序集合)，是一种关联式容器，它包含了有序的唯一元素。

1. set容器中的元素是**唯一**的，即每个元素的值都不同。
2. set容器中的元素是**有序**的，默认按照元素值的升序排序。

与之相对应的是multiset容器，它允许存储重复的元素。

与之前所学的线性容器(vector, list, deque)不同，set是树形容器，底层采用的是树状结构（二叉搜索树），因此中序遍历的结果就是一个有序的序列。

## 二、set容器的创建

set容器的**创建**可以通过多种方式进行，主要包括以下几种：

- 默认构造函数：创建一个空的set容器。
- 拷贝构造函数：通过已有的set容器创建一个新的set容器。
- 范围构造函数：通过指定的范围（如数组、迭代器等）创建一个set容器。
- 初始化列表构造函数：通过初始化列表创建一个set容器。

```cpp
set<int> s1;  // 默认构造函数
set<int> s2(s1);  // 拷贝构造函数
set<int> s3({1, 7, 3, 4, 5});  // 初始化列表构造函数 自动排序
set<int> s4(s3.begin(), s3.end());  // 范围构造函数
```

set容器的**赋值**可以通过多种方式进行，主要借助于赋值运算符重载进行赋值操作。

```cpp
set<int> s2 = {23, 12, 21, 11, 11 };
set<int> s1;
s1 = s2;  // 赋值运算符重载
s1 = {1, 2, 3, 4, 5};  // 初始化列表赋值
```

## 三、set容器常用操作

set容器的常用操作包括以下几种：

- 插入元素：使用insert()函数插入指定元素或迭代器范围，时间复杂度为O(logn)。
- 删除元素：使用erase()函数删除指定的元素(元素值或迭代器)，或者使用范围删除多个元素。
- 查找元素：使用find()函数查找指定的元素(返回迭代器)，或使用count()函数统计指定元素的个数。
- 遍历元素：使用迭代器遍历set容器中的元素。
- 清空容器：使用clear()函数清空set容器中的所有元素。
- 检查容器是否为空：使用empty()函数检查set容器是否为空。
- 获取容器大小：使用size()函数获取set容器中元素的个数。

```cpp
// 容器大小
set<int> s = {5, 2, 8, 1, 4};
cout << "s 容器的大小为：" << s.size() << endl;
cout << "s 容器是否为空：" << s.empty() << endl;

// 插入元素
s.insert(6);    // 自动排序
cout << "s 容器的大小为：" << s.size() << endl;
vector<int> v = {32, 12, 21, 11, 11 };
s.insert(v.begin(), v.end());   
cout << "s 容器的大小为：" << s.size() << endl;

// 查找元素
set<int>::iterator it = s.find(21);
if (it != s.end()) {
    cout << "找到元素：" << *it << endl;
} else {
    cout << "未找到元素" << endl;
}
cout << "s 容器中 21 元素的个数为：" << s.count(21) << endl;

// 删除元素
s.erase(21);
set<int>::iterator it2 = s.find(11);
if (it2 != s.end()) {
    cout << "找到元素：" << *it2 << endl;
    s.erase(it2);
} else {
    cout << "未找到元素" << endl;
}
s.erase(it2, s.end()); 

// 统计元素个数 只能是0或1
cout << "s 容器中 11 元素的个数为：" << s.count(11) << endl;
```

## 四、set容器的排序规则

当引入自定义类型时，需要为set容器指定排序规则。

```cpp
class Person {
private:
    string name;
    int age;
public:
    Person(string n, int a) : name(n), age(a) {}
    Person() : name("person111"), age(0) {}
};

set<Person> s;
s.insert(Person("张三", 20));
s.insert(Person("李四", 18));
s.insert(Person("王五", 25));
```

上述代码会报错，因为编译器不知道如何比较Person对象的大小。为了解决这个问题，我们可以通过重载运算符<的方式为set容器指定排序规则：在Person类中重载小于运算符<，根据姓名或年龄进行比较。

```cpp
bool operator<(const Person& p) const {
    return this->age < p.age;  // 按年龄升序排序
}
```
