/**
 * Given a string and an integer k,
 *  you need to reverse the first k characters for every 2k characters counting from the start of the string.
 *   If there are less than k characters left, reverse all of them.
 *    If there are less than 2k but greater than or equal to k characters,
 *    then reverse the first k characters and left the other as original.
Example:
Input: s = "abcdefg", k = 2
Output: "bacdfeg"
 */


/**
 * Solution:使用std::reverse，输入为区间指针，因此通过遍历，输入需要的指针区间，注意分情况讨论k>size的情况
 */


class Solution {
public:
    string reverseStr(string s, int k) {
        if(s.size() < 2 || k < 2) return s;
        auto it = s.begin();
        for(int i = 0; i < s.size(); i += 2 * k) {
            auto start = it + i;
            if(i + k > s.size()) {
                reverse_locall(start, s.size() - i);
            }
            else {
                reverse_locall(start, k);
            }
        }
        return std::move(s);
    }

private:
    inline void reverse_locall(string::iterator start, int len) {
        std::reverse(start, start + len);
    }
};