/**
 * Given a sorted linked list, delete all nodes that have duplicate numbers, leaving only distinct numbers from the original list.

For example,
Given 1->2->3->3->4->4->5, return 1->2->5.
Given 1->1->1->2->3, return 2->3.
 */

/**
 * 两遍遍历链表，其中第一遍使用哈希表之类的快速存储容器存储单独的值
 * 第二遍遍历时处理那些不是单独的节点，将非单独的节点跳过，只剩下单独的
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
        // handle first
        ListNode *pre = head, *work = head->next, *post = work->next;
        unordered_set<int> distinct;
        if(pre->val != work->val) distinct.emplace(pre->val);
        while(work != nullptr) {
            if((!post && pre->val != work->val) ||
                (pre->val != work->val && post->val != work->val)) {
                distinct.emplace(work->val);
            }
            pre = work;
            work = post;
            if(post) post = post->next;
        }
        if(distinct.empty()) return nullptr;
        ListNode* p = head;
        while(distinct.count(p->val) == 0) p = p->next;
        ListNode *new_head = p, *_pre = p;
        p = p->next;
        while(p) {
            if(distinct.count(p->val)) {
                _pre->next = p;
                _pre = p;
            } else {
                _pre->next = nullptr;
            }
            p = p->next;
        }
        return new_head;
    }
};