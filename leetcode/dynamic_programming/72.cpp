/**
 * Given two words word1 and word2, find the minimum number of steps required to convert word1 to word2. (each operation is counted as 1 step.)

You have the following 3 operations permitted on a word:

a) Insert a character
b) Delete a character
c) Replace a character
 */


/**
 * 动态规划中的经典问题
 * 使用一个二维数字来记录不同子字符串之间的距离，当加一个新的字母会有递推关系
 * 即同时修改，删一个或者添加一个，取三种情况中的最小值。
 * 其中当当前位置字符相同时，对应到斜上方为距离
 */



class Solution {
public:
    int minDistance(string word1, string word2) {
        if(word1.empty() || word2.empty()) return max(word1.size(), word2.size());
        auto m = word1.size(); auto n = word2.size();
        std::vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
        for(size_t i = 1; i <= m; ++i) dp[i][0] = i;
        for(size_t j = 1; j <= n; ++j) dp[0][j] = j;
        for(size_t i = 1; i <= m; ++i) {
            for(size_t j = 1; j <= n; ++j) {
                int d = word1[i - 1] == word2[j - 1] ? 0 : 1;
                dp[i][j] = min(dp[i - 1][j - 1] + d,
                    min(dp[i - 1][j] + 1, dp[i][j - 1] + 1));
            }
        }
        return dp.back().back();
    }
};