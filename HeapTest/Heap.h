#pragma once

#include <functional>
#include <queue>
#include <vector>

namespace HeapTest {

// 返回第 k 大元素；平均意义上适合用优先队列维护大小为 k 的最小堆。
int kthLargest(const std::vector<int>& nums, int k)
{
    if (k <= 0 || k > static_cast<int>(nums.size())) return 0;
    std::priority_queue<int, std::vector<int>, std::greater<int>> heap;
    for (int value : nums) 
    { 
        heap.push(value); 
        if (heap.size() > static_cast<size_t>(k)) 
        {
            heap.pop();
        }
    }
    return heap.top();
}

}
