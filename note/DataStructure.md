> 以下均为个人总结

# 复杂度

时间复杂度：算法中基本操作的执行次数

- 时间复杂度分为最优、平均和最坏，一般只考虑最坏时间复杂度
- 对于有随机行为的算法，则应考虑平均时间复杂度

空间复杂度：算法需要的额外空间

大 $O$ 表示法：常数都改成 1，且只保留最高阶项

# 数组和链表

访问效率：数组可以通过下标访问任意元素，时间复杂度为 $O(1)$，链表访问某个元素需要遍历到目标位置，时间复杂度为 $O(n)$

插入删除：数组只有尾部插入删除是 $O(1)$，其他位置插入删除需要挪动元素，时间复杂度为 $O(n)$，链表插入删除只需要修改指针指向，时间复杂度是 $O(1)$ 的

缓存命中：数组在内存中是连续的，缓存命中高，链表在内存中不一定连续，缓存命中低

适用场景：数组适合频繁访问元素的场景，链表适合频繁插入删除的场景

# 栈和队列

栈：后进先出，允许在栈顶插入删除

队列：先进先出，允许队尾插入队头删除

# 树

树是一种非线性的数据结构，由结点和边组成，每个结点可以有多个子结点，适合用来表示层次关系

> 树的性质：树的点数为 n，则边数为 n - 1

# 二叉树

二叉树就是度不大于 2 的树

> 二叉树的性质：度为 0 的点数 = 度为 2 的点数 + 1

满二叉树就是每层都是满的二叉树

> 满二叉树的点数：$2^h - 1$​

完全二叉树的最后一层可以不满，但是从左到右必须连续（满二叉树是完全二叉树的一种）

> 完全二叉树的点数范围：$[2^{h-1}, 2^h-1]$
>
> 完全二叉树的性质：度为 1 的点数 = 0 或 1

## 堆

堆是一种完全二叉树

每个结点都大于等于子结点的叫大顶堆，每个结点都小于等于子结点的叫小顶堆，这里以大顶堆为例

```cpp
template<typename T>
class heap {
    std::vector<T> a; //完全二叉树一般用数组存储，用下标关系找到父结点和子结点
};
```

上浮 $O(logn)$

```cpp
void up(int index) {
    int parent = (index - 1) / 2;
    while (index >= 0) {
        if (a[parent] < a[index]) {
            std::swap(a[parent], a[index]);
            index = parent;
            parent = (index - 1) / 2;
        } else {
            break;
        }
    }
}
```

下沉 $O(logn)$

```cpp
void down(int index, int n) {
    int child = index * 2 + 1;

    while (child < n) {
        if (child + 1 < n && a[child] < a[child + 1]) {
            child++;
        }

        if (a[index] < a[child]) {
            std::swap(a[index], a[child]);
            index = child;
            child = index * 2 + 1;
        } else {
            break;
        }
    }
}
```

建堆 $O(n)$

```cpp
void heapify() {
    int n = a.size();
    for (int i = (n - 1 - 1) / 2; i >= 0; i--) {
    	down(i, n);
    }
}
```

插入 $O(logn)$

```cpp
void push(const T& x) {
    a.push_back(x);
    up(a.size() - 1);
}
```

删除 $O(logn)$

```cpp
void pop() {
    std::swap(a.front(), a.back());
    a.pop_back();
    down(0, a.size());
}
```

堆排序 $O(nlogn)$

```cpp
void heapSort() {
    heapify();
    int n = a.size();
    for(int i = n - 1; i > 0; i--) {
        std::swap(a[0], a[i]);
        down(0, i);
    }
}
```

## 二叉搜索树

二叉搜索树，左子树的结点都小于根节点，右子树的结点都大于根结点，且左右子树也是二叉搜索树

二叉搜索树的中序遍历序列是升序的

二叉搜索树增删查改的时间复杂度为 $O(h)$，理想情况下是 $O(logn)$。最坏情况是插入序列有序或者接近有序，此时二叉搜索树会退化成单支树，时间复杂度是 $O(n)$，解决方法：平衡树

## AVL树

AVL 树是一种严格平衡的二叉搜索树，每个结点的左右子树高度差（平衡因子）不超过 1

通过自平衡的特性，AVL 树的时间复杂度可以维持在 $O(logn)$

由于 AVL 树的平衡特性较为严格，维持平衡的旋转操作比较频繁，插入性能较差

## 红黑树

红黑树是一种近似平衡的二叉搜索树：每个结点都有一个颜色（红或黑），根结点和空结点为黑色，红色结点的两个子结点为黑色，每条路径的黑色结点数量相同

通过这些特性，红黑树可以确保最长路径不超过最短路径的两倍（路径是从根节点到空结点），从而维持时间复杂度为 $O(logn)$

由于红黑树是近似平衡，所以查找性能略差于 AVL 树，但插入性能优于 AVL 树

# 哈希表

哈希表就是利用哈希函数（比如 `(x % N + N) % N`）把键映射到某个位置，然后将元素（键值对）存储在这个位置，平均时间复杂度为 $O(1)$

哈希冲突就是不同的键通过哈希函数映射到了同一个位置，解决办法：链地址法，开放寻址法（线性探测、二次探测等）

负载因子：存储的元素个数 / 哈希表长度

当负载因子较高时，很容易发生哈希冲突，过多的哈希冲突会严重影响性能，一般会设置一个阈值，用于触发扩容和 rehash

哈希表的缺点：

- 无序
- 性能依赖哈希函数（如 std::unordered_map 对于自定义类型的键需特化 std::hash）
- 有较多的空间浪费
- 有扩容和 rehash 的开销

# 排序算法

这里均以排升序为例

## 稳定性

排序算法的稳定性是指，排序前后，相等元素的相对顺序是否发生改变

稳定的排序有：冒泡排序、插入排序、归并排序、计数排序、基数排序

不稳定的排序有：选择排序、堆排序、快速排序、希尔排序

## 选择排序

每次选出最小的元素换到最左边

稳定性：不稳定

时间复杂度：$O(n^2)$

空间复杂度：$O(1)$

```cpp
for(int i = 0; i < n; i++) {
    int mini = i;
    for(int j = i + 1; j < n; j++) {
        if(a[j] < a[mini]) {
            mini = j;
        }
    }
    std::swap(a[i], a[mini]);
}
```

## 冒泡排序

通过相邻元素的比较和交换，每次将最大的元素推到最右边

稳定性：稳定

时间复杂度：最优 $O(n)$，最坏 $O(n^2)$

空间复杂度：$O(1)$

```cpp
for(int i = 0; i < n - 1; i++) {
    bool exchange = false;
    for(int j = 0; j < n - 1 - i; j++) {
        if(a[j] > a[j + 1]) {
            exchange = true;
            std::swap(a[j], a[j + 1]);
        }
    }
    if(!exchange) {
        break;
    }
}
```

## 快速排序

通过划分将待排序列分成前后两部分，前一部分的数据小于等于后一部分的数据，再递归对两部分的序列分别进行快速排序

稳定性：不稳定

时间复杂度：平均 $O(nlogn)$。最坏是 $O(n^2)$（每次选的分界值都是序列的最大值或最小值，解决办法：随机选择分界值）

空间复杂度：$O(logn)$

```cpp
std::mt19937 gen;

void quickSort(int a[], int l, int r) {
    if(l >= r) {
        return;
    }
    
    int i = l - 1, j = r + 1;
    int x = a[l + gen() % (r - l + 1)]; //分界值x可以随便取
    while(i < j) {
        while(a[++i] < x);
        while(a[--j] > x);
        if(i < j) {
            std::swap(a[i], a[j]);
        }
    }
    
    quickSort(a, l, j);     //[l, j] <= x
    quickSort(a, j + 1, r); //[j + 1, r] >= x
}
```

## 插入排序

类似扑克牌洗牌，每拿到一张牌，插入到合适的位置

稳定性：稳定

时间复杂度：最优 $O(n)$，最坏 $O(n^2)$

空间复杂度：$O(1)$

```cpp
for(int i = 0; i < n; i++) {
    int t = a[i];
    int j = i - 1;
    while(j >= 0 && a[j] > t) {
        a[j + 1] = a[j];
        j--;
    }
    a[j + 1] = t;
}
```

## 归并排序

将待排序列分解成两个子序列，先递归调用函数使两个子序列有序，再有序合并两个子序列，使整个序列达到有序

稳定性：稳定

时间复杂度：$O(nlogn)$

排序数组，空间复杂度为 $O(n)$

```cpp
int tmp[N];

void mergeSort(int a[], int l, int r) {
    if(l >= r) {
        return;
    }
    
    int mid = (l + r) / 2;
    mergeSort(a, l, mid);
    mergeSort(a, mid + 1, r);
    
    int i = l, j = mid + 1, k = l;
    while(i <= mid && j <= r) {
        if(a[i] <= a[j]) {
            tmp[k++] = a[i++];
        } else {
            tmp[k++] = a[j++];
        }
    }
    
    while(i <= mid) {
        tmp[k++] = a[i++];
    }
    while(j <= r) {
        tmp[k++] = a[j++];
    }
    
    for(int i = l; i <= r; i++) {
        a[i] = tmp[i];
    }
}
```

排序链表，空间复杂度为 $O(logn)$

```cpp
ListNode* sortList(ListNode* head) {
    //length: 0 or 1
    if(head == nullptr || head->next == nullptr) {
        return head;
    }

    //length >= 2
    auto slow = head, fast = head;
    while(fast->next && fast->next->next) {
        fast = fast->next->next;
        slow = slow->next;
    }
    auto head2 = slow->next;
    slow->next = nullptr;

    auto i = sortList(head);
    auto j = sortList(head2);
    auto dummy = new ListNode, cur = dummy;
    while(i && j) {
        if(i->val <= j->val) {
            cur->next = i;
            i = i->next;
        } else {
            cur->next = j;
            j = j->next;
        }
        cur = cur->next;
    }
    if(i) {
        cur->next = i;
    }
    if(j) {
        cur->next = j;
    }
    auto res = dummy->next;
    delete dummy;
    return res;
}
```

# 基础算法

## 二分

二分就是根据某个性质，将一段区间分为有这个性质和没这个性质的两段，二分答案就是这两段区间的边界

1. 确定答案区间
2. 考虑用什么性质来二分
3. 判断题目是否有解

## 高精度

高精度加法

```cpp
//读入到字符串
//逆置

std::vector<int> res;
int t = 0;
for(size_t i = 0; i < a.size() || i < b.size() || t; i++) {
    if(i < a.size()) {
        t += a[i] - '0';
    }
    if(i < b.size()) {
        t += b[i] - '0';
    }
    res.push_back(t % 10);
    t /= 10;
}
std::reverse(res.begin(), res.end());
```

高精度乘法

```cpp
//读入到字符串
//逆置

std::vector<int> res(a.size() + b.size());
for(int i = 0; i < a.size(); i++) {
    for(int j = 0; j < b.size(); j++) {
        res[i + j] += (a[i] - '0') * (b[j] - '0');
    }
}
for(int i = 0; i < res.size() - 1; i++) {
    res[i + 1] += res[i] / 10;
    res[i] %= 10;
}
while(res.size() > 1 && res.back() == 0) {
    res.pop_back();
}
std::reverse(res.begin(), res.end());
```

## 差分

一维

```cpp
void insert(int l, int r, int c) {
    a[l] += c;
    a[r + 1] -= c;
}
```

二维

```cpp
void insert(int x1, int y1, int x2, int y2, int c) {
    a[x1][y1] += c;
    a[x2 + 1][y1] -= c;
    a[x1][y2 + 1] -= c;
    a[x2 + 1][y2 + 1] += c;
}
```

## 单调栈

单调栈就是维护一个单调递增或单调递减的栈，将遍历的操作优化成取栈顶

求每个数左边第一个比它小的数

```cpp
std::stack<int> stk;
for(int i = 0; i < n; i++) {
    while(!stk.empty() && stk.top() >= a[i]) { //单调递增 求左边第一个比它小的数 取栈顶即可
        stk.pop();
    }
    std::cout << (stk.empty() ? -1 : stk.top()) << ' ';
    stk.push(a[i]);
}
```

## 单调队列

单调队列就是维护一个单调递增或单调递减的队列，将遍历的操作优化成取队头

求滑动窗口中的最小值

```cpp
std::deque<int> q; //存下标，才能判断队头合法性
for(int i = 0; i < n; i++) {
    //[i - k + 1, i]
    while(!q.empty() && q.front() < i - k + 1) {
        q.pop_front();
    }
    while(!q.empty() && a[q.back()] > a[i]) { //单调非减 最小值取队头即可
        q.pop_back();
    }
    q.push_back(i);
    if(i - k + 1 >= 0) {
        std::cout << a[q.front()] << ' ';
    }
}
```

## 贪心

贪心就是关注局部最优解

1. 猜测
2. 证明，常用反证法

区间问题常对左端点或者右端点排序

## 判断质数

质数：从 2 开始，只能被 1 和自己整除的数

一个合数的约数总是成对出现的，如果 i 能整除 x，则 x / i 也整除 x，因此只需判断较小的那个，枚举到 i <= x / i 即可

```cpp
bool isPrime(int x) {
    if(x < 2) {
        return false;
    }
    for(int i = 2; i <= x / i; i++) {
        if(x % i == 0) {
            return false;
        }
    }
    return true;
}
```

## 筛质数

求 1 ~ n 的质数

```cpp
for(int i = 2; i <= n; i++) {
    if(!st[i]) {
        primes.push_back(i);
    }
    for(int j = 2 * i; j <= n; j += i) {
        st[j] = true;
    }
}
```

## 最大公约数

```cpp
int gcd(int a, int b) {
    return b == 0 ? a : gcd(b, a % b);
}
```

## 最小公倍数

gcd(a, b) * lcm(a, b) == a * b

```cpp
int lcm(int a, int b) {
    return a / gcd(a, b) * b;
}
```

## 上取整除法

C++ 整数除法的结果是向零取整（大于 0 时下取整，小于 0 时上取整）

`a / b` 大于 0 时怎么实现上取整：`(a + b - 1) / b`

## FloodFill

FloodFill 算法可以在线性时间复杂度下，找到某个点所在的连通块

可以用 DFS 实现，也可以用 BFS 实现，时间复杂度相同，由于 DFS 递归过深可能栈溢出，所以优先用 BFS

常见题型：山峰山谷问题

- 连通块周围没有更高的点则为山峰，连通块周围没有更低的点则为山谷
- 扩展队列时，扩展点如果高度不同，更新 has_higher 和 has_lower，否则判重入队

## BFS最短路

当图的边权为 1 时，可以用 BFS 求最短路 

外层循环更新当前步数，内层循环遍历这层的点