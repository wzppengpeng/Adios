/**
 * Given a m x n grid filled with non-negative numbers, find a path from top left to bottom right which minimizes the sum of all numbers along its path.

Note: You can only move either down or right at any point in time.
 */

/**
 * 最最典型的动态规划，当前位置的最大值只和上面左边的最大值有关
 */


class Solution {
public:
    int minPathSum(vector<vector<int>>& grid) {
        int m = grid.size();
        int n = grid.front().size();
        vector<vector<int>> dp(m, vector<int>(n, grid[0][0]));
        for(int i = 1; i < m; ++i) dp[i][0] = dp[i - 1][0] + grid[i][0];
        for(int j = 1; j < n; ++j) dp[0][j] = dp[0][j - 1] + grid[0][j];
        for(int i = 1; i < m; ++i) {
            for(int j = 1; j < n; ++j) {
                dp[i][j] = min(grid[i][j] + dp[i - 1][j], grid[i][j] + dp[i][j - 1]);
            }
        }
        return dp[m - 1][n - 1];
    }
};