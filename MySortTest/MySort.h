#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>   // 非递归快排需要用到栈
#include <string>

/**
 * 1. 插入排序 (Insertion Sort)
 * -----------------------------------------------------------
 * 时间复杂度：最优 O(N) [已排序], 最坏 O(N^2) [逆序], 平均 O(N^2)
 * 空间复杂度：O(1)
 * 稳定性：稳定 (Stable)
 * -----------------------------------------------------------
 */
template <typename T>
void insertionSort(T arr[], int n) {
    for (int i = 1; i < n; ++i) {
        T key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

/**
 * 2. 快速排序 - 递归实现 (Quick Sort Recursive)
 * -----------------------------------------------------------
 * 时间复杂度：最优 O(N log N), 最坏 O(N^2) [基准选得差且基本有序时]
 * 空间复杂度：O(log N) [递归调用栈]
 * 稳定性：不稳定 (Unstable)
 * -----------------------------------------------------------
 */
template <typename T>
void quickSortRecursive(T arr[], int left, int right) {
    if (left >= right) return;

    T pivot = arr[left];
    int i = left, j = right;
    while (i < j) {
        while (i < j && arr[j] >= pivot) j--;
        arr[i] = arr[j];
        while (i < j && arr[i] <= pivot) i++;
        arr[j] = arr[i];
    }
    arr[i] = pivot;

    quickSortRecursive(arr, left, i - 1);
    quickSortRecursive(arr, i + 1, right);
}

/**
 * 3. 快速排序 - 非递归实现 (Quick Sort Iterative)
 * -----------------------------------------------------------
 * 逻辑：利用 std::stack 手动模拟系统函数调用栈，避免深层递归导致栈溢出。
 * 复杂度与稳定性：同递归版。
 * -----------------------------------------------------------
 */
template <typename T>
void quickSortIterative(T arr[], int n) {
    if (n <= 1) return;

    std::stack<std::pair<int, int>> s;
    s.push({0, n - 1});

    while (!s.empty()) {
        std::pair<int, int> curr = s.top();
        s.pop();
        int left = curr.first;
        int right = curr.second;

        if (left >= right) continue;

        // Partition 过程
        T pivot = arr[left];
        int i = left, j = right;
        while (i < j) {
            while (i < j && arr[j] >= pivot) j--;
            arr[i] = arr[j];
            while (i < j && arr[i] <= pivot) i++;
            arr[j] = arr[i];
        }
        arr[i] = pivot;

        // 将子区间入栈
        if (i - 1 > left) s.push({left, i - 1});
        if (i + 1 < right) s.push({i + 1, right});
    }
}

/**
 * 4. 归并排序 (Merge Sort)
 * -----------------------------------------------------------
 * 时间复杂度：最优 O(N log N), 最坏 O(N log N), 平均 O(N log N)
 * 空间复杂度：O(N) [辅助数组]
 * 稳定性：稳定 (Stable)
 * -----------------------------------------------------------
 */
template <typename T>
void merge(T arr[], int left, int mid, int right) {
    int n1 = mid - left + 1, n2 = right - mid;
    T *L = new T[n1], *R = new T[n2];
    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
    delete[] L; delete[] R;
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
 * 时间复杂度：最优 O(N log N), 最坏 O(N log N), 平均 O(N log N)
 * 空间复杂度：O(1)
 * 稳定性：不稳定 (Unstable)
 * -----------------------------------------------------------
 */
template <typename T>
void heapify(T arr[], int n, int i) {
    int largest = i, l = 2 * i + 1, r = 2 * i + 2;
    if (l < n && arr[l] > arr[largest]) largest = l;
    if (r < n && arr[r] > arr[largest]) largest = r;
    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

template <typename T>
void heapSort(T arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--) heapify(arr, n, i);
    for (int i = n - 1; i > 0; i--) {
        std::swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

// --- 打印工具 ---
template <typename T>
void printArray(T arr[], int n, const std::string& name) {
    std::cout << name << ": ";
    for (int i = 0; i < n; i++) std::cout << arr[i] << " ";
    std::cout << std::endl;
}