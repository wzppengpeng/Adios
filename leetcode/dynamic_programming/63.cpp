/**
 * Follow up for "Unique Paths":

Now consider if some obstacles are added to the grids. How many unique paths would there be?

An obstacle and empty space is marked as 1 and 0 respectively in the grid.

For example,
There is one obstacle in the middle of a 3x3 grid as illustrated below.

[
  [0,0,0],
  [0,1,0],
  [0,0,0]
]
The total number of unique paths is 2.
 */


/**
 * 还是一样的思路，注意如果有阻碍这个点能到达的数量就是一了
 * 递推关系还是当前位置只能从上方和左方到达
 */


class Solution {
public:
    int uniquePathsWithObstacles(vector<vector<int>>& obstacleGrid) {
        int m = obstacleGrid.size();
        int n = obstacleGrid.front().size();
        vector<vector<int>> dp(m, vector<int>(n, 0));
        if(obstacleGrid[0][0] == 1) return 0;
        dp[0][0] = 1;
        for(int i = 1; i < m; ++i) dp[i][0] = obstacleGrid[i][0] == 1 ? 0 : dp[i - 1][0];
        for(int j = 1; j < n; ++j) dp[0][j] = obstacleGrid[0][j] == 1 ? 0 : dp[0][j - 1];
        for(int i = 1; i < m; ++i) {
            for(int j = 1; j < n; ++j) {
                dp[i][j] = obstacleGrid[i][j] == 1 ? 0 :
                    dp[i - 1][j] + dp[i][j - 1];
            }
        }
        return dp[m - 1][n - 1];
    }
};