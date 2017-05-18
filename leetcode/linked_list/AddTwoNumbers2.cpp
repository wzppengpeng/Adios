/**
 * You are given two linked lists representing two non-negative numbers. The digits are stored in reverse order and each of their nodes contain a single digit. Add the two numbers and return it as a linked list.

Input: (2 -> 4 -> 3) + (5 -> 6 -> 4)
Output: 7 -> 0 -> 8
 */

/**
 * Solution:
 *
 *写一个通用函数，输入为指向每一位的指针，以及一个进位符，计算当前位置的值，然后返回是否进位
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
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        bool is_carry = false;
        ListNode* work = nullptr;
        std::vector<ListNode*> v;
        while(l1 || l2 || is_carry) {
            is_carry = add(l1, l2, work, is_carry);
            if(l1) l1 = l1->next;
            if(l2) l2 = l2->next;
            if(work) {
                v.push_back(work);
                work = work->next;
            }
        }
        for(int i = 0; i < v.size() - 1; ++i) {
            v[i]->next = v[i + 1];
        }
        return v.empty() ? nullptr : v[0];
    }

private:
    ListNode* head;

    bool add(ListNode* l1, ListNode* l2, ListNode*& work, bool is_carry) {
        if(l1 == nullptr && l2 == nullptr) {
            if(is_carry) {
                work = new ListNode(1);
            }
            return false;
        }
        if(l1 == nullptr) {
            auto num = l2->val;
            if(is_carry) {
                if(num == 9) {
                    work = new ListNode(0);
                    return true;
                }
                else {
                    work = new ListNode(num + 1);
                    return false;
                }
            }
            else {
                work = new ListNode(num);
                return false;
            }
        }
        if(l2 == nullptr) {
            auto num = l1->val;
            if(is_carry) {
                if(num == 9) {
                    work = new ListNode(0);
                    return true;
                }
                else {
                    work = new ListNode(num + 1);
                    return false;
                }
            }
            else {
                work = new ListNode(num);
                return false;
            }
        }
        auto num = is_carry ? l1->val + l2->val + 1 : l1->val + l2->val;
        if(num > 9) {
            work = new ListNode(num - 10);
            return true;
        }
        else {
            work = new ListNode(num);
            return false;
        }
    }
};