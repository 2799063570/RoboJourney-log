# C++ STL string 容器

 在c语言中字符串是字符型数组，同vector、数组一样都是存储在一块连续的内存中，特别的是最后一个元素为'\0'标志字符串的结尾。STL模版中的string类同vector一样都可以自动扩容，本质上都是动态大小的序列容器。我们可以将string理解为vector的一种特殊类型（vector<char>），专门用来存储处理字符串，同时增加了大量字符串特有的功能，使其更适合文本处理。而vector是通用的动态数组模版。
下面我们来看一下string的一些用法，主要都是一些固定用法，这里就相当于是用法总结，忘记咋用的时候回来看看。

## 初始化和赋值操作

初始化和赋值操作相近，**初始化**是变量创建时赋予初始值，赋值是修改变量已有值的操作。初始化发生在变量声明时，目的是为变量设置一个起始值，这可以避免未定义行为。

```cpp
char c[100] = "英雄联盟";
cout << c << endl;            // 打印字符串
cout << (void*)c << endl;     // 打印字符串数组的首地址
 
string s_c = "zhong guo jing qi xian sheng";// string对象
cout << s_c << endl;
 
// 对象初始化
string s1("hello world");						// 字符串
string s2({ '2', '3', 'd', 'd', 't', '5' });	// 字符列表
string s3("英雄哪里出来啊", 4);  // 字符串 前几个个数		
string s4(s3);                  // 字符串对象
string s5(4, 'u');	            // 个数 字符
```

从以上示例内容可以看出，**string初始化**可以利用以下内容进行初始化：

- 字符串常量
- 字符列表
- 字符串常量 和 取字符串的前几个字符
- string实例化的对象
- 字符个数 和 对应的字符
        
**赋值**主要是**对已经存在的对象进行值的修改操作**，主要通过赋值操作符号 = 和 assign函数来实现赋值操作。

```cpp
string s1;
s1 = "字符串常量";  // 字符串字面量赋值 const char[]
 
string s2;
s2 = s1;          // 字符串对象赋值 string 
 
string s3;
s3 = 'a';           // 字符常量赋值 char
 
// assign 赋值
string s4;
s4.assign("字符串常量");  // 传入字符串常量
 
string s5;
s5.assign("字符串常量赋值操作", 6); // 传入字符串常量 个数
 
string s6;
s6.assign(s5);               // 传入字符串对象
 
string s7;
s7.assign(7, 'a');           // 指定字符个数赋值
```

从 以上示例内容可以看出，string赋值操作主要通过（赋值运算符 和 assign函数）和 （以下内容）进行赋值：

- **赋值运算符** ‘ = ’：字符串常量、string对象、字符常量
- **assign函数**：字符串常量、字符串常量和指定字符个数、string对象、字符个数和对应字符

## 拼接操作

字符串的拼接操作指的是 字符串的末尾拼接上其他的字符串。C语言中使用strcat函数实现字符串的拼接。

```cpp
char s1[100] = "hehehe";
char s2[] = "lalala";
 
strcat_s(s1, s2);
 
cout << s1 << endl;    // hehehelalala
```

**strcat(s1, s2)会将s2的内容（不包含自身的终止符）追加到s1的末尾，并在新的结尾添加一个终止符'\0'**。值得注意的是要保证被追加的字符数组的容量是否可以再容纳s2的字符，如果容量过小会出现溢出问题。
**string中实现拼接操作主要借助拼接运算符'+' 、'+=' 以及append、push_back函数实现**。实现起来比较简单。同时呢，string作为特别的vector支持append和push_back两种函数方法。push_back方法较为简单，就是在字符串尾部添加字符。append方法则通过函数重载，可以添加多种类型的参数：

- 字符串
- 字符串 + 要添加的字符串的前几个字符个数
- 字符

拼接方法：

1."+", "+="：拼接字符串或者字符
2.append函数：在字符串的末尾添加字符串、字符串指定的位数
3.push_back函数：在字符串的末尾添加字符

```cpp
string ns = "蒙多";
ns = ns + "想去哪";		// +  const char[] 字符串常量
cout << ns << endl;
string ns1 = "就去哪";	
ns = ns + ns1;			// +  string 字符串对象
cout << ns << endl;
ns = ns + 'a';			// +  char 字符类型
cout << ns << endl;

string ns2 = "蒙多";
ns2 += "想去哪";		// +  const char[] 字符串常量
cout << ns2 << endl;
string ns3 = "就去哪";
ns += ns2;			// +  string 字符串对象
cout << ns2 << endl;
ns2 += 'a';			// +  char 字符类型
cout << ns2 << endl;

// 借助append函数
string nss = "福建";
nss.append("烤");
cout << nss << endl;        // 福建烤
string nss1 = "老鼠";
nss.append(nss1);           // 福建烤老鼠
cout << nss << endl;
nss.append("asdlgkajgdlkaljdks", 5);		// 个数
cout << nss << endl;        // 福建烤老鼠asdlg
nss.append("12345611", 3, 4);					// 位数 个数
cout << nss << endl;        // 福建烤老鼠asdlg4561

string str = "diudiu";
str.push_back('~');
str.push_back('~');
cout << str << endl;        // diudiu~~
```

## 字符串比较

字符串比较分成两种，一种是借用**compare函数实现**，一种是**借用运算符重载**。

**compare函数的返回值类型为int**，因为存在三种情况分别为**小于、等于、大于对应 -1、0、1**。比较规则为：从头开始比较每个字符的大小 相等的话比较下一个字符 一直到不等或者结束。

 “**==、!=、>、<、>=、<=** ”，这六种运算符重载，**结果只有两种情况，因此返回值只有1或0**。

 ```cpp
 string s1 = "hello";
string c11 = "he440";
string c12 = "hello";
string c13 = "zello";
string c14 = "helloooo";
string c15 = "hell";
 
int r11 = s1.compare(c11);
cout << s1 << " compare " << c11 << " = " << r11 << endl;
int r12 = s1.compare(c12);
cout << s1 << " compare " << c12 << " = " << r12 << endl;
int r13 = s1.compare(c13);
cout << s1 << " compare " << c13 << " = " << r13 << endl;
int r14 = s1.compare(c14);
cout << s1 << " compare " << c14 << " = " << r14 << endl;
int r15 = s1.compare(c15);
cout << s1 << " compare " << c15 << " = " << r15 << endl;
 
cout << s1 << " == " << c11 << " = " << (s1 == c11) << endl;
cout << s1 << " != " << c11 << " = " << (s1 != c11) << endl;
cout << s1 << " > " << c11 << " = " << (s1 > c11) << endl;
cout << s1 << " >= " << c11 << " = " << (s1 >= c11) << endl;
cout << s1 << " < " << c11 << " = " << (s1 < c11) << endl;
cout << s1 << " <= " << c11 << " = " << (s1 <= c11) << endl;
```

## 随机访问

**string随机访问**：**在常数时间内直接访问数据结构中的任意元素**，vector可以实现随机访问，那么作为vector的特殊情况，string也可以实现随机访问。借助指定索引，快速访问特定位置的字符。随机访问类比于vector的随机访问(通过[]运算符重载 和 通过at函数)具体区别见vector篇中的介绍。
对string的变量，常常需要知道string对应的**字符长度**，这时候经常用到的函数是**size函数**，该函数可以返回string中字符的个数，值得注意的是size() 的值不包括最后的'/0'。

```cpp
// string 随机访问
string str = "hello world!";
cout << str << endl;	// 访问
for (int i = 0, size = str.size(); i < size; i++)
{
	cout << str[i];
	cout << str.at(i);
}cout << endl;
 
str[1] = '$';
str.at(2) = '$';
cout << str << endl;
```

## 插入与删除

插入和删除，主要借助于insert函数和erase函数

- **insert函数**：输入（插入的位置索引，（个数, 可省略），要插入的字符串或字符）
- **erase函数**：
  - 无参数：全删除
  - 单参数（数字索引\迭代器）：从该索引(迭代器)开始后面的字符全删除
  - (开始删除的索引值，要删除的字符个数)
  - (开始删除的迭代器，结束的迭代器)

```cpp
// string插入
string s = "hello world";
s.insert(5, 2, 'm');	// 位置 个数 字符
cout << s << endl;
s.insert(4, "$");		// 位置 字符串
cout << s << endl;
s.insert(s.size(), " hehe~");
cout << s << endl;
s.insert(s.begin(), 'A');
cout << s << endl;
```

```cpp
string s = "hello wooooooorld";
s.erase();			// 全删
cout << s << endl;
 
s = "hello wooooooorld";
s.erase(7);			// 位置
cout << s << endl;
 
s = "hello wooooooorld";
s.erase(7, 6);		// 位置 个数
cout << s << endl;
 
s = "hello wooooooorld";
s.erase(s.begin());
cout << s << endl;	// 传入迭代器  只删一个字符
 
s = "hello wooooooorld";
s.erase(s.begin(), s.begin() + 6);	// 两个迭代器之间
cout << s << endl;
```

## 查找与替换

string数据查找，主要借助于find函数，替换借助于replace函数

**string数据查找find**：

- 单参数：字符串、字符
- 双参数：（字符串\字符，开始查找的位数）
        
可以查找字符串，字符，返回一个对应的索引。同时也可以输入参数，从字符串的第几个参数开始查找。find函数找到的话会返回下标 找不到的话会返回一个特别的标记npos，一般写作string::npos。当然通过int类型强制转换可以转化为-1

```cpp
string s = "hello wooooooorld";
cout << s.find("oooo") << endl;// 查找的字符串
cout << s.find("oooo", 9) << endl;// 查找的字符串 开始查找的位数
cout << (int)s.find("qwer") << endl;// 如果没有找到就是-1
 
cout << s.find('o') << endl;// 查找的字符
cout << s.find('o', s.find('o') + 1) << endl;// 查找第二个字符
 
cout << s.rfind('o') << endl;// 从右边开始找
```

应用--统计目标字符串在字符串中出现的个数

```cpp
string strs = "12231244322";
string objs = "22";
int index = 0;          // 初始从字符串的第一个字符开始查找
int obj_cnt = 0;         // 目标字符串个数
while ((index = strs.find(objs, index)) != string::npos)
{
	index += objs.length();
	obj_cnt++;
}
cout << obj_cnt << endl;    // 2
```

获取字符串从index位置开始查找目标字符串的位置，如果找到的话（没有到string::npos）。则index为当前找到字符串的位置，在加上目标字符串的长度（查找位置的更新），同时计数+1。开始循环进行下一次的查找，直到找不到的出现（index == string::npos）。

**string数据替换replace**
输入参数（起始位置索引\起始迭代器，结束位置索引\结束迭代器，字符串\字符，(字符串位数)）

```cpp
string s = "hello wooooooorld";
s.replace(7, 7, "o");	// 起始位置 字符个数 替换的字符串
cout << s << endl;        // hello world
 
s = "hello wooooooorld";
s.replace(s.begin() + 7, s.begin() + 14, "o");
cout << s << endl;    // hello world
 
s = "hello wooooooorld";
s.replace(s.begin() + 7, s.begin() + 14, "o123213", 1); // 位数
cout << s << endl;    // hello world
 
s = "hello wooooooorld";
s.replace(7, 7, "o1212", 1);// 起始位置 个数 替换的字符串
cout << s << endl;
```

## string子串

**获取字符串中的部分字符**，主要借助于substr函数，参数(起始索引，个数)

```cpp
string s = "你好！今日有点乏惫";
string subStr = s.substr(6, 12);		// 位置个数
cout << subStr << endl;		// 今日有点乏惫
 
// 应用
s = "灵根育孕源流出$$心性修持大道生";
int pos = s.find("$$");
string s1 = s.substr(0, pos);
string s2 = s.substr(pos + 2);
cout << s1 << " " << s2 << endl;
```