/**
 * Given a string s consists of upper/lower-case alphabets and empty space characters ' ', return the length of last word in the string.

If the last word does not exist, return 0.

Note: A word is defined as a character sequence consists of non-space characters only.

For example,
Given s = "Hello World",
return 5.
 */

/**
 * 唯一需要注意的是，需要先trim一下，把头部和尾部的空格去掉
 */

class Solution {
public:
    int lengthOfLastWord(string s) {
        if(!trim(s)) return 0;
        auto pos = s.find_last_of(' ');
        return pos != string::npos ? s.size() - 1 - pos : s.size();
    }

    bool trim(std::string& str) {
        if (str.empty()) return false;
        str.erase(str.find_last_not_of(" \f\t\n\r\v") + 1);
        str.erase(0, str.find_first_not_of(" \f\t\n\r\v"));
        return true;
    }
};