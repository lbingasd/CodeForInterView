#include <iostream>
#include <vector>
#include <stack>

/**
 * 链表节点结构定义
 */
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

/**
 * 1. 链表反转 (Reverse Linked List)
 * -----------------------------------------------------------
 * 【详细逻辑】：
 * 1. 使用三个指针：prev (前驱)、curr (当前)、nextTemp (后继)。
 * 2. 在遍历过程中，先记录下 curr->next，防止断链。
 * 3. 改变 curr->next 的指向，让其指向 prev。
 * 4. prev 和 curr 同步向后移动一位，直到 curr 为空。
 * 【复杂度】：时间 O(N), 空间 O(1)
 * -----------------------------------------------------------
 */
ListNode* reverseList(ListNode* head) {
    ListNode* prev = nullptr;
    ListNode* curr = head;
    while (curr != nullptr) {
        ListNode* nextTemp = curr->next; // 暂存后继
        curr->next = prev;              // 核心：反向指向
        prev = curr;                    // prev 移动到当前
        curr = nextTemp;                // curr 移动到下一个
    }
    return prev; // 新的头节点
}

/**
 * 2. 检测环与寻找环入口 (Detect Cycle & Find Entrance)
 * -----------------------------------------------------------
 * 【详细逻辑】：
 * 1. 快慢指针：slow 每次走一步，fast 每次走两步。
 * 2. 若 fast 与 slow 相遇，证明有环。
 * 3. 寻找入口：相遇后，让 slow 回到 head，fast 保持在相遇点。
 * 4. 两者同时以每次一步的速度移动，再次相遇点即为环入口（数学推导）。
 * 【复杂度】：时间 O(N), 空间 O(1)
 * -----------------------------------------------------------
 */
ListNode* detectCycle(ListNode* head) {
    ListNode *slow = head, *fast = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) { // 发现环
            ListNode* index1 = head;
            ListNode* index2 = slow;
            while (index1 != index2) { // 寻找入口
                index1 = index1->next;
                index2 = index2->next;
            }
            return index1;
        }
    }
    return nullptr;
}

/**
 * 3. 合并两个有序链表 (Merge Two Sorted Lists)
 * -----------------------------------------------------------
 * 【详细逻辑】：
 * 1. 虚拟头节点(Dummy Node)：创建一个 dummy 节点，简化头节点插入逻辑。
 * 2. 比较：遍历两个链表，将较小的节点挂载到 curr->next。
 * 3. 拼接：当其中一个链表遍历完，直接将另一个链表的剩余部分挂在末尾。
 * 【复杂度】：时间 O(M+N), 空间 O(1)
 * -----------------------------------------------------------
 */
ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
    ListNode* dummy = new ListNode(0); // 虚拟头节点
    ListNode* curr = dummy;
    while (l1 && l2) {
        if (l1->val <= l2->val) {
            curr->next = l1;
            l1 = l1->next;
        } else {
            curr->next = l2;
            l2 = l2->next;
        }
        curr = curr->next;
    }
    curr->next = (l1 != nullptr) ? l1 : l2;
    return dummy->next;
}

/**
 * 4. 删除倒数第 N 个节点 (Remove N-th Node From End)
 * -----------------------------------------------------------
 * 【详细逻辑】：
 * 1. 快慢指针：fast 先走 N+1 步（多走一步是为了定位到待删节点的前驱）。
 * 2. 同步走：fast 和 slow 同时移动，直到 fast 为 nullptr。
 * 3. 删除：此时 slow 指向待删节点的前一个，执行 slow->next = slow->next->next。
 * 【复杂度】：时间 O(N), 空间 O(1)
 * -----------------------------------------------------------
 */
void removeNthFromEnd(ListNode** head, int n) {
    ListNode* dummy = new ListNode(0);
    dummy->next = *head;
    ListNode* slow = dummy;
    ListNode* fast = dummy;

    for (int i = 0; i <= n; i++) fast = fast->next;
    while (fast != nullptr) {
        slow = slow->next;
        fast = fast->next;
    }
    ListNode* tmp = slow->next;
    slow->next = slow->next->next;
    delete tmp; // 释放内存
    *head = dummy->next;
    delete dummy;
}

/**
 * 5. 链表排序 - 归并法 (Sort List using Merge Sort)
 * -----------------------------------------------------------
 * 【详细逻辑】：
 * 1. 找中点：使用快慢指针，快指针到尾时，慢指针在中点。
 * 2. 断开：将链表从中点断开，分为左右两半。
 * 3. 递归：递归对左右两半排序。
 * 4. 合并：调用 mergeTwoLists 函数。
 * 【复杂度】：时间 O(N log N), 空间 O(log N) [递归深度]
 * -----------------------------------------------------------
 */
ListNode* sortList(ListNode* head) {
    if (!head || !head->next) return head;
    
    // 快慢指针找中点
    ListNode *slow = head, *fast = head, *pre = nullptr;
    while (fast && fast->next) {
        pre = slow;
        slow = slow->next;
        fast = fast->next->next;
    }
    pre->next = nullptr; // 断开链表

    return mergeTwoLists(sortList(head), sortList(slow));
}