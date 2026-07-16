# C++ 常用算法笔记

> 目标：遇到题目时，先识别模型，再套用可靠的模板。本文按「何时使用 → 核心思路 → 复杂度 → C++ 模板」组织。

## 一、先建立算法意识

### 复杂度速查

| 复杂度 | 常见规模（经验值） | 典型算法 |
| --- | --- | --- |
| `O(log n)` | 很大也可行 | 二分查找 |
| `O(n)` | `10^7` 左右仍常见 | 扫描、双指针、前缀和 |
| `O(n log n)` | `10^6` 左右常见 | 排序、堆、归并 |
| `O(n²)` | 通常 `n ≤ 5000`，视常数而定 | 枚举两两关系、朴素 DP |
| `O(2^n)` / `O(n!)` | 仅适合较小 `n` | 子集枚举、全排列、回溯 |

### 解题时的四个问题

1. 数据规模是多少？它决定可接受的复杂度。
2. 数据是否有序、是否连续、是否有单调性？这决定能否二分或双指针。
3. 问题是在求最优值、计数、可达性，还是构造方案？
4. 选定算法后，边界条件是什么：空数组、重复值、负数、溢出、图不连通？

---

## 二、数组与序列

### 1. 线性枚举

**适用：** 需要逐个检查元素，例如求最值、计数、查找、验证条件。

**核心：** 一次扫描，维护答案或状态。时间复杂度通常为 `O(n)`，额外空间 `O(1)`。

```cpp
#include <vector>
#include <limits>
using namespace std;

int getMax(const vector<int>& a) {
    int ans = numeric_limits<int>::min();
    for (int x : a) ans = max(ans, x);
    return ans;
}

bool contains(const vector<int>& a, int target) {
    for (int x : a)
        if (x == target) return true;
    return false;
}
```

### 2. 模拟

**适用：** 题目把操作过程描述得很具体，只需按规则执行。

**要点：** 先把题意拆成“状态、一次操作、停止条件”三部分；不要急于寻找复杂技巧。

例：不断把一个数的各位相加，直到只剩一位数。

```cpp
int addDigits(int num) {
    while (num >= 10) {
        int sum = 0;
        while (num > 0) {
            sum += num % 10;
            num /= 10;
        }
        num = sum;
    }
    return num;
}
```

### 3. 前缀和

**适用：** 多次查询静态数组的区间和；也常用于“区间和等于某值”的计数问题。

令 `prefix[i]` 表示前 `i` 个数的和（`prefix[0] = 0`），则闭区间 `[l, r]` 的和为：

`sum(l, r) = prefix[r + 1] - prefix[l]`

预处理 `O(n)`，每次查询 `O(1)`。

```cpp
vector<long long> buildPrefix(const vector<int>& a) {
    vector<long long> prefix(a.size() + 1, 0);
    for (int i = 0; i < (int)a.size(); ++i)
        prefix[i + 1] = prefix[i] + a[i];
    return prefix;
}

long long rangeSum(const vector<long long>& prefix, int l, int r) {
    return prefix[r + 1] - prefix[l];
}
```

### 4. 双指针

#### 快慢指针

**适用：** 原地删除元素、去重、链表判环、寻找链表中点。

```cpp
// 删除所有等于 val 的元素，返回新长度。
int removeElement(vector<int>& a, int val) {
    int slow = 0;
    for (int fast = 0; fast < (int)a.size(); ++fast) {
        if (a[fast] != val) a[slow++] = a[fast];
    }
    return slow;
}
```

#### 对撞指针

**适用：** 有序数组的两数之和、回文判断、盛水容器等。

```cpp
bool hasTwoSum(const vector<int>& a, int target) { // a 已升序
    int l = 0, r = (int)a.size() - 1;
    while (l < r) {
        long long sum = 1LL * a[l] + a[r];
        if (sum == target) return true;
        if (sum < target) ++l;
        else --r;
    }
    return false;
}
```

#### 同向双指针（分离双指针）

**适用：** 两个有序序列合并、判断一个序列是否为另一个的子序列。

```cpp
bool isSubsequence(const string& s, const string& t) {
    int i = 0, j = 0;
    while (i < (int)s.size() && j < (int)t.size()) {
        if (s[i] == t[j]) ++i;
        ++j;
    }
    return i == (int)s.size();
}
```

### 5. 滑动窗口

**适用：** 连续子数组/子串，且窗口扩大或缩小时能维护某个条件。

**套路：** 右指针扩张窗口；满足或违反条件时移动左指针；每一步维护答案。通常 `O(n)`，因为每个元素最多进出窗口一次。

```cpp
#include <unordered_map>

// 最长无重复字符子串
int lengthOfLongestSubstring(const string& s) {
    unordered_map<char, int> cnt;
    int ans = 0, left = 0;
    for (int right = 0; right < (int)s.size(); ++right) {
        ++cnt[s[right]];
        while (cnt[s[right]] > 1) --cnt[s[left++]];
        ans = max(ans, right - left + 1);
    }
    return ans;
}
```

### 6. 二分查找

**适用：** 有序数组，或答案具有“满足/不满足”单调性的场景（即“二分答案”）。

最稳妥的写法是维护半开区间 `[l, r)`：循环结束时，`l == r`，它指向第一个满足条件的位置。

```cpp
// 返回第一个 >= target 的下标；若不存在则返回 a.size()。
int lowerBound(const vector<int>& a, int target) {
    int l = 0, r = (int)a.size();
    while (l < r) {
        int mid = l + (r - l) / 2;
        if (a[mid] >= target) r = mid;
        else l = mid + 1;
    }
    return l;
}
```

查找 `target` 的出现区间：左端为 `lower_bound(target)`，右端为 `lower_bound(target + 1) - 1`（注意 `target + 1` 可能溢出，通用场景应写 upper bound）。

---

## 三、递推与动态规划入门

### 1. 递推的三件事

递推/动态规划必须明确：

1. **状态：** `dp[i]` 或 `dp[i][j]` 到底表示什么；
2. **转移：** 当前状态如何由更小的状态得到；
3. **初始值：** 最小规模时答案是多少。

### 2. 一维递推：斐波那契与爬楼梯

`F(n) = F(n - 1) + F(n - 2)`，可把整个数组压缩成两个变量，空间为 `O(1)`。

```cpp
long long climbStairs(int n) { // 每次走 1 或 2 阶
    if (n <= 1) return 1;
    long long a = 1, b = 1;
    for (int i = 2; i <= n; ++i) {
        long long c = a + b;
        a = b;
        b = c;
    }
    return b;
}
```

### 3. 二维递推：网格路径

从左上角走到右下角，每次只能向右或向下：到达 `(i, j)` 的最后一步只能来自上方或左方。

`dp[i][j] = dp[i - 1][j] + dp[i][j - 1]`

```cpp
long long uniquePaths(int m, int n) {
    vector<vector<long long>> dp(m, vector<long long>(n, 1));
    for (int i = 1; i < m; ++i)
        for (int j = 1; j < n; ++j)
            dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
    return dp[m - 1][n - 1];
}
```

---

## 四、排序

实际开发优先使用 `std::sort`：平均 `O(n log n)`，接口稳定可靠。

```cpp
sort(a.begin(), a.end());
sort(a.begin(), a.end(), greater<int>()); // 降序
```

| 算法 | 平均/最坏时间 | 额外空间 | 稳定性 | 适用印象 |
| --- | --- | --- | --- | --- |
| 选择排序 | `O(n²)` / `O(n²)` | `O(1)` | 否 | 教学、交换次数少 |
| 冒泡排序 | `O(n²)` / `O(n²)` | `O(1)` | 是 | 教学；几乎不用 |
| 插入排序 | `O(n²)` / `O(n²)` | `O(1)` | 是 | 小数组、近乎有序 |
| 归并排序 | `O(n log n)` / `O(n log n)` | `O(n)` | 是 | 稳定排序、链表排序 |
| 快速排序 | `O(n log n)` / `O(n²)` | `O(log n)` 栈 | 否 | 常用分治思想 |
| 堆排序 | `O(n log n)` / `O(n log n)` | `O(1)` | 否 | 需保证最坏复杂度 |
| 计数排序 | `O(n + k)` | `O(k)` | 可稳定 | 整数值域 `k` 小 |

### 1. 插入排序

将当前元素插入左侧已经有序的部分。注意用“移动”而非反复交换。

```cpp
void insertionSort(vector<int>& a) {
    for (int i = 1; i < (int)a.size(); ++i) {
        int key = a[i], j = i - 1;
        while (j >= 0 && a[j] > key) {
            a[j + 1] = a[j];
            --j;
        }
        a[j + 1] = key;
    }
}
```

### 2. 归并排序

先分别排序左右两段，再线性合并。合并是关键：两个指针始终指向两段中尚未取出的最小元素。

```cpp
void mergeSort(vector<int>& a, int l, int r) { // 闭区间 [l, r]
    if (l >= r) return;
    int mid = l + (r - l) / 2;
    mergeSort(a, l, mid);
    mergeSort(a, mid + 1, r);
    vector<int> tmp;
    int i = l, j = mid + 1;
    while (i <= mid && j <= r)
        tmp.push_back(a[i] <= a[j] ? a[i++] : a[j++]);
    while (i <= mid) tmp.push_back(a[i++]);
    while (j <= r) tmp.push_back(a[j++]);
    for (int k = 0; k < (int)tmp.size(); ++k) a[l + k] = tmp[k];
}
```

### 3. 快速排序

选取基准值，把数组划分为“小于基准”和“大于等于基准”的两部分，再递归处理。最坏情况可达 `O(n²)`；随机选基准可明显降低风险。

```cpp
void quickSort(vector<int>& a, int l, int r) {
    if (l >= r) return;
    int pivot = a[l + (r - l) / 2];
    int i = l, j = r;
    while (i <= j) {
        while (a[i] < pivot) ++i;
        while (a[j] > pivot) --j;
        if (i <= j) swap(a[i++], a[j--]);
    }
    quickSort(a, l, j);
    quickSort(a, i, r);
}
```

### 4. 计数、桶与基数排序

- **计数排序：** 元素是整数且值域较小。负数可用 `x - minValue` 作下标。
- **桶排序：** 数据分布较均匀，先按范围放入多个桶，再分别排序。
- **基数排序：** 按个位、十位等逐位稳定排序，适合定长非负整数/字符串。

```cpp
void countingSort(vector<int>& a) {
    if (a.empty()) return;
    auto [mn, mx] = minmax_element(a.begin(), a.end());
    vector<int> cnt(*mx - *mn + 1);
    for (int x : a) ++cnt[x - *mn];
    int p = 0;
    for (int i = 0; i < (int)cnt.size(); ++i)
        while (cnt[i]--) a[p++] = i + *mn;
}
```

### 5. 堆与优先队列

**适用：** 动态维护最小/最大值、Top K、Dijkstra 的最小距离点。

```cpp
#include <queue>
priority_queue<int> maxHeap;
priority_queue<int, vector<int>, greater<int>> minHeap;
```

---

## 五、贪心算法

**适用：** 每一步局部最优选择能够导向全局最优的问题。

贪心不是“看起来合理就选”，必须能证明。常见证明方式是**交换论证**：证明任何最优解都能交换成包含当前贪心选择、且不会变差的解。

经典例子：不重叠区间最多选择多少个。按结束时间升序，每次选择当前能接上的、结束最早的区间。

```cpp
int maxNonOverlapping(vector<pair<int, int>> intervals) {
    sort(intervals.begin(), intervals.end(),
         [](auto a, auto b) { return a.second < b.second; });
    int count = 0, lastEnd = numeric_limits<int>::min();
    for (auto [start, end] : intervals) {
        if (start >= lastEnd) {
            ++count;
            lastEnd = end;
        }
    }
    return count;
}
```

---

## 六、图论基础

### 1. 建图

稀疏图通常用邻接表；稠密图或 Floyd 通常用邻接矩阵。

```cpp
struct Edge { int to, w; };
vector<vector<Edge>> graph(n);
graph[u].push_back({v, w});       // 有向边
graph[v].push_back({u, w});       // 无向图再加这一句
```

### 2. DFS（深度优先搜索）

**适用：** 连通块、路径枚举、回溯、树的遍历。时间复杂度 `O(V + E)`。

```cpp
void dfs(int u, const vector<vector<int>>& g, vector<bool>& visited) {
    visited[u] = true;
    for (int v : g[u])
        if (!visited[v]) dfs(v, g, visited);
}
```

全排列是 DFS + 回溯：选择一个未用元素，递归，回来后撤销选择。

```cpp
void permutations(vector<int>& a, int pos, vector<vector<int>>& ans) {
    if (pos == (int)a.size()) { ans.push_back(a); return; }
    for (int i = pos; i < (int)a.size(); ++i) {
        swap(a[pos], a[i]);
        permutations(a, pos + 1, ans);
        swap(a[pos], a[i]);
    }
}
```

### 3. BFS（广度优先搜索）

**适用：** 无权图的最短路、分层遍历、最少操作次数。首次访问某点时得到的距离就是最短距离。

```cpp
vector<int> bfsDistance(int start, const vector<vector<int>>& g) {
    vector<int> dist(g.size(), -1);
    queue<int> q;
    q.push(start);
    dist[start] = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : g[u]) {
            if (dist[v] != -1) continue;
            dist[v] = dist[u] + 1;
            q.push(v);
        }
    }
    return dist;
}
```

### 4. Dijkstra：单源最短路（边权非负）

**适用：** 从一个起点到所有点，且所有边权 `≥ 0`。使用优先队列的复杂度为 `O((V + E) log V)`。

```cpp
using PII = pair<long long, int>; // {距离, 点}
const long long INF = (1LL << 60);

vector<long long> dijkstra(int start, const vector<vector<Edge>>& g) {
    vector<long long> dist(g.size(), INF);
    priority_queue<PII, vector<PII>, greater<PII>> pq;
    dist[start] = 0;
    pq.push({0, start});
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d != dist[u]) continue; // 忽略旧状态
        for (auto [v, w] : g[u]) {
            if (dist[v] > d + w) {
                dist[v] = d + w;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}
```

> 有负边不能用 Dijkstra；无权图应优先用 BFS。

### 5. Bellman–Ford：允许负边

**适用：** 单源最短路、允许负边、需要检测从起点可达的负环。复杂度 `O(VE)`。

连续松弛全部边 `V - 1` 轮；第 `V` 轮仍能更新，说明存在可达负环。

```cpp
struct DirectedEdge { int u, v, w; };

bool bellmanFord(int n, int start, const vector<DirectedEdge>& edges,
                 vector<long long>& dist) {
    dist.assign(n, INF);
    dist[start] = 0;
    for (int i = 1; i < n; ++i) {
        bool changed = false;
        for (auto [u, v, w] : edges) {
            if (dist[u] != INF && dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                changed = true;
            }
        }
        if (!changed) return true;
    }
    for (auto [u, v, w] : edges)
        if (dist[u] != INF && dist[v] > dist[u] + w) return false;
    return true;
}
```

### 6. Floyd–Warshall：全源最短路

**适用：** 顶点数较少（通常 `n ≤ 400`）、需要任意两点最短距离。复杂度 `O(n³)`，空间 `O(n²)`。

`dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j])`：枚举允许经过的中转点 `k`。

```cpp
void floyd(vector<vector<long long>>& dist) {
    int n = dist.size();
    for (int k = 0; k < n; ++k)
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                if (dist[i][k] != INF && dist[k][j] != INF)
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
}
```

### 7. Prim：最小生成树

**目标不同于最短路：** 最小生成树要连接所有顶点，并让所选边的**总权重**最小；它不保证任意两点路径最短。

**适用：** 连通无向带权图。堆优化 Prim 的复杂度为 `O(E log V)`。

```cpp
long long prim(const vector<vector<Edge>>& g) {
    vector<bool> used(g.size(), false);
    priority_queue<PII, vector<PII>, greater<PII>> pq; // {边权, 终点}
    pq.push({0, 0});
    long long total = 0;
    int count = 0;
    while (!pq.empty()) {
        auto [w, u] = pq.top(); pq.pop();
        if (used[u]) continue;
        used[u] = true;
        total += w;
        ++count;
        for (auto [v, cost] : g[u])
            if (!used[v]) pq.push({cost, v});
    }
    return count == (int)g.size() ? total : -1; // -1 表示图不连通
}
```

---

## 七、选择算法的快速清单

| 题目特征 | 优先考虑 |
| --- | --- |
| 一次扫描即可维护答案 | 线性枚举 |
| 连续区间、多次区间和 | 前缀和 |
| 连续子串/子数组，窗口条件可维护 | 滑动窗口 |
| 有序或答案单调 | 二分查找 |
| 数量由更小规模答案推出 | 递推 / DP |
| 两端或两个序列同步移动 | 双指针 |
| 无权图最短步数 | BFS |
| 图的连通性、枚举、回溯 | DFS |
| 单源最短路，边权非负 | Dijkstra |
| 单源最短路，存在负边 | Bellman–Ford |
| 所有点对最短路，点数小 | Floyd–Warshall |
| 无向图连通且总边权最小 | Prim / Kruskal |

## 八、提交前检查

- 下标区间是闭区间还是半开区间？二分、前缀和最容易混淆这里。
- 和、距离、方案数是否可能超过 `int`？必要时使用 `long long`。
- 图是否有重边、自环、负边、不连通情况？
- BFS/DFS 是否在入队/进入递归时立即标记，避免重复访问？
- 最短路中的 `INF + w` 是否被误算？先判断 `INF`。

