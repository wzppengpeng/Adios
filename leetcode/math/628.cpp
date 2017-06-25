/**
 * Given an integer array, find three numbers whose product is maximum and output the maximum product.
 */

/**
 * 数学问题，乘积最大的很有可能会是两个负数，因此分别拿出来比一比
 */


class Solution {
public:
    int maximumProduct(vector<int>& nums) {
        if(nums.size() < 2) return 0;
        std::sort(std::begin(nums), std::end(nums), std::greater<int>());
        int tmp_max = nums[2] * nums[1] * nums[0];
        size_t j = nums.size() - 1;
        return std::max(tmp_max, nums[0] * nums[j] * nums[j - 1]);
    }
};