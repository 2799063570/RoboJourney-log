# C++ STL list容器

## 一、list容器基本概念

list是一种**双向链表**bidirectional linked list数据结构，允许在任意位置高效地插入和删除元素。逻辑结构同vector相似，都支持双向操作。与vector不同，list不支持随机访问，但在频繁插入和删除操作时性能更优。list适用于需要频繁修改元素顺序的场景，如任务调度、事件处理等。物理结构上是由节点组成Node(val、next、prev)，值、下一指针、上一指针。
不仅仅是一个链表，它是一个**双向循环链表**，即最后一个节点的next指针指向第一个节点，第一个节点的prev指针指向最后一个节点。
![list容器物理结构](./list_img/物理结构.jpg)

进入list容器的底层实现，可以看到只有一个成员变量_Mypair，它是一个压缩对(compressed pair)，用于存储链表的头节点和尾节点指针。可以看到_Mypair由两个元素组成，分布为_Alnode和_Scary_val。_Alnode是链表节点的类型，_Scary_val是一个_List_val<_Val_types>类型，用于存储链表的大小。可以看到_List_val主要是有两个参数，分别是_Myhead和_Mysize。

- _Myhead：指向链表头节点的指针
- _Mysize：链表的大小，即节点的数量

```cpp
_Compressed_pair<_Alnode, _Scary_val> _Mypair;

using _Scary_val = _List_val<_Val_types>;

//initialize data _list_val构造函数
_List_val() noexcept : _Myhead(), _Mysize(0) {}
```

可以发现的是`_Nodeptr _Myhead`, 即每个节点的类型为_Nodeptr，接下来我们分析一个这个数据类型。

```cpp
template <class _Ty>
struct _List_simple_types : _Simple_types<_Ty> {
    using _Node    = _List_node<_Ty, void*>;
    using _Nodeptr = _Node*;
};
```

_Nodeptr是_List_node<_Ty, void*>类型的指针，而_List_node结构体定义如下：

```cpp
struct _List_node { // list node
    using value_type = _Value_type;
    using _Nodeptr   = _Rebind_pointer_t<_Voidptr, _List_node>;
    _Nodeptr _Next; // 下一个节点指针
    _Nodeptr _Prev; // 上一个节点指针
    _Value_type _Myval = // the stored value, unused if head
}
```

可以看到_List_node结构体中包含了三个成员变量：

- _Next：指向下一个节点的指针
- _Prev：指向上一个节点的指针
- _Myval：存储节点的值
  
## 二、list容器的创建

list容器的创建可以通过多种方式进行，主要包括以下几种：

- 默认构造函数：创建一个空的list容器
- 初始化列表构造函数：使用初始化列表初始化list容器
- 拷贝构造函数、移动构造函数
- 元素数量构造函数：指定数量元素、指定的初始值的list容器
- 范围构造函数：使用迭代器范围初始化list容器

```cpp
list<int> mylist; // 默认构造函数

list<int> mylist2 = {1, 2, 3, 4, 5}; // 初始化列表构造函数

list<int> mylist3(mylist2); // 拷贝构造函数
list<int> mylist4(std::move(mylist2)); // 移动构造函数
list<int> mylist5(5, 10); // 个数
list<int> mylist6(mylist2.begin(), mylist2.end()); // 范围构造函数
```

list容器的赋值操作可以通过多种方式进行，主要包括以下几种：

- 赋值运算符重载：使用赋值运算符将一个list容器赋值给另一个list容器
- 赋值函数：迭代器范围、元素数量、初始化列表赋值

```cpp
list<int> mylist6 = {1, 2, 3, 4, 5}; // 初始化列表构造函数
list<int> mylist7 
mylist7 = mylist6; // 赋值运算符重载

mylist7.assign(mylist5.begin(), mylist5.end()); // 范围赋值函数
mylist7.assign(3, 20); // 元素数量赋值函数
mylist7.assign({100, 200, 300}); // 初始化列表赋值
```

## 三、list容器常用操作

list容器的常用操作包括以下几种：

- 元素访问：front()、back()
- 元素插入：insert()、push_front()、push_back()
- 元素删除：erase()、pop_front()、pop_back()
- 容器操作：size()、empty()、clear()、resize()
- 迭代器操作：begin()、end()、rbegin()、rend()
- 排序操作：sort()
- 合并操作：splice()
- 唯一化操作：unique()
- 逆置操作：reverse()

list数据访问主要通过front()和back()函数实现，分别用于访问链表的第一个和最后一个元素。不支持随机访问(底层结构式链表、效率低)。需要注意的是迭代器不支持随机访问，只能通过++、--、+=、-=等操作符进行移动。

```cpp
list<int> mylist = {1, 2, 3, 4, 5};
int getListItemByIndex(list<int>& mylist, int index) {
    if (index < 0 || index >= mylist.size()) {
        throw std::out_of_range("Index out of range");
    }
    list<int>::iterator iter = mylist.begin();
    std::advance(iter, index);
    return *iter;
}
```

insert()有三种重载形式：(迭代器位置、元素数量、(默认元素值))、(迭代器位置、迭代器范围), (迭代器位置、初始化列表)。
erase()有两种重载形式：(迭代器位置)、(迭代器范围)。

```cpp
list<int> mylist = {1, 2, 3, 4, 5};
mylist.insert(mylist.begin(), 100); // 插入一个元素
mylist.insert(mylist.begin(), 2, 200); // 插入多个元素
mylist.insert(mylist.begin(), {300, 400, 500}); // 初始化列表
mylist.insert(mylist.begin(), mylist.begin(), mylist.end()); // 迭代器范围

mylist.erase(mylist.begin()); // 删除一个元素
list<int>::iterator iter = mylist.erase(mylist.begin(), --mylist.end()); // 删除多个元素 返回删除最后一个元素的下一个位置
mylist.clear(); // 清空容器
```

list反转操作可以通过reverse()函数实现，用于将链表中的元素顺序反转。

```cpp
void reverse() noexcept { // reverse sequence存储头节点
    const _Nodeptr _Phead = _Mypair._Myval2._Myhead;
    _Nodeptr _Pnode       = _Phead;

    for (;;) { // flip pointers in a node 指针交换
        const _Nodeptr _Pnext = _Pnode->_Next;
        _Pnode->_Next         = _Pnode->_Prev;
        _Pnode->_Prev         = _Pnext;

        if (_Pnext == _Phead) {
            break; // 循环双端节点，一直遍历到头节点
        }

        _Pnode = _Pnext;
    }
}
```

```cpp
list<int> mylist = {4, 2, 6, 5, 3, 1};

mylist.sort();  // 排序

void cmp(int a, int b) {
    return a > b;
}

mylist.sort(cmp); // 自定义排序
```

我们进入sort函数的源码分析一下，可以看到sort的输入为`_Pr2 _Pred`，即排序谓词。排序谓词是一个函数对象，用于定义排序的规则。如果不提供排序谓词，默认使用`<`运算符进行排序。
采用的排序方法为先将链表分成两个子链表，分别对两个子链表进行排序，最后合并两个子链表。我们重点分析一下`_Merge_same`函数。

```cpp
template <class _Pr2>
void sort(_Pr2 _Pred) { // order sequence
    auto& _My_data = _Mypair._Myval2;
    _Scary_val::_Sort(_My_data._Myhead->_Next, _My_data._Mysize, _STD _Pass_fn(_Pred));
}

template <class _Pr2>
static _Nodeptr _Sort(_Nodeptr& _First, const size_type _Size, _Pr2 _Pred) {
    // order [_First, _First + _Size), return _First + _Size
    switch (_Size) {
    case 0:
        return _First;
    case 1:
        return _First->_Next;
    default:
        break;
    }

    auto _Mid        = _Sort(_First, _Size / 2, _Pred);
    const auto _Last = _Sort(_Mid, _Size - _Size / 2, _Pred);
    _First           = _Merge_same(_First, _Mid, _Last, _Pred);
    return _Last;
}
```

```cpp
template <class _Pr2>
static _Nodeptr _Merge_same(_Nodeptr _First, _Nodeptr _Mid, const _Nodeptr _Last, _Pr2 _Pred) {
    // Merge the sorted ranges [_First, _Mid) and [_Mid, _Last)
    // Returns the new beginning of the range (which won't be _First if it was spliced elsewhere)
    _STL_INTERNAL_CHECK(_First != _Mid && _Mid != _Last);
    _Nodeptr _Newfirst;
    if (_DEBUG_LT_PRED(_Pred, _Mid->_Myval, _First->_Myval)) {
        // _Mid will be spliced to the front of the range
        _Newfirst = _Mid;
    } else {
        // Establish _Pred(_Mid->_Myval, _First->_Myval) by skipping over elements from the first
        // range already in position.
        _Newfirst = _First;
        do {
            _First = _First->_Next;
            if (_First == _Mid) {
                return _Newfirst;
            }
        } while (!_DEBUG_LT_PRED(_Pred, _Mid->_Myval, _First->_Myval));
    }

    for (;;) { // process one run splice
        auto _Run_start = _Mid;
        do { // find the end of the "run" of elements we need to splice from the second range into the first
            _Mid = _Mid->_Next;
        } while (_Mid != _Last && _DEBUG_LT_PRED(_Pred, _Mid->_Myval, _First->_Myval));

        // [_Run_start, _Mid) goes before _First->_Myval
        _Unchecked_splice(_First, _Run_start, _Mid);
        if (_Mid == _Last) {
            return _Newfirst;
        }

        // Reestablish _Pred(_Mid->_Myval, _First->_Myval) by skipping over elements from the first
        // range already in position.
        do {
            _First = _First->_Next;
            if (_First == _Mid) {
                return _Newfirst;
            }
        } while (!_DEBUG_LT_PRED(_Pred, _Mid->_Myval, _First->_Myval));
    }
}
```

_DEBUG_LT_PRED就是排序函数

```cpp
#define _DEBUG_LT_PRED(pred, x, y) _STD _Debug_lt_pred(pred, x, y)
```

我们分析一下过程，_Merge_same函数的输入为四个参数，分别是第一个子链表的头节点_First，第二个子链表的头节点_Mid，第二个子链表的尾节点_Last，排序谓词_Pred。函数的返回值是新的头节点_Newfirst。
函数首先比较两个子链表的头节点，如果第二个子链表的头节点小于第一个子链表的头节点，则将第二个子链表的头节点作为新的头节点_Newfirst。否则，遍历第一个子链表，找到第一个大于第二个子链表头节点的位置，将第二个子链表插入到该位置之前。然后继续处理下一个元素，直到所有元素都被处理完毕。
