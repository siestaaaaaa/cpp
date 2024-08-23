> 多打草稿，分析清楚再写代码

# 双指针

## [15. 三数之和](https://leetcode.cn/problems/3sum/)

```cpp
class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        vector<vector<int>> res;
        sort(nums.begin(), nums.end());
        int n = nums.size();
        for(int i = 0; i < n; i++) {
            int l = i + 1, r = n - 1;
            while(l < r) {
                int t = nums[i] + nums[l] + nums[r];
                if(t < 0) {
                    l++;
                } else if(t > 0) {
                    r--;
                } else {
                    res.push_back({nums[i], nums[l], nums[r]});
                    do l++; while(l < r && nums[l] == nums[l - 1]);
                    do r--; while(l < r && nums[r] == nums[r + 1]);
                }
            }
            while(i + 1 < n && nums[i] == nums[i + 1]) {
                i++;
            }
        }
        return res;
    }
};
```

## [5. 最长回文子串](https://leetcode.cn/problems/longest-palindromic-substring/)

```cpp
class Solution {
    auto expand(const string& s, int l, int r) {
        while(l - 1 >= 0 && r + 1 < s.size() && s[l - 1] == s[r + 1]) {
            l--;
            r++;
        }
        return make_tuple(l, r);
    }

public:
    string longestPalindrome(string s) {
        int l = 0, r = 0;
        for(int i = 0; i < s.size() - 1; i++) {
            if(auto [ll, rr] = expand(s, i, i); rr - ll > r - l) {
                l = ll;
                r = rr;
            }
            if(s[i] == s[i + 1]) {
                if(auto [ll, rr] = expand(s, i, i + 1); rr - ll > r - l) {
                    l = ll;
                    r = rr;
                }
            }
        }
        return s.substr(l, r - l + 1);
    }
};
```

# 滑动窗口

进窗口、判断更新、出窗口

## [3. 无重复字符的最长子串](https://leetcode.cn/problems/longest-substring-without-repeating-characters/)

```cpp
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        unordered_set<char> us;
        int n = s.size();
        int res = 0;
        for(int i = 0, j = 0; i < n; i++) {
            while(us.count(s[i])) {
                us.erase(s[j]);
                j++;
            }
            us.insert(s[i]);
            res = max(res, i - j + 1);
        }
        return res;
    }
};
```

# 链表

dummy、头插、尾插、快慢指针

## [141. 环形链表](https://leetcode.cn/problems/linked-list-cycle/)

慢指针一次走一步，快指针一次走两步，相对速度为 1

如果存在环，快指针会先进环，慢指针后进环，当慢指针进环时，设两者距离为 x，则经过 x 次后快慢指针会相遇

如果不存在环，快指针会先走到空

拓展：如果有环，求环的入口

```cpp
设起点到环入口的距离为 L，环入口到相遇点的距离为 X，环的长度为 C

相遇时：
慢指针走的路程 = L + X
快指针走的路程 = L + X + N*C, N>=1
又因为快指针的速度是慢指针的两倍
所以快指针的路程是慢指针的两倍
即 2*(L + X) = L + X + N*C
得 L = N*C - X
即 L = (N-1)*C + C - X
可知，两个指针一个从相遇点出发，另一个从起点出发，会在环入口相遇
```

## [234. 回文链表](https://leetcode.cn/problems/palindrome-linked-list/)

找中点 - 反转后半段 - 遍历对比

```cpp
class Solution {
    ListNode* reverseList(ListNode* head) {
        ListNode* prev = nullptr;
        ListNode* cur = head;
        while(cur) {
            auto next = cur->next;
            cur->next = prev;
            prev = cur;
            cur = next;
        }
        return prev;
    }

public:
    bool isPalindrome(ListNode* head) {
        auto fast = head, slow = head;
        while(fast->next && fast->next->next) {
            fast = fast->next->next;
            slow = slow->next;
        }
        auto l1 = head, l2 = slow->next;
        slow->next = nullptr;
        l2 = reverseList(l2);
        while(l1 && l2) {
            if(l1->val != l2->val) {
                return false;
            }
            l1 = l1->next;
            l2 = l2->next;
        }
        return true;
    }
};
```

## [445. 两数相加 II](https://leetcode.cn/problems/add-two-numbers-ii/)

类似高精度加法

```cpp
class Solution {
    ListNode* reverseList(ListNode* head) {
        ListNode* prev = nullptr;
        ListNode* cur = head;
        while(cur) {
            auto next = cur->next;
            cur->next = prev;
            prev = cur;
            cur = next;
        }
        return prev;
    }

public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        l1 = reverseList(l1);
        l2 = reverseList(l2);
        auto dummy = new ListNode;
        int t = 0;
        while(l1 || l2 || t) {
            if(l1) {
                t += l1->val;
                l1 = l1->next;
            }
            if(l2) {
                t += l2->val;
                l2 = l2->next;
            }
            auto nd = new ListNode(t % 10);
            t /= 10;
            nd->next = dummy->next;
            dummy->next = nd;
        }
        return dummy->next;
    }
};
```

## [25. K 个一组翻转链表](https://leetcode.cn/problems/reverse-nodes-in-k-group/)

```cpp
class Solution {
public:
    ListNode* reverseKGroup(ListNode* head, int k) {
        auto cur = head;
        int n = 0;
        while(cur) {
            cur = cur->next;
            n++;
        }
        n /= k; //n组

        auto dummy = new ListNode;
        auto prev = dummy;
        cur = head;
        while(n--) {
            auto tmp = cur;
            //以prev为dummy 头插
            for(int i = 0; i < k; i++) {
                auto next = cur->next;
                cur->next = prev->next;
                prev->next = cur;
                cur = next;
            }
            prev = tmp;
        }
        prev->next = cur;
        return dummy->next;
    }
};
```

# 二叉树

## [606. 根据二叉树创建字符串](https://leetcode.cn/problems/construct-string-from-binary-tree/)

```cpp
class Solution {
public:
    string tree2str(TreeNode* root) {
        string res;
        dfs(root, res);
        return res;
    }

    void dfs(TreeNode* root, string& s) {
        if(root == nullptr) {
            return;
        }

        s += to_string(root->val);

        if(root->left || root->right) {
            s += '(';
            dfs(root->left, s);
            s += ')';
        }

        if(root->right) {
            s += '(';
            dfs(root->right, s);
            s += ')';
        }
    }
};
```

## [236. 二叉树的最近公共祖先](https://leetcode.cn/problems/lowest-common-ancestor-of-a-binary-tree/)

```cpp
class Solution {
public:
    TreeNode* lowestCommonAncestor(TreeNode* cur, TreeNode* p, TreeNode* q) {
        if(cur == nullptr) {
            return nullptr;
        }

        if(cur == p || cur == q) {
            return cur;
        }

        auto l = lowestCommonAncestor(cur->left, p, q);
        auto r = lowestCommonAncestor(cur->right, p, q);
        if(l == nullptr) {
            return r;
        }
        if(r == nullptr) {
            return l;
        }
        return cur;
    }
};
```

## [102. 二叉树的层序遍历](https://leetcode.cn/problems/binary-tree-level-order-traversal/)

```cpp
class Solution {
public:
    vector<vector<int>> levelOrder(TreeNode* root) 
    {
        if(root == nullptr) {
            return {};
        }
        vector<vector<int>> res;
        queue<TreeNode*> q;
        q.push(root);
        while(!q.empty()) {
            auto sz = q.size();
            vector<int> v;
            while(sz--) {
                auto t = q.front();
                q.pop();
                v.push_back(t->val);
                if(t->left) q.push(t->left);
                if(t->right) q.push(t->right);
            }
            res.push_back(v);
        }
        return res;
    }
};
```

## [105. 从前序与中序遍历序列构造二叉树](https://leetcode.cn/problems/construct-binary-tree-from-preorder-and-inorder-traversal/)

```cpp
class Solution {
    unordered_map<int, int> inorder_idx;
    TreeNode* build(const vector<int>& preorder, int root, int left, int right) {
        if(left > right) {
            return {};
        }
        auto nd = new TreeNode{preorder[root]};
        int idx = inorder_idx[preorder[root]];
        int leftLength = idx - left;
        int rightLength = right - idx;
        nd->left = build(preorder, root + 1, left, idx - 1);
        nd->right = build(preorder, root + leftLength + 1, idx + 1, right);
        return nd;
    }

public:
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
        for(size_t i = 0; i < inorder.size(); i++) {
            inorder_idx[inorder[i]] = i;
        }
        return build(preorder, 0, 0, inorder.size() - 1);
    }
};
```

## [144. 二叉树的前序遍历](https://leetcode.cn/problems/binary-tree-preorder-traversal/)

根左右

```cpp
class Solution {
public:
    vector<int> preorderTraversal(TreeNode* root) {
        vector<int> res;
        stack<TreeNode*> stk;
        if(root == nullptr) {
            return {};
        }
        stk.push(root);
        while(!stk.empty()) {
            auto t = stk.top();
            stk.pop();
            res.push_back(t->val);
            if(t->right) {
                stk.push(t->right);
            }
            if(t->left) {
                stk.push(t->left);
            }
        }
        return res;
    }
};
```

## [145. 二叉树的后序遍历](https://leetcode.cn/problems/binary-tree-postorder-traversal/)

左右根 -> 根右左 + 逆置

```cpp
class Solution {
public:
    vector<int> postorderTraversal(TreeNode* root) {
        vector<int> res;
        stack<TreeNode*> stk;
        if(root == nullptr) {
            return {};
        }
        stk.push(root);
        while(!stk.empty()) {
            auto t = stk.top();
            stk.pop();
            res.push_back(t->val);
            if(t->left) {
                stk.push(t->left);
            }
            if(t->right) {
                stk.push(t->right);
            }
        }
        reverse(res.begin(), res.end());
        return res;
    }
};
```

## [94. 二叉树的中序遍历](https://leetcode.cn/problems/binary-tree-inorder-traversal/)

左根右

```cpp
class Solution {
public:
    vector<int> inorderTraversal(TreeNode* root) {
        vector<int> res;
        stack<TreeNode*> stk;
        if(root == nullptr) {
            return {};
        }
        auto cur = root;
        while(!stk.empty() || cur) {
            if(cur) {
                stk.push(cur);
                cur = cur->left;
            } else {
                auto t = stk.top();
                stk.pop();
                res.push_back(t->val);
                cur = t->right;
            }
        }
        return res;
    }
};
```

# 哈希表

## [1. 两数之和](https://leetcode.cn/problems/two-sum/)

可以用一个哈希表存元素和下标的映射，扫描整个数组，每扫描到一个元素 x，就在哈希表中查找是否存在 target - x，如果存在则返回其下标和当前扫描到的元素下标，否则就将当前扫描到的元素插入哈希表

```cpp
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        unordered_map<int, int> idx;
        int n = nums.size();
        for(int i = 0; i < n; i++) {
            int t = target - nums[i];
            if(idx.count(t)) {
                return {idx[t], i};
            }
            idx[nums[i]] = i;
        }
        return {};
    }
};
```

# 堆

## [295. 数据流的中位数](https://leetcode.cn/problems/find-median-from-data-stream/)

双堆法：大堆存小的那部分数，小堆存大的那部分数，维护 size 差值不超过 1。中位数由两个堆顶决定

## [347. 前 K 个高频元素](https://leetcode.cn/problems/top-k-frequent-elements/)

```cpp
class Solution {
    using T = pair<int, int>;
    struct cmp {
        bool operator()(const T& u, const T& v) const {
            return u.second > v.second;
        }
    };
    
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
        unordered_map<int, int> cnt;
        for(const auto e : nums) {
            cnt[e]++;
        }

        priority_queue<T, vector<T>, cmp> pq;
        for(const auto& [u, v] : cnt) {
            if(pq.size() < k) {
                pq.push({u, v});
            } else {
                if(v > pq.top().second) {
                    pq.pop();
                    pq.push({u, v});
                }
            }
        }

        vector<int> res;
        while(!pq.empty()) {
            res.push_back(pq.top().first);
            pq.pop();
        }
        return res;
    }
};
```

# Trie

## [208. 实现 Trie (前缀树)](https://leetcode.cn/problems/implement-trie-prefix-tree/)

```cpp
class Trie {
    struct Node {
        Node* child[26]{};
        bool isEnd{};
    };
    Node* root_ = new Node;

public:
    Trie() {}

    void insert(string word) {
        auto cur = root_;
        for(auto c : word) {
            auto t = c - 'a';
            if(cur->child[t] == nullptr) {
                cur->child[t] = new Node;
            }
            cur = cur->child[t];
        }
        cur->isEnd = true;
    }
    
    bool search(string word) {
        auto cur = root_;
        for(auto c : word) {
            auto t = c - 'a';
            if(cur->child[t] == nullptr) {
                return false;
            }
            cur = cur->child[t];
        }
        return cur->isEnd;
    }
    
    bool startsWith(string prefix) {
        auto cur = root_;
        for(auto c : prefix) {
            auto t = c - 'a';
            if(cur->child[t] == nullptr) {
                return false;
            }
            cur = cur->child[t];
        }
        return true;
    }
};
```

# DFS

画递归搜索树

## [46. 全排列](https://leetcode.cn/problems/permutations/)

```cpp
class Solution {
    vector<vector<int>> paths;
    vector<int> path;
    bool st[10]{};

    void dfs(vector<int>& nums, int u) {
        if(u == nums.size()) {
            paths.push_back(path);
            return;
        }

        for(int i = 0; i < nums.size(); i++) {
            if(!st[i]) {
                st[i] = true;
                path.push_back(nums[i]);
                dfs(nums, u + 1);
                st[i] = false;
                path.pop_back();
            }
        }
    }

public:
    vector<vector<int>> permute(vector<int>& nums) {
        dfs(nums, 0);
        return paths;
    }
};
```

# BFS

入队时标记，防止重复入队

## [994. 腐烂的橘子](https://leetcode.cn/problems/rotting-oranges/)

多源 BFS + FloodFill

```cpp
class Solution {
    int dx[4]{-1, 1, 0, 0};
    int dy[4]{0, 0, -1, 1};
public:
    int orangesRotting(vector<vector<int>>& grid) {
        int n = grid.size(), m = grid[0].size();
        queue<pair<int, int>> q;
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < m; j++) {
                if(grid[i][j] == 2) {
                    q.push({i, j});
                    grid[i][j] = 0;
                }
            }
        }
        int res = q.empty() ? 0 : -1;
        while(!q.empty()) {
            int sz = q.size();
            while(sz--) {
                auto [x, y] = q.front();
                q.pop();
                for(int i = 0; i < 4; i++) {
                    int nx = x + dx[i], ny = y + dy[i];
                    if(nx >= 0 && nx < n && ny >= 0 && ny < m && grid[nx][ny]) {
                        q.push({nx, ny});
                        grid[nx][ny] = 0;
                    }
                }
            }
            res++;
        }
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < m; j++) {
                if(grid[i][j] == 1) {
                    return -1;
                }
            }
        }
        return res;
    }
};
```

# 拓扑排序

拓扑序列就是可行的执行顺序，一个结点的入度是 $0$ 时才能执行

## [207. 课程表](https://leetcode.cn/problems/course-schedule/)

```cpp
class Solution {
public:
    bool canFinish(int numCourses, vector<vector<int>>& prerequisites) {
        vector<int> in(numCourses);
        vector<vector<int>> g(numCourses);
        for(const auto& e: prerequisites) {
            in[e[0]]++;
            g[e[1]].push_back(e[0]);
        }
        queue<int> q;
        for(size_t i = 0; i < numCourses; i++) {
            if(in[i] == 0) {
                q.push(i);
            }
        }
        int cnt = 0;
        while(!q.empty()) {
            auto a = q.front();
            q.pop();
            cnt++;
            for(const auto& b : g[a]) {
                in[b]--;
                if(in[b] == 0) {
                    q.push(b);
                }
            }
        }
        return cnt == numCourses;
    }
};
```

# 动态规划

## [53. 最大子数组和](https://leetcode.cn/problems/maximum-subarray/)

```cpp
class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        //f[i]: 以i结尾的最大子数组的和
        int n = nums.size();
        vector<int> f(n);
        f[0] = nums[0];
        for(int i = 1; i < n; i++) {
            f[i] = max(f[i - 1] + nums[i], nums[i]);
        }
        int res = -1e4;
        for(int i = 0; i < n; i++) {
            res = max(res, f[i]);
        }
        return res;
    }
};
```

## [1143. 最长公共子序列](https://leetcode.cn/problems/longest-common-subsequence/)

```cpp
class Solution {
public:
    int longestCommonSubsequence(string text1, string text2) {
        int n = text1.size(), m = text2.size();
        vector<vector<int>> f(n + 1, vector<int>(m + 1));
        //f[i][j]: LCE(text1[0,i-1], text2[0,j-1])
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < m; j++) {
                if(text1[i] == text2[j]) {
                    f[i + 1][j + 1] = f[i][j] + 1;
                } else {
                    f[i + 1][j + 1] = max(f[i + 1][j], f[i][j + 1]);
                }
            }
        }
        return f[n][m];
    }
};
```

## [72. 编辑距离](https://leetcode.cn/problems/edit-distance/)

```cpp
class Solution {
public:
    int minDistance(string word1, string word2) {
        int n = word1.size(), m = word2.size();
        vector<vector<int>> f(n + 1, vector<int>(m + 1));
        //f[i][j]: minDistance(word1[0,i-1] -> word2[0,j-1])
        for(int i = 0; i <= n; i++) {
            f[i][0] = i;
        }
        for(int i = 0; i <= m; i++) {
            f[0][i] = i;
        }
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < m; j++) {
                if(word1[i] == word2[j]) {
                    f[i + 1][j + 1] = f[i][j];
                } else {
                    //insert delete replace
                    f[i + 1][j + 1] = min(min(f[i + 1][j], f[i][j + 1]), f[i][j]) + 1; 
                }
            }
        }
        return f[n][m];
    }
};
```

# 前缀和

## [560. 和为 K 的子数组](https://leetcode.cn/problems/subarray-sum-equals-k/)

`s[i]-s[j]==k` 等价于 `s[j]==s[i]-k`，问题等价于对于每个 i，枚举 j 从 0 到 i-1，找到 `s[j]==s[i]-k` 的个数

```cpp
class Solution {
public:
    int subarraySum(vector<int>& nums, int k) {
        unordered_map<int, int> cnt; //存s的次数
        cnt[0] = 1;
        int s = 0, ans = 0;
        for(auto e : nums) {
            s += e;
            if(cnt.count(s - k)) {
                ans += cnt[s - k];
            }
            cnt[s]++;
        }
        return ans;
    }
};
```

## [974. 和可被 K 整除的子数组](https://leetcode.cn/problems/subarray-sums-divisible-by-k/)

`(s[i]-s[j])%k==0` 等价于 `s[i]%k==s[j]%k`，问题等价于对于每个 i，枚举 j 从 0 到 i-1，找到 `s[i]%k==s[j]%k` 的个数

```cpp
class Solution {
public:
    int subarraysDivByK(vector<int>& nums, int k) {
        unordered_map<int, int> cnt; //存前缀和%k的次数
        cnt[0] = 1;
        int s = 0, ans = 0;
        for(auto e : nums) {
            s = ((s + e) % k + k) % k;
            if(cnt.count(s)) {
                ans += cnt[s];
            }
            cnt[s]++;
        }
        return ans;
    }
};
```

## [42. 接雨水](https://leetcode.cn/problems/trapping-rain-water/)

对于每个单位，能接雨水的量 = min(lmax, rmax) - height

```cpp
class Solution {
public:
    int trap(vector<int>& height) {
        int n = height.size();
        //lmax[i]=max(h[0]~h[i-1])
        //rmax[i]=max(h[i+1]~h[n-1])
        //lmax[0] = rmax[n-1] = 0
        vector<int> lmax(n), rmax(n);
        int x = height[0];
        for(int i = 1; i < n; i++) {
            lmax[i] = x;
            x = max(x, height[i]);
        }
        x = height[n-1];
        for(int i = n - 2; i >= 0; i--) {
            rmax[i] = x;
            x = max(x, height[i]);
        }
        int res = 0;
        for(int i = 0; i < n; i++) {
            int t = min(lmax[i], rmax[i]);
            if(t <= height[i]) {
                continue;
            }
            res += t - height[i];
        }
        return res;
    }
};
```

# 快速选择

## [215. 数组中的第K个最大元素](https://leetcode.cn/problems/kth-largest-element-in-an-array/)

```cpp
class Solution {
    int quickSelect(vector<int>& a, int l, int r, int k) {
        if(l == r) {
            return a[l];
        }

        int i = l - 1, j = r + 1, x = a[(l + r) / 2];
        while(i < j) {
            while(a[++i] > x);
            while(a[--j] < x);
            if(i < j) {
                swap(a[i], a[j]);
            }
        }
        int lCnt = j - l + 1;
        if(lCnt >= k) {
            return quickSelect(a, l, j, k);
        } else {
            return quickSelect(a, j + 1, r, k - lCnt);
        }
    }
    
public:
    int findKthLargest(vector<int>& nums, int k) {
        return quickSelect(nums, 0, nums.size() - 1, k);
    }
};
```

# 贪心

## [55. 跳跃游戏](https://leetcode.cn/problems/jump-game/)

维护最右能到达的下标

```cpp
class Solution {
public:
    bool canJump(vector<int>& nums) {
        int ma = 0;
        for(int i = 0; i < nums.size(); i++) {
            if(ma < i) {
                return false;
            }
            ma = max(ma, i + nums[i]);
        }
        return true;
    }
};
```

## [45. 跳跃游戏 II](https://leetcode.cn/problems/jump-game-ii/)

维护当前窗口和下个窗口的右边界

```cpp
class Solution {
public:
    int jump(vector<int>& nums) {
        int curR = 0, nextR = 0;
        int res = 0;
        for(int i = 0; i < nums.size() - 1; i++) {
            nextR = max(nextR, i + nums[i]);
            if(i == curR) {
                curR = nextR;
                res++;
            }
        }
        return res;
    }
};
```

## [121. 买卖股票的最佳时机](https://leetcode.cn/problems/best-time-to-buy-and-sell-stock/)

只能买一次，枚举要卖的那天，维护这天之前的最低买入价格

```cpp
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int mi = prices[0];
        int res = 0;
        for(int i = 1; i < prices.size(); i++) {
            res = max(res, prices[i] - mi);
            mi = min(mi, prices[i]);
        }
        return res;
    }
};
```

## [122. 买卖股票的最佳时机 II](https://leetcode.cn/problems/best-time-to-buy-and-sell-stock-ii/)

可以买无数次，把所有利润拿到手就行

```cpp
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int res = 0;
        for(int i = 1; i < prices.size(); i++) {
            int t = prices[i] - prices[i - 1];
            if(t > 0) {
                res += t;
            }
        }
        return res;
    }
};
```

# 位运算

## [371. 两整数之和](https://leetcode.cn/problems/sum-of-two-integers/)

```cpp
class Solution {
public:
    //无进位加法：a ^ b
    //进位：(a & b) << 1
    int getSum(int a, int b) {
        while(b) {
            size_t c = (size_t)(a & b) << 1;
            a = a ^ b;
            b = c;
        }
        return a;
    }
};
```

## [137. 只出现一次的数字 II](https://leetcode.cn/problems/single-number-ii/)

先不考虑只出现一次的那个数，其他数都出现了三次，因此对于某一位是 0 和是 1 的个数都是 3 的倍数，所以将所有数的某一位加起来模 3 就是只出现一次的那个数在此位的值

```cpp
class Solution {
public:
    int singleNumber(vector<int>& nums) {
        int res = 0;
        for(int i = 0; i < 32; i++) {
            int s = 0;
            for(auto& e : nums) {
                s += e >> i & 1;
            }
        	if(s % 3) {
                res |= 1 << i;
            }
        }
        return res;
    }
};
```

# 模拟

## [1419. 数青蛙](https://leetcode.cn/problems/minimum-number-of-frogs-croaking/)

```cpp
class Solution {
    int cnt[128]{}; //cnt[index]: 正在叫 index 的青蛙数量

public:
    int minNumberOfFrogs(string croakOfFrogs) {
        for(const auto c : croakOfFrogs) {
            cnt[c]++;
            if(c == 'c') {
                if(cnt['k'] > 0) {
                    cnt['k']--;
                }
            } else if(c == 'r') {
                if(cnt['c'] > 0) {
                    cnt['c']--;
                } else { //不能从 r 开始叫
                    return -1;
                }
            } else if(c == 'o') {
                if(cnt['r'] > 0) {
                    cnt['r']--;
                } else { //不能从 o 开始叫
                    return -1;
                }
            } else if(c == 'a') {
                if(cnt['o'] > 0) {
                    cnt['o']--;
                } else { //不能从 a 开始叫
                    return -1;
                }
            } else if(c == 'k') {
                if(cnt['a'] > 0) {
                    cnt['a']--;
                } else { //不能从 k 开始叫
                    return -1;
                }
            }
        }
        //最后都必须在叫 k
        if(cnt['c'] || cnt['r'] || cnt['o'] || cnt['a']) {
            return -1;
        }
        return cnt['k'];
    }
};
```

## [8. 字符串转换整数 (atoi)](https://leetcode.cn/problems/string-to-integer-atoi/)

1. 空格：读入字符串并丢弃前导空格
2. 符号：检查下一个字符（假设还未到字符末尾）为 `'-'` 还是 `'+'`。如果两者都不存在，则假定结果为正
3. 转换：跳过前置零，读取该整数，直到遇到非数字字符或到达字符串结尾。如果没有读取数字，则结果为 0
4. 舍入：如果整数数超过 32 位有符号整数范围，需要截断这个整数，使其保持在这个范围内

```cpp
class Solution {
public:
    int myAtoi(string s) {
        int idx = 0;
        while(idx < s.size() && s[idx] == ' ') {
            idx++;
        }

        int flag = 0;
        if(s[idx] == '+') {
            flag = 1;
            idx++;
        } else if(s[idx] == '-') {
            flag = -1;
            idx++;
        } else {
            flag = 1;
        }

        int64_t res = 0;
        while(idx < s.size() && s[idx] == '0') {
            idx++;
        }
        while(idx < s.size() && isdigit(s[idx])) {
            if(res > INT_MAX || res < INT_MIN) {
                break;
            }
            res = res * 10 + s[idx] - '0';
            idx++;
        }
        res *= flag;
        if(res < INT_MIN) {
            res = INT_MIN;
        }
        if(res > INT_MAX) {
            res = INT_MAX;
        }
        return res;
    }
};
```

## [54. 螺旋矩阵](https://leetcode.cn/problems/spiral-matrix/)

```cpp
class Solution {
    bool st[20][20]{};
    int dx[4]{0, 1, 0, -1};
    int dy[4]{1, 0, -1, 0};

public:
    vector<int> spiralOrder(vector<vector<int>>& matrix) {
        int n = matrix.size(), m = matrix[0].size();
        vector<int> res;
        int x = 0, y = 0;
        int d = 0;
        for(int i = 0; i < m * n; i++) {
            res.push_back(matrix[x][y]);
            st[x][y] = true;
            int nx = x + dx[d], ny = y + dy[d];
            if(nx < 0 || nx >= n || ny < 0 || ny >= m || st[nx][ny]) {
                d = (d + 1) % 4;
                nx = x + dx[d], ny = y + dy[d];
            }
            x = nx, y = ny;
        }
        return res;
    }
};
```

## [149. 直线上最多的点数](https://leetcode.cn/problems/max-points-on-a-line/)

```cpp
class Solution {
public:
    int maxPoints(vector<vector<int>>& points) {
        int n = points.size();
        int res = 1;
        for(int i = 0; i < n; i++) {
            auto& a_point = points[i];
            for(int j = i + 1; j < n; j++) {
                auto& b_point = points[j];
                // a---b line
                int cnt = 2;
                for(int k = j + 1; k < n; k++) {
                    auto& c_point = points[k];
                    // (b[1]-a[1])/(b[0]-a[0]) == (b[1]-c[1])/(b[0]-c[0])
                    int p = (b_point[1]-a_point[1])*(b_point[0]-c_point[0]);
                    int q = (b_point[1]-c_point[1])*(b_point[0]-a_point[0]);
                    if(p == q) {
                        cnt++;
                    }
                }
                res = max(res, cnt);
            }
        }
        return res;
    }
};
```

# 数学

## [470. 用 Rand7() 实现 Rand10()](https://leetcode.cn/problems/implement-rand10-using-rand7/)

```cpp
// The rand7() API is already defined for you.
// int rand7();
// @return a random integer in the range 1 to 7

class Solution {
public:
    int rand10() {
        //random: 1-7
        //-1
        //random: 0-6
        //*7
        //random: 0 7 14 ... 42
        //+rand7()
        //random: 1-7 8-14 ... 43-49
        //random: 1-49
        //只取<=40的
        //random: 1-40
        //%10
        //random: 0-9（每个数有四个来源）
        //+1
        //random: 1-10
        int res = 0;
        while((res = (rand7() - 1) * 7 + rand7()) > 40) {}
        return res % 10 + 1;
    }
};
```

## [172. 阶乘后的零](https://leetcode.cn/problems/factorial-trailing-zeroes/)

```cpp
class Solution {
public:
    int trailingZeroes(int n) {
        //n!的质因子中 2和5的个数取小
        //可证2的个数一定>=5的个数
        int res = 0;
        for(int i = 1; i <= n; i++) {
            //分解质因子 - 唯一分解定理
            int x = i;
            while(x % 5 == 0) {
                res++;
                x /= 5;
            }
        }
        return res;
    }
};
```

# 矩阵

## [73. 矩阵置零](https://leetcode.cn/problems/set-matrix-zeroes/)

```cpp
class Solution {
public:
    void setZeroes(vector<vector<int>>& matrix) {
        int n = matrix.size();
        int m = matrix[0].size();
        vector<bool> row(n), col(m); //需要被标记为0的行和列
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (!matrix[i][j]) {
                    row[i] = col[j] = true;
                }
            }
        }
        //遍历矩阵每个元素
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (row[i] || col[j]) { //如果该行需要被标记为0或该列需要被标记为0
                    matrix[i][j] = 0;
                }
            }
        }
    }
};
```

## [48. 旋转图像](https://leetcode.cn/problems/rotate-image/)

```cpp
class Solution {
public:
    void rotate(vector<vector<int>>& matrix) {
        //水平翻转
        for(auto& row : matrix) {
            reverse(row.begin(), row.end());
        }
        //以副对角线为轴翻转
        int n = matrix.size();
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < n - i; j++) {
                swap(matrix[i][j], matrix[n - 1 - j][n - 1 - i]);
            }
        }
    }
};
```