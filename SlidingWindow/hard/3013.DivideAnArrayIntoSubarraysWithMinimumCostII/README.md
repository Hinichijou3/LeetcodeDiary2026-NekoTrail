# 滑动窗口与multiset
喵~🎶
> 3013.将数组分成最小总代价的子数组II，[灵神的题解](https://leetcode.cn/problems/divide-an-array-into-subarrays-with-minimum-cost-ii/solutions/2614067/liang-ge-you-xu-ji-he-wei-hu-qian-k-1-xi-zdzx/?envType=daily-question&envId=2026-02-03 "灵神真神了！")展现了非常精巧的的迭代器用法，尝试把它看明白可以考验你对迭代器的理解程度哦！

## 核心知识点（先考考你）

- **迭代器指向哪里？**
  - `begin()` → 容器第一个元素
  - `end()` → 容器最后一个元素的**下一个位置**（容器外）
  - `rbegin()` → 容器最后一个元素
  - `rend()` → 容器第一个元素的**前一个位置**

- **哪些可以解引用？**
  - `*begin()` ✅
  - `*end()` ❌（会报错！因为指向容器外）
  - `*rbegin()` ✅
  - `*rend()` ❌

灵活运用这些迭代器可以减少很多麻烦，这篇题解就是一个精妙的例子！

---

## 题目理解（简化版）

原题描述很绕，窝窝给出一个简化版：

在数组 `nums` 中找 k 个下标：`i₀, i₁, i₂, ... iₖ₋₁`，其中 `i₀ = nums[0]`。

要求：`iₖ₋₁ - i₁ ≤ dist`，并且让这些下标对应的值之和最小，求这个最小值。

---

### 申请中译中

其实题意就是“第二个下标到最后一个下标的间距不超过 dist”，所以可以看作：

**在一个大小为 dist+1 的滑动窗口里，找前 k-1 小的元素和**（因为 i₀ 已经固定啦）。

可想而知，窗口的初始位置是`nums[1]` 到 `nums[dist+1]`。

---

## 重要的数据结构：multiset

`multiset` 和 `set` 一样会自动排序，但**允许重复元素**，非常适合这题，以及类似的滑动窗口题说。

---

## 解题思路

### 1. 准备工作
第一个下标 i₀ 已经确定，我们只需要再找 k-1 个下标，所以先 `k--`。

我们准备：
- **两个 multiset**：`L` 和 `R`
  - `L` 存窗口里最小的 k 个数
  - `R` 存窗口里其他的数，L和R加起来就是整个滑动窗口
- **一个整形 sum**：存 `L` 中所有元素的和（它是我们要的答案候选）

### 2. 搬运工函数
因为经常要在 `L` 和 `R` 之间移动元素，先写好两个工具函数：
- **L2R**：把 `L` 中最大的数移到 `R`，并从 `sum` 中减去
- **R2L**：把 `R` 中最小的数移到 `L`，并在 `sum` 中加上

### 3. 初始化
```cpp
long long sum = reduce(nums.begin(), nums.begin() + dist + 2, 0LL);
multiset<int> L(nums.begin() + 1, nums.begin() + dist + 2), R;
while (L.size() > k) {
    L2R();
}
```
注意：
- `reduce` 和 `multiset` 的区间都是 **[begin, end)** 左闭右开！
- `sum` 从 `nums[0]` 开始，`L` 从 `nums[1]` 开始，因为窗口从第二个元素开始滑动，而最终答案需包含第一个元素
- 注意对齐类型用的`0LL`，它把`reduce`函数返回的类型也修改为`long long`

### 4. 滑动窗口
从 `dist+2` 到 `n-1` 遍历，每次处理一个“进入窗口”的元素。

#### 出窗口的元素
```cpp
int out = nums[i - dist - 1];
auto it = L.find(out);
if (it != L.end()) {
    sum -= out;
    L.erase(it);
} else {
    R.erase(R.find(out));
}
```
为什么先存 `out` 的值而不是直接操作迭代器？因为删元素时还不知道它在 `L` 还是 `R` 里，用值来找更方便。
一个小细节：为什么要写作`R.erase(R.find(out));`而不是`R.erase(out);`？因为`erase`函数接受的是迭代器，我们用一个`find`函数对齐类型！

#### 入窗口的元素
```cpp
int in = nums[i];
if (in < *L.rbegin()) {
    sum += in;
    L.insert(in);
} else {
    R.insert(in);
}
```
比较 `in` 和 `L` 里最大的数（`*L.rbegin()`），决定放哪边。

#### 维护 L 的大小
进出元素后，`L` 的大小可能不对了，用搬运工函数调整：
```cpp
if (L.size() == k - 1) {
    R2L();
} else if (L.size() == k + 1) {
    L2R();
}
```

### 5. 更新答案
每次滑动完，更新最小值：
```cpp
ans = min(ans, sum);
```

---

## 完整代码（复制灵神）

```cpp
class Solution {
public:
    long long minimumCost(vector<int>& nums, int k, int dist) {
        k--;
        long long sum = reduce(nums.begin(), nums.begin() + dist + 2, 0LL);
        multiset<int> L(nums.begin() + 1, nums.begin() + dist + 2), R;
        
        // 搬运工函数（lambda 表达式）
        auto L2R = [&]() {
            int x = *L.rbegin();  // L 中最大的数
            sum -= x;
            L.erase(L.find(x));
            R.insert(x);
        };
        
        auto R2L = [&]() {
            int x = *R.begin();   // R 中最小的数
            sum += x;
            R.erase(R.find(x));
            L.insert(x);
        };
        
        // 初始化 L 的大小为 k
        while (L.size() > k) {
            L2R();
        }
        
        long long ans = sum;
        for (int i = dist + 2; i < nums.size(); i++) {
            // 移除 out
            int out = nums[i - dist - 1];
            auto it = L.find(out);
            if (it != L.end()) {
                sum -= out;
                L.erase(it);
            } else {
                R.erase(R.find(out));
            }
            
            // 添加 in
            int in = nums[i];
            if (in < *L.rbegin()) {
                sum += in;
                L.insert(in);
            } else {
                R.insert(in);
            }
            
            // 维护 L 的大小为 k
            if (L.size() == k - 1) {
                R2L();
            } else if (L.size() == k + 1) {
                L2R();
            }
            
            ans = min(ans, sum);
        }
        return ans;
    }
};
```

---

## 关于 lambda 表达式的小贴士

原题解用了 lambda，窝窝觉得值得提一下！

lambda函数声明格式：
```cpp
auto 函数名 = [捕获列表](参数列表) -> 返回类型 { 函数体 };
```

- **捕获列表**：
  - `[&]`：引用捕获，可以直接修改外部变量
  - `[=]`：值捕获，修改不影响外部
  - 也可以混搭：`[&a, b]`，其中值引用的等号省略了
- **参数列表**：和普通函数一样
- **返回类型**：可省略，让编译器推断

窝窝第一次写的时候忘记加分号了，大家注意 lambda 末尾花括号后一定要要加 `;` ！

---

## 总结

这道题巧妙利用了：
1. **multiset 自动排序** + **允许重复**
2. **迭代器灵活操作**（尤其是 `rbegin()` 和 `begin()`）
3. **滑动窗口思想**，维护两个集合来快速获取前 k 小

理解了这些，以后遇到类似“滑动窗口 + 前k小”的问题，就可以轻松搞定了喵！