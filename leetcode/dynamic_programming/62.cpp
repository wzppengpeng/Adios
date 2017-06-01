/**
 * A robot is located at the top-left corner of a m x n grid (marked 'Start' in the diagram below).

The robot can only move either down or right at any point in time. The robot is trying to reach the bottom-right corner of the grid (marked 'Finish' in the diagram below).
 */

/**
 * 动态规划问题，对于当前位置，只能从上或者从左来，因此，当前位置的路径数量应该是上+左的数量
 */


class Solution {
public:
    int uniquePaths(int m, int n) {
        if(m == 0 || n == 0) return 0;
        vector<vector<int>> dp(m, vector<int>(n, 1));
        for(int i = 1; i < m; ++i) {
            for(int j = 1; j < n; ++j) {
                dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
            }
        }
        return dp[m - 1][n - 1];
    }
};