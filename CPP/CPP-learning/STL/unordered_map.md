# C++ STL unordered_map容器

## unordered_map 的基本概念

set和map的区别在于set的元素是一个数据，map的数据是两个元素(pair)。

- `unordered_set(Type)`
- `unordered_map(Type1, Type2)`

set是使用Type对应的数据作为键值（哈希键）进行索引，而map则是利用Type1对应的数据作为键值(哈希键)进行索引。

### 创建unorderd_map

创建的方法主要分为

- 采用默认构造函数
- 采用初始化列表的方式，当然列表可以使用{}也可以使用pair定义
- 采用迭代的方式
- 拷贝构造函数

需要注意的是桶unordered_set一样，unordered_map元素同样是无序的，和插入顺序无关和元素的大小同样没有关系。

```cpp
unordered_map<int, int> um1;    // 默认构造函数
unordered_map<int, int> um2 = {
    pair<int, int>(1, 23),
    pair<int, int>(23, 11),
    pair<int, int>(22, 12),
    pair<int, int>(45, 33)
};                      // 利用初始化列表
unordered_map<int, int> um3 = {
	{1, 23},
	{1, 34},
	{2, 34},
	{6, 56}
};
unordered_map<int, int> um4(um3.begin(), um3.end());
unordered_map<int, int> um5 = um4;  // 拷贝构造函数
```

### unordered_map 赋值操作

赋值则借助于赋值运算符=，借助于已存在的对象或者列表

```cpp
unordered_map<int, int> um = {
    pair<int, int>(23, 12),
    pair<int, int>(34, 32),
    pair<int, int>(36, 23)
};
unordered_map<int, int> um1, um2, um3;
um1 = um;
um2 = {
    pair<int, int>(34, 3),
    pair<int, int>(3213, 3)
};
```

### unordered_map 大小操作

```cpp
unordered_map<int, int> um = {
    pair<int, int>(23, 1),
    pair<int, int>(12, 2)
};
cout << um.size() << endl;
cout << um.empty() << endl;
```

## unordered_map 数据插入

数据插入方法借助于insert函数 和 []重载的方式

```cpp
unordered_map<int, int> um;
um.insert(pair<int, int>(12, 23));
um.insert(make_pair(3, 2));
um.insert({23, 2});
m[34] = 6;

pair<unordered_map<int, int>::iterator, bool> its = um.insert(pair<int, int>(2, 12));
```
