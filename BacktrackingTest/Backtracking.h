#pragma once

#include <algorithm>
#include <cmath>
#include <numeric>
#include <string>
#include <vector>

namespace BacktrackingTest {

// ============================================================================
// 一、全排列系列 —— 生成所有可能的排列顺序
// ============================================================================

/**
 * 1. 全排列 (LeetCode 46)
 * 难度：中等
 * 思路：回溯 + used 数组标记已选元素，每次从头遍历选未使用的数字。
 * 复杂度：时间 O(N * N!)，空间 O(N)
 */
void permuteDfs(const std::vector<int>& nums, std::vector<bool>& used,
                std::vector<int>& path, std::vector<std::vector<int>>& result)
{
    // 路径长度等于数组长度时，得到一个完整排列。
    if (path.size() == nums.size()) { result.push_back(path); return; }
    for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
        if (used[i]) continue;
        used[i] = true;
        path.push_back(nums[i]);
        permuteDfs(nums, used, path, result);
        // 撤销选择，恢复状态以尝试下一个候选。
        path.pop_back();
        used[i] = false;
    }
}

std::vector<std::vector<int>> permute(const std::vector<int>& nums)
{
    std::vector<std::vector<int>> result;
    std::vector<int> path;
    std::vector<bool> used(nums.size(), false);
    permuteDfs(nums, used, path, result);
    return result;
}

/**
 * 2. 全排列 II (LeetCode 47)
 * 难度：中等
 * 思路：先排序让相同元素相邻，然后用 used[i-1] 做同层剪枝：
 *       当 nums[i] == nums[i-1] 且 !used[i-1] 时，说明同一层已经选过该值，跳过。
 * 复杂度：时间 O(N * N!)，空间 O(N)
 */
void permuteUniqueDfs(const std::vector<int>& nums, std::vector<bool>& used,
                      std::vector<int>& path, std::vector<std::vector<int>>& result)
{
    if (path.size() == nums.size()) { result.push_back(path); return; }
    for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
        if (used[i]) continue;
        // 同层去重：当前值与前一个相同，且前一个在本层未被使用 → 必然重复。
        if (i > 0 && nums[i] == nums[i - 1] && !used[i - 1]) continue;
        used[i] = true;
        path.push_back(nums[i]);
        permuteUniqueDfs(nums, used, path, result);
        path.pop_back();
        used[i] = false;
    }
}

std::vector<std::vector<int>> permuteUnique(std::vector<int> nums)
{
    // 排序保证相同元素相邻，是同层剪枝的前提。
    std::sort(nums.begin(), nums.end());
    std::vector<std::vector<int>> result;
    std::vector<int> path;
    std::vector<bool> used(nums.size(), false);
    permuteUniqueDfs(nums, used, path, result);
    return result;
}

/**
 * 3. 下一个排列 (LeetCode 31)
 * 难度：中等
 * 思路：1) 从右向左找第一个升序对 (nums[i] < nums[i+1])，i 即为交换位置；
 *       2) 从右向左找第一个大于 nums[i] 的数 nums[j]，交换 i 和 j；
 *       3) 反转 i+1 到末尾，使其变为升序（即最小字典序）。
 * 复杂度：时间 O(N)，空间 O(1)
 */
void nextPermutation(std::vector<int>& nums)
{
    int n = static_cast<int>(nums.size());
    // 第一步：从右找第一个非降序位置。
    int i = n - 2;
    while (i >= 0 && nums[i] >= nums[i + 1]) --i;

    if (i >= 0) {
        // 第二步：从右找第一个比 nums[i] 大的数，交换。
        int j = n - 1;
        while (j >= 0 && nums[j] <= nums[i]) --j;
        std::swap(nums[i], nums[j]);
    }
    // 第三步：反转 i+1 到末尾。i < 0 时整个数组反转，回到最小排列。
    std::reverse(nums.begin() + i + 1, nums.end());
}

/**
 * 4. 排列序列 (LeetCode 60)
 * 难度：困难
 * 思路：阶乘数制。n 个数的排列共 n! 种，首位由 k/(n-1)! 决定，
 *       逐位确定后从候选集中移除已用数字，k 更新为余数。
 * 复杂度：时间 O(N^2)，空间 O(N)
 */
std::string getPermutation(int n, int k)
{
    // 预计算阶乘，fact[i] = i!
    std::vector<int> fact(n + 1, 1);
    for (int i = 1; i <= n; ++i) fact[i] = fact[i - 1] * i;

    // 候选数字列表 1..n
    std::vector<int> nums(n);
    std::iota(nums.begin(), nums.end(), 1);

    // 转换为 0-based 索引
    --k;
    std::string result;
    for (int i = n; i >= 1; --i) {
        // 当前位在候选列表中的索引 = k / (i-1)!
        int idx = k / fact[i - 1];
        result += std::to_string(nums[idx]);
        // 从候选列表移除已用数字。
        nums.erase(nums.begin() + idx);
        // k 更新为组内偏移。
        k %= fact[i - 1];
    }
    return result;
}

// ============================================================================
// 二、组合系列 —— 从 n 个元素中选 k 个，顺序无关
// ============================================================================

/**
 * 5. 组合 (LeetCode 77)
 * 难度：中等
 * 思路：回溯 + start 参数控制"只往后选"，天然避免重复组合。
 *       剪枝：剩余元素不足以凑齐 k 个时提前返回。
 * 复杂度：时间 O(C(n,k) * k)，空间 O(k)
 */
void combineDfs(int n, int k, int start, std::vector<int>& path,
                std::vector<std::vector<int>>& result)
{
    if (static_cast<int>(path.size()) == k) { result.push_back(path); return; }
    // 剪枝：剩余可选数 = n - i + 1，还需选 = k - path.size()
    for (int i = start; i <= n - (k - static_cast<int>(path.size())) + 1; ++i) {
        path.push_back(i);
        combineDfs(n, k, i + 1, path, result);
        path.pop_back();
    }
}

std::vector<std::vector<int>> combine(int n, int k)
{
    std::vector<std::vector<int>> result;
    std::vector<int> path;
    combineDfs(n, k, 1, path, result);
    return result;
}

/**
 * 6. 组合总和 (LeetCode 39)
 * 难度：中等
 * 思路：回溯 + start 参数，每个数可重复选取 → 递归时 start 传 i（不跳过自身）。
 * 复杂度：时间 O(N^(T/M))，T=target，M=最小候选值；空间 O(T/M)
 */
void combinationSumDfs(const std::vector<int>& candidates, int target, int start,
                       std::vector<int>& path, std::vector<std::vector<int>>& result)
{
    if (target == 0) { result.push_back(path); return; }
    if (target < 0) return;
    for (int i = start; i < static_cast<int>(candidates.size()); ++i) {
        path.push_back(candidates[i]);
        // 传 i 而非 i+1，因为同一数字可以重复使用。
        combinationSumDfs(candidates, target - candidates[i], i, path, result);
        path.pop_back();
    }
}

std::vector<std::vector<int>> combinationSum(const std::vector<int>& candidates, int target)
{
    std::vector<std::vector<int>> result;
    std::vector<int> path;
    combinationSumDfs(candidates, target, 0, path, result);
    return result;
}

/**
 * 7. 组合总和 II (LeetCode 40)
 * 难度：中等
 * 思路：排序 + 同层去重。每个数字只能用一次，且结果不能重复。
 *       nums[i] == nums[i-1] 且 i > start 时跳过，保证同层不选相同值。
 * 复杂度：时间 O(2^N)，空间 O(N)
 */
void combinationSum2Dfs(const std::vector<int>& candidates, int target, int start,
                        std::vector<int>& path, std::vector<std::vector<int>>& result)
{
    if (target == 0) { result.push_back(path); return; }
    for (int i = start; i < static_cast<int>(candidates.size()); ++i) {
        // 同层去重：i > start 意味着 candidates[i-1] 在本层已经尝试过了。
        if (i > start && candidates[i] == candidates[i - 1]) continue;
        if (candidates[i] > target) break;  // 剪枝：后续更大，不可能凑出 target
        path.push_back(candidates[i]);
        // 传 i+1，每个数字只能用一次。
        combinationSum2Dfs(candidates, target - candidates[i], i + 1, path, result);
        path.pop_back();
    }
}

std::vector<std::vector<int>> combinationSum2(std::vector<int> candidates, int target)
{
    std::sort(candidates.begin(), candidates.end());
    std::vector<std::vector<int>> result;
    std::vector<int> path;
    combinationSum2Dfs(candidates, target, 0, path, result);
    return result;
}

// ============================================================================
// 三、子集系列 —— 枚举所有可能的子集
// ============================================================================

/**
 * 8. 子集 (LeetCode 78)
 * 难度：中等
 * 思路：回溯，每个节点都加入结果（不像排列等到叶子才加）。
 * 复杂度：时间 O(N * 2^N)，空间 O(N)
 */
void subsetsDfs(const std::vector<int>& nums, int start, std::vector<int>& path,
                std::vector<std::vector<int>>& result)
{
    // 每个递归节点都对应一个合法子集。
    result.push_back(path);
    for (int i = start; i < static_cast<int>(nums.size()); ++i) {
        path.push_back(nums[i]);
        subsetsDfs(nums, i + 1, path, result);
        path.pop_back();
    }
}

std::vector<std::vector<int>> subsets(const std::vector<int>& nums)
{
    std::vector<std::vector<int>> result;
    std::vector<int> path;
    subsetsDfs(nums, 0, path, result);
    return result;
}

/**
 * 9. 子集 II (LeetCode 90)
 * 难度：中等
 * 思路：排序 + 同层去重。与全排列 II 的去重逻辑一致：
 *       nums[i] == nums[i-1] 且 i > start 时跳过。
 * 复杂度：时间 O(N * 2^N)，空间 O(N)
 */
void subsetsWithDupDfs(const std::vector<int>& nums, int start,
                       std::vector<int>& path, std::vector<std::vector<int>>& result)
{
    result.push_back(path);
    for (int i = start; i < static_cast<int>(nums.size()); ++i) {
        // 同层去重：本层已经选过相同值则跳过。
        if (i > start && nums[i] == nums[i - 1]) continue;
        path.push_back(nums[i]);
        subsetsWithDupDfs(nums, i + 1, path, result);
        path.pop_back();
    }
}

std::vector<std::vector<int>> subsetsWithDup(std::vector<int> nums)
{
    std::sort(nums.begin(), nums.end());
    std::vector<std::vector<int>> result;
    std::vector<int> path;
    subsetsWithDupDfs(nums, 0, path, result);
    return result;
}

// ============================================================================
// 四、其他回溯经典 —— 全排列思想的变体应用
// ============================================================================

/**
 * 10. 电话号码的字母组合 (LeetCode 17)
 * 难度：中等
 * 思路：回溯遍历每个数字对应的字母集合，本质是多集合的笛卡尔积。
 * 复杂度：时间 O(4^N)，N=数字个数；空间 O(N)
 */
void letterCombinationsDfs(const std::string& digits, int idx,
                           const std::vector<std::string>& mapping,
                           std::string& path, std::vector<std::string>& result)
{
    if (idx == static_cast<int>(digits.size())) { result.push_back(path); return; }
    // 当前数字对应的所有可选字母。
    for (char c : mapping[digits[idx] - '0']) {
        path.push_back(c);
        letterCombinationsDfs(digits, idx + 1, mapping, path, result);
        path.pop_back();
    }
}

std::vector<std::string> letterCombinations(const std::string& digits)
{
    if (digits.empty()) return {};
    // 数字到字母的映射表。
    const std::vector<std::string> mapping = {
        "", "", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"
    };
    std::vector<std::string> result;
    std::string path;
    letterCombinationsDfs(digits, 0, mapping, path, result);
    return result;
}

/**
 * 11. 括号生成 (LeetCode 22)
 * 难度：中等
 * 思路：回溯，维护已用左括号数和右括号数。
 *       左括号 < n 时可加左括号；右括号 < 左括号时可加右括号。
 * 复杂度：时间 O(4^N / sqrt(N))（卡特兰数），空间 O(N)
 */
void generateParenthesisDfs(int n, int open, int close, std::string& path,
                            std::vector<std::string>& result)
{
    // 左右括号都用满 n 个时收集结果。
    if (open == n && close == n) { result.push_back(path); return; }
    // 左括号还有余额就可以加。
    if (open < n) {
        path.push_back('(');
        generateParenthesisDfs(n, open + 1, close, path, result);
        path.pop_back();
    }
    // 右括号数必须严格小于左括号数才合法。
    if (close < open) {
        path.push_back(')');
        generateParenthesisDfs(n, open, close + 1, path, result);
        path.pop_back();
    }
}

std::vector<std::string> generateParenthesis(int n)
{
    std::vector<std::string> result;
    std::string path;
    generateParenthesisDfs(n, 0, 0, path, result);
    return result;
}

/**
 * 12. 分割回文串 (LeetCode 131)
 * 难度：中等
 * 思路：回溯选择切割点。从 start 出发，枚举每个可能的回文子串结尾，
 *       如果是回文就切下去，继续递归剩余部分。
 * 复杂度：时间 O(N * 2^N)，空间 O(N)
 */
bool isPalindrome(const std::string& s, int left, int right)
{
    while (left < right) {
        if (s[left++] != s[right--]) return false;
    }
    return true;
}

void partitionDfs(const std::string& s, int start, std::vector<std::string>& path,
                  std::vector<std::vector<std::string>>& result)
{
    // 切割点到达末尾，说明整串被成功分割。
    if (start >= static_cast<int>(s.size())) { result.push_back(path); return; }
    for (int end = start; end < static_cast<int>(s.size()); ++end) {
        // 只有当前子串是回文时才继续切割。
        if (isPalindrome(s, start, end)) {
            path.push_back(s.substr(start, end - start + 1));
            partitionDfs(s, end + 1, path, result);
            path.pop_back();
        }
    }
}

std::vector<std::vector<std::string>> partition(const std::string& s)
{
    std::vector<std::vector<std::string>> result;
    std::vector<std::string> path;
    partitionDfs(s, 0, path, result);
    return result;
}

/**
 * 13. N 皇后 (LeetCode 51)
 * 难度：困难
 * 思路：逐行放置皇后，回溯选择列。用三个数组标记列、主对角线、副对角线的占用。
 *       主对角线：row - col + n - 1；副对角线：row + col。
 * 复杂度：时间 O(N!)，空间 O(N)
 */
void solveNQueensDfs(int n, int row, std::vector<bool>& cols,
                     std::vector<bool>& diag1, std::vector<bool>& diag2,
                     std::vector<std::string>& board,
                     std::vector<std::vector<std::string>>& result)
{
    if (row == n) { result.push_back(board); return; }
    for (int col = 0; col < n; ++col) {
        // 主对角线索引：同一对角线上 row - col 恒定（加 n-1 偏移避免负数）。
        int d1 = row - col + n - 1;
        // 副对角线索引：同一对角线上 row + col 恒定。
        int d2 = row + col;
        if (cols[col] || diag1[d1] || diag2[d2]) continue;

        // 放置皇后。
        cols[col] = diag1[d1] = diag2[d2] = true;
        board[row][col] = 'Q';
        solveNQueensDfs(n, row + 1, cols, diag1, diag2, board, result);
        // 撤销。
        board[row][col] = '.';
        cols[col] = diag1[d1] = diag2[d2] = false;
    }
}

std::vector<std::vector<std::string>> solveNQueens(int n)
{
    std::vector<std::vector<std::string>> result;
    // 初始化空棋盘。
    std::vector<std::string> board(n, std::string(n, '.'));
    // 列占用标记。
    std::vector<bool> cols(n, false);
    // 主对角线占用标记（左上→右下），共 2n-1 条。
    std::vector<bool> diag1(2 * n - 1, false);
    // 副对角线占用标记（右上→左下），共 2n-1 条。
    std::vector<bool> diag2(2 * n - 1, false);
    solveNQueensDfs(n, 0, cols, diag1, diag2, board, result);
    return result;
}

}
