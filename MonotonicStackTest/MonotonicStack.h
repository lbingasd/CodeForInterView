#pragma once

#include <stack>
#include <vector>

namespace MonotonicStackTest {

// 每日温度：寻找右侧第一个更大元素，时间 O(N)。
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

}
