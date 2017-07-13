/**
 * Given a sorted linked list, delete all duplicates such that each element appear only once.

For example,
Given 1->1->2, return 1->2.
Given 1->1->2->3->3, return 1->2->3.
 */

/**
 * 使用两个指针，其中一个指针指向前一节点，判断当前节点值和前一节点值，然后根据值的情况进行去除或是后移
 */

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */

class Solution {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        if(!head || !(head->next)) return head;
        ListNode *pre = head, *work = head->next;
        while(work != nullptr) {
            if(work->val == pre->val) {
                pre->next = work->next;
            } else {
                pre = work;
            }
            work = work->next;
        }
        return head;
    }
};