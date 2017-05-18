/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */

/**
 * the first is the high bit
 * Solution use deque to reverse node, other is the same as problem 2
 * however this is not a very good solution for deque is dynamic= =
 */

class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        if(!l1 && !l2) return nullptr;
        if(!l1) return l2;
        if(!l2) return l1;
        gen_vector(l1, l2);
        auto l1_iter = v1.begin();
        auto l2_iter = v2.begin();
        bool is_carry = false;
        while(l1_iter != v1.end() || l2_iter != v2.end() || is_carry) {
            is_carry = add(l1_iter, l2_iter, is_carry);
            if(l1_iter != v1.end()) ++l1_iter;
            if(l2_iter != v2.end()) ++l2_iter;
        }
        auto it = res.rbegin();
        ListNode* head = new ListNode(*it);
        ListNode* p = head;
        ++it;
        for(; it != res.rend(); ++it) {
            p->next = new ListNode(*it);
            p = p->next;
        }
        return head;
    }

private:
    std::deque<int> v1;
    std::deque<int> v2;
    deque<int> res;

    void gen_vector(ListNode* l1, ListNode* l2) {
        //for l1
        ListNode* p = l1;
        while(p) {
            v1.push_front(p->val);
            p = p->next;
        }
        p = l2;
        while(p) {
            v2.push_front(p->val);
            p = p->next;
        }
    }

    bool add(deque<int>::iterator l1_iter, deque<int>::iterator l2_iter, bool is_carry) {
        if(l1_iter == v1.end() && l2_iter == v2.end()) {
            if(is_carry) {
                res.push_back(1);
            }
            return false;
        }
        if(l1_iter == v1.end()) {
            if(is_carry) {
                if(*l2_iter == 9) {
                    res.push_back(0);
                    return true;
                }
                else {
                    res.push_back(*l2_iter + 1);
                    return false;
                }
            }
            else {
                res.push_back(*l2_iter);
                return false;
            }
        }
        if(l2_iter == v2.end()) {
            if(is_carry) {
                if(*l1_iter == 9) {
                    res.push_back(0);
                    return true;
                }
                else {
                    res.push_back(*l1_iter + 1);
                    return false;
                }
            }
            else {
                res.push_back(*l1_iter);
                return false;
            }
        }
        auto num = is_carry ? *l1_iter + *l2_iter + 1 : *l1_iter + *l2_iter;
        if(num > 9) {
            res.push_back(num - 10);
            return true;
        }
        else {
            res.push_back(num);
            return false;
        }
    }

};