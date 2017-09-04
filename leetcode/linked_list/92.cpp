/**
 * Reverse a linked list from position m to n. Do it in-place and in one-pass.

For example:
Given 1->2->3->4->5->NULL, m = 2 and n = 4,

return 1->4->3->2->5->NULL.
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
    ListNode* reverseBetween(ListNode* head, int m, int n) {
        if(!head) return nullptr;
        ListNode* m_ptr, n_ptr;
        auto work = head;
        int i = 1;
        while(i <= n) {
            if(i == m) m_ptr = work;
            if(i == n) n_ptr = work;
            ++i;
            work = work->next;
        }
        ListNode* p = m_ptr
    }
};