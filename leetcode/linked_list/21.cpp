/**
 * Merge two sorted linked lists and return it as a new list.
 *  The new list should be made by splicing together the nodes of the first two lists.
 */

/**
 * 使用双指针分别指向对应的链表，注意开头为空以及结尾为空的情况
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
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        if(!l1) return l2;
        if(!l2) return l1;
        ListNode* work;
        if(l1->val <= l2->val) {
            work = l1;
            l1 = l1->next;
        }
        else {
            work = l2;
            l2 = l2->next;
        }
        head = work;
        while(l1 != nullptr && l2 != nullptr) {
            if(l1->val <= l2->val) {
                work->next = l1;
                l1 = l1->next;
            }
            else {
                work->next = l2;
                l2 = l2->next;
            }
            work = work->next;
        }
        if(!l1) work->next = l2;
        else work->next = l1;
        return head;
    }

    ListNode* head = nullptr;
};