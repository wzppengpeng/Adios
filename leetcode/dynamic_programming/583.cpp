/**
 * Given two words word1 and word2, find the minimum number of steps required to make word1 and word2 the same, where in each step you can delete one character in either string.
 */


/**
 * 问题可以转化为最长的公共序列，使用动态规划
 * （二维）
 * 分别记录当前的最长值，根据该点知否相等来更新，注意开头的那一行和一列
 */

class Solution {
public:
    int minDistance(string word1, string word2) {
        if(word1.empty() || word2.empty()) {
            return std::max(word1.size(), word2.size());
        }
        int m = word1.size(), n = word2.size();
        std::vector<vector<int>> dp(m, vector<int>(n, 0));
        for(int i = 0; i < m; ++i) {
            if(word1[i] == word2[0]) dp[i][0] = 1;
            if(i) dp[i][0] = max(dp[i-1][0], dp[i][0]);
        }
        for(int j = 0; j < n; ++j) {
            if(word2[j] == word1[0]) dp[0][j] = 1;
            if(j) dp[0][j] = max(dp[0][j-1], dp[0][j]);
        }
        for(int i = 1; i < m; ++i) {
            for(int j = 1; j < n; ++j) {
                if(word1[i] != word2[j]) {
                    dp[i][j] = std::max(dp[i-1][j-1], std::max(dp[i][j-1], dp[i-1][j]));
                }
                else {
                    dp[i][j] = max(1+dp[i-1][j-1], std::max(dp[i][j-1], dp[i-1][j]));
                }
            }
        }
        return (m + n) - 2 * dp[m-1][n-1];
    }
};