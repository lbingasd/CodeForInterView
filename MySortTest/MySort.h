#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>   // 用于非递归快排的栈
#include <string>

/**
 * 1. 插入排序 (Insertion Sort)
 * -----------------------------------------------------------
 * 【详细逻辑】：
 * 1. 将数组视为两部分：左侧已排序，右侧未排序。初始时左侧只有一个元素。
 * 2. 每次从未排序部分取第一个元素（key），在已排序部分从后往前扫描。
 * 3. 如果已排序元素比 key 大，就将其向后挪动一位。
 * 4. 重复此过程，直到找到一个不大于 key 的位置，将 key 插入其后。
 * 【复杂度】：最优 O(N) [已排序], 最坏 O(N^2) [逆序]
 * 【稳定性】：稳定。相等元素不会交换，维持原有顺序。
 * -----------------------------------------------------------
 */
template <typename T>
void insertionSort(T arr[], int n) {
    for (int i = 1; i < n; ++i) {
        T key = arr[i]; 
        int j = i - 1;
        // 核心：在已排序序列中“腾位置”
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j]; // 元素后移
            j--;
        }
        arr[j + 1] = key; // 插入空出的位置
    }
}

/**
 * 2. 快速排序 - 递归实现 (Quick Sort Recursive)
 * -----------------------------------------------------------
 * 【详细逻辑】：
 * 1. 选基准：通常选区间第一个元素为基准(pivot)。
 * 2. 分区(Partition)：使用双指针 i 和 j。j 先从右往左找小于 pivot 的数，
 * i 再从左往右找大于 pivot 的数。
 * 3. 交换与归位：通过交换确保 pivot 左边全小于它，右边全大于它。
 * 4. 递归：对 pivot 左右两个独立的子区间重复上述过程。
 * 【复杂度】：最优/平均 O(N log N), 最坏 O(N^2) [基准选到极值且数组有序]
 * 【稳定性】：不稳定。由于存在跨越式交换，相同元素的相对位置可能改变。
 * -----------------------------------------------------------
 */
template <typename T>
void quickSortRecursive(T arr[], int left, int right) {
    if (left >= right) return;

    T pivot = arr[left];
    int i = left, j = right;
    while (i < j) {
        // 逻辑：右边找小填左坑，左边找大填右坑
        while (i < j && arr[j] >= pivot) j--;
        arr[i] = arr[j];
        while (i < j && arr[i] <= pivot) i++;
        arr[j] = arr[i];
    }
    arr[i] = pivot; // pivot 归位

    quickSortRecursive(arr, left, i - 1);
    quickSortRecursive(arr, i + 1, right);
}

/**
 * 3. 快速排序 - 非递归实现 (Quick Sort Iterative)
 * -----------------------------------------------------------
 * 【详细逻辑】：
 * 1. 模拟栈：递归的本质是压栈。我们手动建立一个 std::stack 来存储区间的 [left, right]。
 * 2. 迭代处理：只要栈不为空，弹出一组边界进行一次分区(Partition)。
 * 3. 判定入栈：分区结束后，如果左侧或右侧子区间长度 > 1，就将其边界压入栈中。
 * 【场景】：适用于担心系统递归栈深度过大导致 Stack Overflow 的极端环境。
 * -----------------------------------------------------------
 */
template <typename T>
void quickSortIterative(T arr[], int n) {
    if (n <= 1) return;

    std::stack<std::pair<int, int>> s;
    s.push({0, n - 1}); // 初始区间入栈

    while (!s.empty()) {
        std::pair<int, int> curr = s.top();
        s.pop();
        int left = curr.first, right = curr.second;

        // Partition 核心逻辑
        T pivot = arr[left];
        int i = left, j = right;
        while (i < j) {
            while (i < j && arr[j] >= pivot) j--;
            arr[i] = arr[j];
            while (i < j && arr[i] <= pivot) i++;
            arr[j] = arr[i];
        }
        arr[i] = pivot;

        // 判定子区间是否需要继续分裂入栈
        if (left < i - 1) s.push({left, i - 1});
        if (right > i + 1) s.push({i + 1, right});
    }
}

/**
 * 4. 归并排序 (Merge Sort)
 * -----------------------------------------------------------
 * 【详细逻辑】：
 * 1. 分割(Divide)：将当前区间一分为二，直到每个子区间只剩 1 个元素。
 * 2. 合并(Merge)：这是核心。开辟临时空间，用两个指针指向两个有序子区间的开头。
 * 3. 比较：谁小谁就放入临时空间，直到两个子区间合并完。
 * 4. 拷贝：将排好序的临时数组覆盖回原数组。
 * 【复杂度】：稳定在 O(N log N)，因为分割总是对等的。
 * 【稳定性】：稳定。在合并时，如果 `L[i] == R[j]`，优先放左边的元素。
 * -----------------------------------------------------------
 */
template <typename T>
void merge(T arr[], int left, int mid, int right) {
    int n1 = mid - left + 1, n2 = right - mid;
    T *L = new T[n1], *R = new T[n2]; // 辅助内存

    // 拷贝数据到临时数组
    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    // 双指针对比合并
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    // 处理剩下的尾巴
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    delete[] L; delete[] R; // 必须手动释放，否则内存泄漏
}

template <typename T>
void mergeSort(T arr[], int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

/**
 * 5. 堆排序 (Heap Sort)
 * -----------------------------------------------------------
 * 【详细逻辑】：
 * 1. 建堆：从最后一个非叶子节点开始，向上进行 heapify。heapify 的逻辑是对比父子节点，
 * 如果孩子比父亲大，则交换，并向下递归。最终形成大顶堆（根节点最大）。
 * 2. 排序：将堆顶（最大值）与数组末尾交换。此时末尾已定。
 * 3. 收缩：将剩下的 n-1 个元素重新 heapify 维持大顶堆，重复此过程。
 * 【复杂度】：O(N log N)，空间 O(1)。
 * 【稳定性】：不稳定。堆调整过程中存在远距离的父子节点交换。
 * -----------------------------------------------------------
 */
template <typename T>
void heapify(T arr[], int n, int i) {
    int largest = i;     // 假设当前节点最大
    int l = 2 * i + 1;   // 左孩子索引
    int r = 2 * i + 2;   // 右孩子索引

    // 如果孩子比父节点大，更新索引
    if (l < n && arr[l] > arr[largest]) largest = l;
    if (r < n && arr[r] > arr[largest]) largest = r;

    // 如果最大者不是父节点，则交换并继续向下递归调整
    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

template <typename T>
void heapSort(T arr[], int n) {
    // 第一步：构建初始大顶堆
    for (int i = n / 2 - 1; i >= 0; i--) 
        heapify(arr, n, i);

    // 第二步：不断提取根节点并重新平衡
    for (int i = n - 1; i > 0; i--) {
        std::swap(arr[0], arr[i]); // 最大值移到当前末尾
        heapify(arr, i, 0);        // 重新调整剩下 i 个元素
    }
}

// --- 测试主函数 ---
/*int main() {
    int raw[] = {19, 3, 8, 24, 7, 10, 15};
    int n = sizeof(raw) / sizeof(raw[0]);

    auto run_test = [&](auto func, std::string title, bool is_recursive) {
        int temp[7];
        std::copy(raw, raw + n, temp);
        if (is_recursive) func(temp, 0, n - 1);
        else func(temp, n);
        
        std::cout << title << ": ";
        for (int x : temp) std::cout << x << " ";
        std::cout << std::endl;
    };

    run_test(insertionSort<int>, "1. 插入排序", false);
    run_test(quickSortRecursive<int>, "2. 快速排序(递归)", true);
    run_test(quickSortIterative<int>, "3. 快速排序(非递归)", false);
    run_test(mergeSort<int>, "4. 归并排序", true);
    run_test(heapSort<int>, "5. 堆排序", false);

    return 0;
}*/