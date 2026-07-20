#pragma once

#include <algorithm>
#include <stack>
#include <vector>

namespace MonotonicStackTest {

/**
 * 1. 每日温度 (LeetCode 739)
 * 难度：中等
 * 思路：单调递减栈，寻找右侧第一个更大元素。
 * 复杂度：时间 O(N)，空间 O(N)
 */
std::vector<int> dailyTemperatures(const std::vector<int>& temperatures)
{
    std::vector<int> result(temperatures.size()); std::stack<int> indexes;
    for (int i = 0; i < static_cast<int>(temperatures.size()); ++i) {
        while (!indexes.empty() && temperatures[indexes.top()] < temperatures[i]) {
            int old = indexes.top(); indexes.pop(); result[old] = i - old;
        }
        indexes.push(i);
    }
    return result;
}

/**
 * 2. 接雨水 (LeetCode 42) — 单调栈解法
 * 难度：困难
 * 思路：单调递减栈。当前高度大于栈顶时，栈顶形成凹槽可以接水。
 *       弹出栈顶作为底部，新的栈顶作为左边界，当前柱作为右边界。
 *       宽度 = i - left - 1，高度 = min(height[left], height[i]) - bottom。
 * 复杂度：时间 O(N)，空间 O(N)
 */
int trap(const std::vector<int>& height)
{
    // 单调递减栈，存放下标（柱子高度严格不增）。
    std::stack<int> st;
    int water = 0;

    for (int i = 0; i < static_cast<int>(height.size()); ++i) {
        // 当前柱子比栈顶高 → 栈顶形成凹槽，可以接水。
        while (!st.empty() && height[i] > height[st.top()]) {
            int bottom = height[st.top()];
            st.pop();
            // 没有左边界则无法接水。
            if (st.empty()) break;
            int left = st.top();
            // 宽度：从左边界到右边界（不含两端）。
            int width = i - left - 1;
            // 高度：左右边界中较矮的那个减去底部高度。
            int h = std::min(height[left], height[i]) - bottom;
            water += width * h;
        }
        st.push(i);
    }
    return water;
}

}
