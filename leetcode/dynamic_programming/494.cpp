/**
 * You are given a list of non-negative integers, a1, a2, ..., an, and a target, S. Now you have 2 symbols + and -. For each integer, you should choose one from + and - as its new symbol.

Find out how many ways to assign symbols to make sum of integers equal to target S.
 */

/**
 * 很简单的方法就是每一个位置两种值进行遍历相加，可以通过，代码也很简单，但是不是最优解
 */


class Solution {
public:
    int findTargetSumWays(vector<int>& nums, int S) {
        return dp(nums, S, 0, 0);
    }

    int dp(const vector<int>& nums, int S, size_t i, int sum_val) {
        if(i == nums.size()) {
            return sum_val == S ? 1 : 0;
        }
        return dp(nums, S, i + 1, sum_val + nums[i])
             + dp(nums, S, i + 1, sum_val - nums[i]);
    }
};