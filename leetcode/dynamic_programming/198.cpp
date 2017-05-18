/**
 * You are a professional robber planning to rob houses along a street.
 *  Each house has a certain amount of money stashed,
 *  the only constraint stopping you from robbing each of them is that adjacent houses have security system connected and it will automatically contact the police if two adjacent houses were broken into on the same night.

Given a list of non-negative integers representing the amount of money of each house,
determine the maximum amount of money you can rob tonight without alerting the police.
 */


/**
 * Solution:递推dp的递进关系，因为只能偷隔着的，所以关注前一天， 和上前一天加今天
 */

class Solution {
public:
    int rob(vector<int>& nums) {
        if(nums.empty()) return 0;
        if(nums.size() == 1) return nums.front();
        vector<int> dp(nums.size(), 0);
        dp[0] = nums[0];
        dp[1] = max(nums[1], dp[0]);
        for(size_t i = 2; i < nums.size(); ++i) {
            dp[i] = max(dp[i - 1], dp[i - 2] + nums[i]);
        }
        return dp.back();
    }
};