/**
 * Given a string, your task is to count how many palindromic substrings in this string.

The substrings with different start indexes or end indexes are counted as different substrings even they consist of same characters.

Example 1:
Input: "abc"
Output: 3
Explanation: Three palindromic strings: "a", "b", "c".
Example 2:
Input: "aaa"
Output: 6
Explanation: Six palindromic strings: "a", "a", "a", "aa", "aa", "aaa".
 */


/**
 * 下面这个方法就是暴力法，使用双指针确定区间，判断该区间是不是回文
 */
class Solution {
public:
    int countSubstrings(string s) {
        if(s.size() < 2) return s.size();
        int res = 0;
        for(size_t i = 0; i < s.size(); ++i) {
            for(size_t j = i; j < s.size(); ++j) {
                if(is_palindromic(s, i, j)) ++res;
            }
        }
        return res;
    }

    bool is_palindromic(const string& s, size_t i, size_t j) {
        while(i < j) {
            if(s[i++] != s[j--]) return false;
        }
        return true;
    }
};


/**
 * dp solution
 */
class Solution {
public:
    int countSubstrings(string s) {
        if(s.size()==0) return 0;
        int res=0;
        vector<vector<int>> dp(s.size(), vector<int>(s.size(), 0));
        dp[0][0]=2;
        for(int i=1;i<s.size();i++) {
            for(int j=0;j<i;j++) {
                if(dp[i-1][j]==2&&s[i]==s[i-1]) dp[i][j+1]=2;
                else if(dp[i-1][j]>0&&j!=i-1&&s[i-j-2]==s[i]) dp[i][j+2]=1;
            }
            dp[i][0]=2;
        }
        for(int i=0;i<dp.size();i++) {
            for(int j=0;j<dp[i].size();j++) res+=min(1, dp[i][j]);
        }
        return res;
    }
};