/**
 * Given a string s, find the longest palindromic substring in s.
 *  You may assume that the maximum length of s is 1000.
 */

/**
 * Solution:从中心考虑，定义双指针左和右分别从中心向两侧辐射开，记录最左位置和最大距离
 * 注意提前往右跳过重复值
 */

class Solution {
public:
    string longestPalindrome(string s) {
        if(s.empty()) return "";
        int len = s.size();
        int max_left = 0, max_len = 1;
        int left, right;
        for(int i = 0; i < len && i < len - max_len / 2; ) {
            left = right = i;
            //remove duplicates
            while(right < len - 1 && s[right + 1] == s[right]) ++right;
            i = right + 1;
            while(left > 0 && right < len && s[left - 1] == s[right + 1]) {
                ++right;
                --left;
            }
            auto new_len = right - left + 1;
            if(new_len > max_len) {
                max_left = left;
                max_len = new_len;
            }
        }
        return std::move(s.substr(max_left, max_len));
    }
};