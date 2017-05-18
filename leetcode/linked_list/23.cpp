/**
 * Merge k sorted linked lists and return it as one sorted list. Analyze and describe its complexity.
 */

/**
 * 使用21题的方法，依次合并，虽然性能有点慢= =
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
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        if(lists.empty()) return nullptr;
        if(lists.size() == 1) return lists.front();
        auto work = mergeTwoLists(lists[0], lists[1]);
        // ListNode* head;
        for(size_t i = 2; i < lists.size(); ++i) {
            work = mergeTwoLists(work, lists[i]);
        }
        return work;
    }

    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        if(!l1) return l2;
        if(!l2) return l1;
        ListNode* work, *head;
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
};