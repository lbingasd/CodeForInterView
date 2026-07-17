#pragma once

#include <string>
#include <vector>

namespace BacktrackingTest {

void permutationsDfs(const std::vector<int>& nums, std::vector<bool>& used,
                     std::vector<int>& path, std::vector<std::vector<int>>& result)
{
    if (path.size() == nums.size()) { result.push_back(path); return; }
    for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
        if (used[i]) continue;
        used[i] = true; path.push_back(nums[i]);
        permutationsDfs(nums, used, path, result);
        path.pop_back(); used[i] = false;
    }
}

// 全排列：时间 O(N*N!)，空间 O(N)。
std::vector<std::vector<int>> permutations(const std::vector<int>& nums)
{
    std::vector<std::vector<int>> result, path;
    std::vector<int> current; std::vector<bool> used(nums.size());
    permutationsDfs(nums, used, current, result);
    return result;
}

void subsetsDfs(const std::vector<int>& nums, int start, std::vector<int>& path,
                std::vector<std::vector<int>>& result)
{
    result.push_back(path);
    for (int i = start; i < static_cast<int>(nums.size()); ++i) {
        path.push_back(nums[i]); subsetsDfs(nums, i + 1, path, result); path.pop_back();
    }
}

// 子集枚举：时间 O(N*2^N)。
std::vector<std::vector<int>> subsets(const std::vector<int>& nums)
{
    std::vector<std::vector<int>> result; std::vector<int> path;
    subsetsDfs(nums, 0, path, result); return result;
}

}
