/**
 * Given a string, you need to reverse the order of characters in each word within a
 *  sentence while still preserving whitespace and initial word order.

Example 1:
Input: "Let's take LeetCode contest"
Output: "s'teL ekat edoCteeL tsetnoc"
 */

class Solution {
public:
    string reverseWords(string s) {
        if(s.size() < 2) return s;
        size_t i = 0, j = s.find_first_of(' ');
        while(j != string::npos) {
            std::reverse(begin(s)+i, begin(s)+j);
            i = s.find_first_not_of(' ', j);
            j = s.find_first_of(' ', i);
        }
        if(i != string::npos) {
            reverse(begin(s)+i, end(s));
        }
        return s;
    }
};