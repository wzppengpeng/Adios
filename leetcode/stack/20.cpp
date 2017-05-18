/**
 * Given a string containing just the characters '(', ')', '{', '}', '[' and ']',
 *  determine if the input string is valid.

The brackets must close in the correct order, "()" and "()[]{}" are all valid but "(]" and "([)]" are not
 */

/**
 * 典型的压栈的方法，注意最后返回应当使栈为空
 */


class Solution {
public:
    bool isValid(string s) {
        stack<char> st;
        for(auto ch : s) {
            if(ch == '(' || ch == '[' || ch == '{') {
                st.push(ch);
            }
            else {
                if(st.empty() || !is_math(st.top(), ch)) return false;
                st.pop();
            }
        }
        return st.empty();
    }

    inline bool is_math(char pre, char post) {
        if(post == ')') {
            return pre == '(';
        }
        else if(post == ']') {
            return pre == '[';
        }
        else
            return pre == '{';
    }
};