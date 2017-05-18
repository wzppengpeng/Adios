/**
 * Given a linked list, swap every two adjacent nodes and return its head.

For example,
Given 1->2->3->4, you should return the list as 2->1->4->3.

Your algorithm should use only constant space.
 You may not modify the values in the list, only nodes itself can be changed.
 */

/**
 * 只需要一遍遍历，使用三个指针，分别指向当前，前面哪一个，后面那一个，先更改最前，然后后面，最后处理最后的那一个
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
    ListNode* swapPairs(ListNode* head) {
        if(!head || !head->next) return head;
        //swap the first two
        ListNode* new_head = head->next;
        ListNode *pre, *now, *post;
        now = head; post = head->next; pre = now;
        now->next = post->next;
        post->next = now;
        pre = now;
        now = now->next;
        while(now) {
            post = now->next;
            if(!post) break;
            pre->next = post;
            now->next = post->next;
            post->next = now;
            pre = now;
            now = now->next;
        }
        return new_head;
    }
};