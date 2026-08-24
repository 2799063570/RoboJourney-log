# C++ 之 数据结构

11.18 目前对一些容器的实现，基本也已经忘得干净了
尤其是后面的几个容器 树、邻接矩阵、邻接表、哈希表

先复习一下树

## 树结构

树类似于链表，也是由一个个节点组成，例如每个节点可以由下列组成

```cpp
struct TreeNode {
    int val; // 节点值
    TreeNode* left; // 左子节点指针
    TreeNode* right; // 右子节点指针
    TreeNode* parent; // 父节点指针（可选）
    TreeNode(int x) : val(x), left(nullptr), right(nullptr), parent(nullptr) {} // 构造函数
};
```

树呢，则是会保存一个根节点的地址和一些操作函数，例如插入、删除、查找等。树有很多种类，例如**二叉树**（左斜树、右斜树、满二叉树、完全二叉树）、**二叉搜索树**、**平衡树**（如AVL树、红黑树）、**堆**（如二叉堆）等。

左斜树、右斜树、满二叉树、完全二叉树都是在结构形态上的不同，完全二叉树是除了最后一层外，其他层的节点都被填满，并且最后一层的节点尽可能地靠左排列。

二叉搜索树（BST）是一种特殊的二叉树，满足左子节点的值小于父节点的值，右子节点的值大于父节点的值。这种性质使得查找、插入和删除操作都能在平均O(log n)时间内完成。
但是当二叉搜索树退化成链表时，最坏情况下的时间复杂度会变为O(n)。
那么是如何退化的呢？例如按顺序插入一组数据，比如 1, 2, 3, 4, 5。由于每次插入的值都大于当前节点的值，所有新节点都会被插入到右子节点的位置，导致树变成了一条单链表。所以就丧失了二分查找的能力。

为了解决这个问题，我们需要一种机制，强迫树在生长时保持“胖胖的”（矮而宽），而不是“瘦瘦的”（高而窄）。因此平衡树就提出来了，这里主要介绍红黑树，红黑树是一种自平衡的二叉搜索树，它通过对节点进行颜色标记（红色或黑色）和旋转操作来保持树的平衡。

红黑树对应五条规则：

1. 每个节点非黑即红
2. 根节点必须是黑色
3. 所有的空节点都被视为黑色
4. 红色节点的两个子节点必须是黑色
5. 黑高一致，从任意节点出发，走到任意一个 NIL 叶子，经过的黑色节点数量必须相同。

同时对应两种旋转操作：左旋和右旋，用于调整树的结构以保持平衡。左旋是将一个节点的右子节点提升为该节点的父节点，而右旋则是将一个节点的左子节点提升为该节点的父节点。

```Plaintext
[左旋前]                     [左旋后]
   X  (旧父)                  Y  (新父)
 /   \                      /   \
α     Y          ==>       X     γ
     / \                  / \
    β   γ                α   β <-- 注意 β 的位置变化！
          
[右旋前]                 [右旋后]
   Y  (旧父)              X  (新父)
 /   \                  /   \
X     γ        ==>     α     Y
/ \                          / \
α   β                       β   γ <-- 注意 β 的位置变化！
```

需要注意的是左旋时，β节点从Y的左子节点变成了X的右子节点；右旋时，β节点从X的右子节点变成了Y的左子节点。

二叉堆时一种线性模拟树，在物理上时存储在列表中，逻辑上是一种树型结构，分为最大堆和最小堆。最大堆中每个节点的值都大于或等于其子节点的值，最小堆则相反。二叉堆常用于实现优先队列。
数据会存储在列表中，满足以下父子节点下标关系(针对在数值中索引)：

- 父节点的下标 = (子节点的下标 - 1) / 2
- 左子节点的下标 = 父节点的下标 * 2 + 1 （奇数）
- 右子节点的下标 = 父节点的下标 * 2 + 2 （偶数）

```cpp
function lson(idx): 
    return idx * 2 + 1
function rson(idx):
    return idx * 2 + 2
function parent(idx):
    return (idx - 1) / 2
```

对于**堆的插入**，每个新的节点都会push_back在列表的最后面，然后需要通过上浮的不断比较，来调整堆的结构，保持堆的性质(父节点大于\小于子节点)，具体上浮过程在priority_queue中已经详细展示。上浮的实现可以利用递归实现：

```cpp
// heap 表示堆的数组表示，curr 表示当前节点的索引
function shiftUp(heap, curr):
    if(curr == 0)   // 已经到达根节点
        return
    parentIdx = parent(curr)
    if(heap[parentIdx] < heap[curr]): // 最大堆
        swap(heap[parentIdx], heap[curr])
        shiftUp(heap, parentIdx) // 当前节点更新为父节点
```

对于**堆的删除**，往往是删除堆顶节点，然后将最后一个节点移动到堆顶位置，再通过下沉操作调整堆的结构，保持堆的性质。相对于上浮，下沉的程序稍微多一点，因为下沉需要比较要下沉的点(父节点)与两个子节点。下沉的实现也可以利用递归实现：

```cpp
// heap 堆的数组，heapSize 堆的大小，curr 当前节点的索引
function shiftDown(heap, heapSize, curr):
    leftIdx = lson(curr)  // 获取左右索引
    rightIdx = rson(curr)
    largestIdx = curr       // 存储最大值索引

    // 求取最大值索引
    if(leftIdx < heapSize and heap[leftIdx] > heap[largestIdx]): // 最大堆
        largestIdx = leftIdx
    if(rightIdx < heapSize and heap[rightIdx] > heap[largestIdx]):
        largestIdx = rightIdx

    if(largestIdx != curr):
        swap(heap[curr], heap[largestIdx])
        // 当前节点更新为最大值索引 即交换的子节点
        shiftDown(heap, heapSize, largestIdx) 
``` 

所以插入和删除的操作就明了了：

```cpp
function insert(heap, value):
    heap.push_back(value) // 插入到最后
    shiftUp(heap, heap.size() - 1) // 上浮调整

function deleteTop(heap):
    if(heap.size() == 0) // 堆为空
        return 
    heap[0] = heap[heap.size() - 1] // 用最后元素覆盖堆顶
    heap.pop_back() // 删除最后元素
    shiftDown(heap, heap.size(), 0) // 下沉调整
```

## 哈希表

首先为什么要引入哈希表？因为在很多情况下，**我们需要快速地进行数据的插入、删除和查找操作**，而传统的数据结构如数组和链表在这些操作上的效率可能不够高。哈希表通过使用哈希函数将键映射到数组的索引，从而**实现了平均O(1)时间复杂度的插入、删除和查找操作**。

那么是如何实现的呢？
1. 数组的随机访问：利用连续存储的机制，只需要通过算出偏移量就可以直接访问到数据
2. 哈希函数：将输入的键映射为数组的索引，通常通过取模运算实现
3. 哈希冲突的处理：当多个键映射到同一个索引时，需要有机制来处理冲突，常见的方法有链地址法和开放地址法

当我们插入一个值的时候，首先计算该值的位置，判断该值存不存在，不存在则创建。而对于删除和查找也是类似的过程，先计算位置，然后在对应位置进行删除或查找。

关于哈希冲突的处理，主要分为链地址法和开放地址法两种。

### 链地址法

链地址法是为每个桶维护一个链表（或其他数据结构），当发生冲突时，将新元素插入到对应桶的链表中。查找和删除操作需要遍历链表来找到目标元素。因此可以想象成把键值映射到一个链表数组中。

```cpp
// 假设 buckets 是数组，存链表头指针
// Node 结构: { key, value, next }

function unorderedMapInsert(key, val)
    // 1. 通过键值计算哈希值
    index = hash(key) % HMAX
    
    // 2. 看桶对应的链表，检查是否存在相同 Key
    current = buckets[index]    // 取出桶的头指针
    while (current != NULL)
        if (current.key == key)
            return  // 发现 Key 已存在，插入失败    
        current = current.next

    // 3. 没找到 Key，创建新节点 
    newNode = createNode()
    newNode.key = key
    newNode.value = val    // 把数据存进去
    
    // 4. 头插法挂载
    newNode.next = buckets[index]
    buckets[index] = newNode
```


```cpp
function unorderedMapErase(key)   
    index = hash(key) % HMAX    // 1. 计算哈希位置
    current = buckets[index]    // 取出桶的头指针
    prev = NULL     // 记录前驱节点

    // 2. 遍历链表
    while (current != NULL)
        // 找到要删除的目标
        if (current.key == key)
            
            // --- 情况 A: 要删的是头节点 (Head) ---
            if (prev == NULL)
                // 直接让桶的头指针指向下一个
                buckets[index] = current.next
            
            // --- 情况 B: 要删的是中间或尾部节点 ---
            else
                // 让前一个节点的 next 跳过当前节点，连到下一个去
                prev.next = current.next
            
            // 3. 释放内存 (C++ 中必须显式 delete)
            deleteNode(current)
            return // 删除成功，返回

        // 继续往后找，prev 紧跟在 current 后面
        prev = current
        current = current.next
```

```cpp
function unorderedMapSet(key, value)
    index = hash(key) % HMAX    // 计算哈希位置
    current = buckets[index]    // 取出桶的头指针

    while(current != nullptr)
        if (current.key == key)
            current.value = value // 找到 Key，更新值
            return
        current = current.next
    // 没找到 Key，插入新节点
    newNode = createNode()
    newNode.key = key
    newNode.value = value
    newNode.next = buckets[index]
    buckets[index] = newNode
```

### 开放地址法

开放地址法是在发生冲突时，通过探测下一个可用的桶位置来存储元素。常见的探测方法有线性探测、二次探测和双重哈希。下面以线性探测为例进行说明。

```cpp
function unorderedMapInsert(key, value)
    int index = hash(key) % HMAX    // 计算哈希位置
    while(1)
        if (buckets[index] is empty)
            // 找到空位，插入新节点
            newNode = createNode()
            newNode.key = key
            newNode.value = value
            buckets[index] = newNode
            return
        else if (buckets[index].key == key)
            // Key 已存在，更新值
            buckets[index].value = value
            return
        else
            // 发生冲突，尝试下一个位置 (线性探测)
            index = (index + 1) % HMAX
```

```cpp
function unorderedMapErase(key)
    index = hash(key) % HMAX    // 计算哈希位置

    while(1)
        if (buckets[index] is empty)
            return // Key 不存在，删除失败
        else if (buckets[index].key == key)
            // 找到 Key，删除节点
            deleteNode(buckets[index])
            buckets[index] = DELETED_MARKER // 标记为空
            return
        else
            // 继续探测下一个位置
            index = (index + 1) % HMAX
```

```cpp
function unorderedMapSet(key, value)
    index = hash(key) % HMAX    // 计算哈希位置

    while(1)
        if (buckets[index] is empty)
            // 找到空位，插入新节点
            newNode = createNode()
            newNode.key = key
            newNode.value = value
            buckets[index] = newNode
            return
        else if (buckets[index].key == key)
            // Key 已存在，更新值
            buckets[index].value = value
            return
        else
            // 发生冲突，尝试下一个位置 (线性探测)
            index = (index + 1) % HMAX
```

## 邻接表、邻接矩阵

邻接表和邻接矩阵都是图的常见表示方法。图就是用点和线段表示的一种数据结构，点表示节点，线段表示节点之间的连接关系。
在图中，我们主要需要实现一种权衡：空间复杂度和时间复杂度之间的权衡（存储空间大小 和 查找速度快慢之间）。可能会出现两种图，一种是稀疏图，点很多，边很少；另一种是稠密图，点很多，边也很多。因此我们可以根据图的类型选择合适的表示方法。

### 邻接矩阵

邻接矩阵直接用一个二维数组表示图的连接关系，每个索引代表一个节点，数组中的值表示节点之间是否有连接（边）。如果节点i和节点j之间有边，则矩阵的`[i][j]`位置为1（或权重值），否则为0。

我们可以发现，邻接矩阵的空间复杂度为O(V^2)，其中V是图中的节点数。这意味着即使图中只有少量的边，邻接矩阵仍然会占用大量的空间。因此，邻接矩阵更适合用于表示稠密图，即边的数量接近于节点数量的平方的图。
同时，邻接矩阵的时间复杂度为O(1)，因为我们可以直接通过索引访问矩阵中的值，快速判断两个节点之间是否有边。例如我们想看节点A和节点B之间是否有边，只需要检查矩阵的`[A][B]`位置的值即可。

### 邻接表
邻接表使用一个数组或列表来存储每个节点的邻居节点列表。每个节点都有一个链表或动态数组，存储与该节点直接相连的其他节点。只记录“发生了的关系”，没发生的不记。
例如我们开一个数组 adj，数组里的每一格存一个链表（或 `vector`）。

- adj[张三]：-> [李四] -> [王五] -> [NULL]
- adj[赵六]：-> [NULL] （他没朋友）

邻接表的空间复杂度为O(V + E)，其中V是节点数，E是边数。这使得邻接表非常适合用于表示稀疏图，即边的数量远小于节点数量的平方的图。因为邻接表只存储实际存在的边，所以在稀疏图中可以节省大量的空间。
缺点在于查找上，邻接表的时间复杂度为O(k)，其中k是节点的度（与该节点相连的边数）。这意味着在最坏情况下，我们可能需要遍历整个邻居列表才能找到特定的边。因此，邻接表在查找边时可能不如邻接矩阵高效。
