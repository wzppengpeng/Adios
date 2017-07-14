/**
 * Given a linked list and a value x, partition it such that all nodes less than x come before nodes greater than or equal to x.

You should preserve the original relative order of the nodes in each of the two partitions.

For example,
Given 1->4->3->2->5->2 and x = 3,
return 1->2->2->4->3->5.
 */

/**
 * 首先用一个数组记录一下小于x的值，然后根据这些值先创建一个链表
 * 然后遍历原链表，将原链表大于x的补在后面
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
    ListNode* partition(ListNode* head, int x) {
        if(!head || !(head->next)) return head;
        vector<int> less_ones;
        auto p = head;
        while(p) {
            if(p->val < x) less_ones.emplace_back(p->val);
            p = p->next;
        }
        if(less_ones.empty()) return head;
        ListNode* new_head = new ListNode(less_ones.front());
        p = new_head;
        for(size_t i = 1; i < less_ones.size(); ++i) {
            ListNode* tmp = new ListNode(less_ones[i]);
            p->next = tmp;
            p = tmp;
        }
        auto q = head;
        while(q) {
            auto tmp = q;
            q = q->next;
            if(tmp->val >= x) {
                p->next = tmp;
                tmp->next = nullptr;
                p = p->next;
            }
        }
        return new_head;
    }
};