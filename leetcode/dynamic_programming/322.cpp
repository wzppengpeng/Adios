/**
 * You are given coins of different denominations and a total amount of money amount. Write a function to compute the fewest number of coins that you need to make up that amount. If that amount of money cannot be made up by any combination of the coins, return -1.

Example 1:
coins = [1, 2, 5], amount = 11
return 3 (11 = 5 + 5 + 1)

Example 2:
coins = [2], amount = 3
return -1.
 */


/**
 * 可以将amount，设计成一个dp数组
 * 每一个位置的值代表到得到该位置金币的coin数量
 * 那么dp[i]就应该是dp[i - coin[j] + 1,取coin[j]可满足中的最小值
 * 需要注意的是最大值设为amount+1，假设全是1也不会超过amount个coin
 */

class Solution {
public:
    int coinChange(vector<int>& coins, int amount) {
        vector<int> dp(amount + 1, amount + 1);
        dp[0] = 0;
        for(size_t i = 0; i < dp.size(); ++i) {
            for(size_t j = 0; j < coins.size(); ++j) {
                if(coins[j] && i >= static_cast<size_t>(coins[j])) {
                    dp[i] = min(dp[i], dp[i - static_cast<size_t>(coins[j])] + 1);
                }
            }
        }
        return dp[amount] > amount ? -1 : dp[amount];
    }
};