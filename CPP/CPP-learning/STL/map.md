# C++ STL map 容器

## 一、map的基本概念

在学系map之前我们先了解一下pair，pair是类模版，在[[C++ 关键字字典]]中已经介绍过
map容器也是同set容器一样，都是关联式容器，底层实现是二叉树(根据key值进行排序)。

1. 所有的元素都是pair，first不得重复(键)
2. pair的第一个数据是key(键), 第二个元素是value(值)
3. 所有元素都会根据元素值进行排序

## 二、map的创建

map容器的创建可以通过多种方式进行，主要包括以下几种：

- 默认构造函数：创建一个空的map容器。
- 拷贝构造函数：通过已有的map容器创建一个新的map容器。
- 范围构造函数：通过指定的迭代器范围创建一个新的map容器。
- 初始化列表构造函数：通过初始化列表创建一个新的map容器。

```cpp
map<int, string> map1; // 默认构造函数
map<int, string> map2(map1); // 拷贝构造函数
map<int, string> map3{{1, "one"}, {2, "two"}, {3, "three"}}; // 初始化列表构造函数
map<int, string> map4(map3.begin(), map3.end()); // 范围构造函数
```

map容器的赋值可以通过多种方式进行，主要借助于赋值运算符重载进行赋值操作。

```cpp
map<int, int> map1{pair<int, int>(1, 10), pair<int, int>(2, 20), pair<int, int>(3, 30)};
map<int, int> map2;
map2 = map1; // 拷贝赋值
map<int, int> map3;
map3 = {{4, 40}, {5, 50}}; // 初始化列表赋值
```

## 三、map的大小操作与数据插入修改

map的**大小操作**主要包括以下几种：

- size()：返回map容器中键值对的个数。
- empty()：判断map容器是否为空。
- max_size()：返回map容器所能容纳的最大元素个数。

```cpp
map<int, int> map1{pair<int, int>(1, 10), pair<int, int>(2, 20), pair<int, int>(3, 30)};
cout << map1.size() << endl; // 3
cout << map1.empty() << endl; // 0
cout << map1.max_size() << endl; // 536870911
```

map的**数据插入**主要包括以下几种：

- insert()：插入一个新的键值对(新的键值)。
- emplace()：直接在容器中构造一个新的元素。
- operator[]：通过键值访问元素，如果键不存在则插入一个默认值。需要注意的是multimap并没有重载

```cpp
map<int, string> map1;
map1.insert(pair<int, string>(1, "one")); // 插入一个新的键值对
map1.emplace(2, "two"); // 同于insert
map1[3] = "three"; // 通过键值访问元素，如果键不存在则插入一个默认值
map1[5];    // 通过键值访问元素，如果键不存在则插入一个默认值(空字符串)

// 返回值为pair类型(迭代器, 是否插入成功)
pair<map<int, string>::iterator, bool> ret = map1.insert(pair<int, string>(4, "four"));
if (ret.second) {
    cout << "插入成功" << endl;
} else {
    cout << "插入失败" << endl;
}
```

map的**数据修改**主要包括以下几种：

- operator[]：通过键值访问元素进行相应的修改，如果键不存在则插入一个默认值。
- swap()：交换两个map容器的元素。

```cpp
map<int, string> map1{{1, "one"}, {2, "two"}, {3, "three"}};
map1[2] = "two modified"; // 修改键值为2的元素对应的值
map<int, string> map2{{5, "five"}, {6, "six"}};
map1.swap(map2); // 交换map1和map2的元素
```

## 四、map的查找与删除

map的**查找操作**主要包括以下几种：

- find()：查找指定键值的元素，如果找到则返回指向该元素的迭代器，否则返回指向map容器末尾的迭代器。
- count()：返回指定键值的元素个数，由于map容器中每个键值只能对应一个元素，因此返回值只能是0或1。但是multimap容器中可以存在多个键值相同的元素，因此返回值可能大于1。
- lower_bound()：返回第一个不小于指定键值的元素的迭代器。
- upper_bound()：返回第一个大于指定键值的元素的迭代器。
- equal_range()：返回一个包含指定键值的元素范围的迭代器对。

```cpp
map<int, string> map1{{1, "one"}, {2, "two"}, {3, "three"}};
map<int, string>::iterator it = map1.find(2); // 查找键值为2的元素
if (it != map1.end()) {
    cout << "找到键值为2的元素，对应的值为：" << it->second << endl;
} else {
    cout << "未找到键值为2的元素" << endl;
}

cout << "键值为3的元素个数为：" << map1.count(3) << endl; // 返回键值为3的元素个数

map<int, string>::iterator lb = map1.lower_bound(2); // 第一个不小于2的元素的迭代器
map<int, string>::iterator ub = map1.upper_bound(2); // 第一个大于2的元素的迭代器
// 返回键值为2的元素范围的迭代器对 对于multimap容器，返回的范围可能包含多个元素
map<int, string>::iterator pair = map1.equal_range(2); 
// 返回键值为2的元素范围的迭代器对 first第一个键值为2的元素的迭代器 second指向第一个大于2的元素的迭代器
for (map<int, string>::iterator it = pair.first; it != pair.second; it++) {
    cout << it->first << " " << it->second << endl;
}
```

map的**删除操作**主要包括以下几种：

- erase()：删除指定键值的元素，或者删除指定迭代器范围的元素。
- clear()：删除map容器中的所有元素。

```cpp
map<int, string> map1{{1, "one"}, {2, "two"}, {3, "three"}};
map1.erase(2); // 删除键值为2的元素
map<int, string>::iterator it = map1.find(3);
if (it != map1.end()) {
    map1.erase(it); // 删除指定迭代器的元素
}
map1.erase(map1.begin(), map1.end()); // 迭代器范围
map1.clear(); // 删除所有元素
```
