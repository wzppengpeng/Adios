/**
 * Given a string s, partition s such that every substring of the partition is a palindrome.

Return all possible palindrome partitioning of s.

For example, given s = "aab",
Return

[
  ["aa","b"],
  ["a","a","b"]
]
 */


/**
 * 回溯算法
 * 注意这里其实就是一个循环，把当前是回文的字符串插入即可，然后就向下递归，在最后一个的位置插入到最终结果中
 * 一定要注意的是每次添加了新的回文，对于递归字符串一定要注意向下递归后，注意清除，回到初始状态
 */

class Solution {
public:
    vector<vector<string>> partition(string s) {
        vector<vector<string>> res;
        back(s, 0, {}, res);
        return res;
    }

    bool is_palindrome(const string& s) {
        auto len = s.size();
        if(len == 1) return true;
        for(size_t i = 0; i < len / 2; ++i) {
            if(s[i] != s[len - i - 1]) return false;
        }
        return true;
    }

    void back(const string& s, size_t i, vector<string> parti, vector<vector<string>>& res) {
        if(i == s.size()) return;
        for(size_t j = i + 1; j <= s.size(); ++j) {
            auto tmp = s.substr(i, j - i);
            if(is_palindrome(tmp)) {
                parti.emplace_back(tmp);
                back(s, j, parti, res);
                if(j == s.size()) {
                    res.emplace_back(parti);
                }
                parti.pop_back();
            }
        }
    }

};