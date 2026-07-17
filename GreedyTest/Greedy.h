#pragma once

#include <algorithm>
#include <vector>

namespace GreedyTest {

// 跳跃游戏：维护当前可到达的最远位置，时间 O(N)。
bool canJump(const std::vector<int>& nums)
{
    int farthest = 0;
    for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
        if (i > farthest) return false;
        farthest = std::max(farthest, i + nums[i]);
    }
    return true;
}

}
