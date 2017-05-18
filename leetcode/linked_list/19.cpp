/**
 * Given a linked list, remove the nth node from the end of list and return its head.

For example,

   Given linked list: 1->2->3->4->5, and n = 2.

   After removing the second node from the end, the linked list becomes 1->2->3->5.
 */

/**
 * 使用顺序容器缓存结点指针，定位到位置并改变即可
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
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        std::vector<ListNode*> v;
        ListNode* p = head;
        while(p) {
            v.push_back(p);
            p = p->next;
        }
        p = v[v.size() - n];
        if(p == v.front()) return p->next;
        ListNode* f = v[v.size() - n - 1];
        f->next = p->next;
        return head;
    }

};