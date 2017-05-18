/**
 * Given a string s, find the longest palindromic subsequence's length in s. You may assume that the maximum length of s is 1000.
 */

/**
 * 动态规划:问题其实又可以抽象为找出两个字符串最大相同的字符，使用二位矩阵进行递推
 */


class Solution {
public:
    int longestPalindromeSubseq(string s) {
        int n = s.size();
        if(n < 2) return n;
        m_s = &s;
        vector<vector<int>> dp(n, vector<int>(n, 0));
        for(int i = 0; i < n; ++i) {
            for(int j = 0; j < n; ++j) {
                if(j == 0) {
                    if(s[i] == p(0)) dp[i][0] = 1;
                    if(i) dp[i][0] = max(dp[i][0], dp[i - 1][0]);
                    continue;
                }
                if(i == 0) {
                    if(s[0] == p(j)) dp[0][j] = 1;
                    if(j) dp[0][j] = max(dp[0][j], dp[0][j - 1]);
                    continue;
                }
                if(s[i] == p(j)) {
                    dp[i][j] = max(1 + dp[i - 1][j - 1], max(dp[i - 1][j], dp[i][j - 1]));
                }
                else {
                    dp[i][j] = max(dp[i - 1][j - 1], max(dp[i - 1][j], dp[i][j - 1]));
                }
            }
        }
        m_s = nullptr;
        return dp[n - 1][n - 1];
    }

    string* m_s;

    inline char p(int j) {
        return (*m_s)[m_s->size() - 1 - j];
    }
};