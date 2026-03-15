

## malloc/free

`malloc`（memory allocation，内存分配）和 `free` 是 C 语言中用于**动态内存管理**的核心函数（C++ 完全兼容，但更推荐 `new/delete`）——`malloc` 负责从堆（heap）中申请指定大小的内存，`free` 负责释放 `malloc` 申请的内存，二者必须**成对使用**，否则会导致内存泄漏、野指针等问题。

```cpp
void* malloc(size_t size);
```

- **参数**：`size` 是要申请的内存字节数（`size_t` 是无符号整数类型，通常等价于 `unsigned int`）；
- **返回值**：
    - 成功：返回指向申请到的内存起始地址的 `void*` 指针（需手动强制类型转换为对应类型）；
    - 失败：返回 `NULL`（如内存不足时）；
- **核心特性**：只分配内存，**不初始化**（内存中是随机的垃圾值）。

```cpp
void free(void* ptr);
```
- **参数**：`ptr` 是 `malloc`（或 `calloc`/`realloc`）返回的内存指针；
- **返回值**：无；
- **核心特性**：仅释放内存，不修改指针本身（指针仍指向原地址，成为 “野指针”）。

基本的使用流程如下：
- 计算申请内存的大小：`n*sizeof(int)`
- 调用malloc分配内存，将返回的指针强制转换
- 对内存进行初始化，例如调用memset
- 对内存的正常使用，参考数组形式调用
- 调用free释放内存
- 将接收返回堆地址的指针指向空（防止变成野指针）
```cpp
#include <iostream>
#include <cstdlib> // malloc/free 头文件
#include <cstring> // memset 头文件（用于初始化内存）
using namespace std;

int main() {
    // 步骤1：定义要申请的内存大小（比如申请5个int类型的空间）
    int n = 5;
    size_t size = n * sizeof(int); // 计算字节数：int占4字节 → 5*4=20字节

    // 步骤2：调用malloc申请内存，强制转换为int*，并检查是否成功
    int* arr = (int*)malloc(size);
    if (arr == NULL) { // 必须检查NULL！避免空指针访问
        cout << "内存分配失败" << endl;
        return 1;
    }

    // 步骤3：初始化内存（malloc分配的内存是垃圾值，需手动初始化）
    memset(arr, 0, size); // 把arr指向的20字节全部置0
    // 或逐个赋值：for (int i=0; i<n; i++) arr[i] = i+1;

    // 步骤4：使用内存
    cout << "使用malloc分配的内存：" << endl;
    for (int i = 0; i < n; i++) {
        arr[i] = i + 1; // 赋值：1,2,3,4,5
        cout << arr[i] << " ";
    }
    cout << endl;

    // 步骤5：释放内存（必须！否则内存泄漏）
    free(arr);
    // 步骤6：将指针置NULL（避免野指针）
    arr = NULL;

    return 0;
}
```

需要注意以下几方面常见的错误：

- 申请过大的内存
- 重复free
- free栈内存
- free后不对指针置空，变成野指针

