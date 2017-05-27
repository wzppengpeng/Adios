/**
 * Find the contiguous subarray within an array (containing at least one number) which has the largest sum.

For example, given the array [-2,1,-3,4,-1,2,1,-5,4],
the contiguous subarray [4,-1,2,1] has the largest sum = 6.
 */

/**
 * 区间求和问题一般使用总体求和然后向前做减法的方法得到区间和
 * 这里在总体求和之后，可以保存当前和的最小值，那么做减法就可以得到区间最大值
 * 所以动态规划其实其实只需要记住当前区间最大值和总体最小值即可
 */

class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        vector<int> sums(nums.size(), nums.front());
        vector<int> min_dp(nums.size(), min(nums.front(), 0));
        for(size_t i = 1; i < nums.size(); ++i) {
            sums[i] = nums[i] + sums[i - 1];
        }
        //handle for the first one
        int max_sum_val = sums.front();
        for(size_t i = 1; i < nums.size(); ++i) {
            max_sum_val = max(max_sum_val, sums[i] - min_dp[i - 1]);
            min_dp[i] = min(min_dp[i - 1], sums[i]);
        }
        return max_sum_val;
    }
};