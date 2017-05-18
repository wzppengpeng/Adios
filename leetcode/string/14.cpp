/**
 * Write a function to find the longest common prefix string amongst an array of strings.
 */

/**
 * Solution:不是一个很麻烦的问题，但是尽量值循环一遍，同时注意结束条件
 */


class Solution {
public:
    string longestCommonPrefix(vector<string>& strs) {
        if(strs.empty()) return "";
        if(strs.size() == 1) return strs[0];
        int length = 0, j = 0;
        if(strs[0].empty()) return "";
        for(;;++j) {
            if(j == strs[0].size()) break;
            char ch = strs[0][j];
            if(!check(strs, j, ch)) break;
            ++length;
        }
        return std::move(strs[0].substr(0, length));
    }

private:
    bool check(const vector<string>& strs, int j, char ch) {
        for(int i = 1; i < strs.size(); ++i) {
            if(j >= strs[i].size()) return false;
            if(strs[i][j] != ch) return false;
        }
        return true;
    }
};