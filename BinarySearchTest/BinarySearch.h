#pragma once

#include <algorithm>
#include <vector>

namespace BinarySearchTest {

// 有序数组中查找目标值，找不到返回 -1。时间 O(log N)。
int search(const std::vector<int>& nums, int target)
{
    int left = 0, right = static_cast<int>(nums.size()) - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (nums[mid] == target) return mid;
        if (nums[mid] < target) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}

// 查找第一个大于等于 target 的位置（lower_bound）。
int lowerBound(const std::vector<int>& nums, int target)
{
    int left = 0, right = static_cast<int>(nums.size());
    while (left < right) {
        int mid = left + (right - left) / 2;
        if (nums[mid] < target) left = mid + 1;
        else right = mid;
    }
    return left;
}

// 旋转有序数组中查找目标值，时间 O(log N)。
int searchRotated(const std::vector<int>& nums, int target)
{
    int left = 0, right = static_cast<int>(nums.size()) - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (nums[mid] == target) return mid;
        if (nums[left] <= nums[mid]) {
            if (nums[left] <= target && target < nums[mid]) right = mid - 1;
            else left = mid + 1;
        } else {
            if (nums[mid] < target && target <= nums[right]) left = mid + 1;
            else right = mid - 1;
        }
    }
    return -1;
}

}
