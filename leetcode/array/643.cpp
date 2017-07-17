/**
 * Given an array consisting of n integers, find the contiguous subarray of given length k that has the maximum average value. And you need to output the maximum average value.

Example 1:
Input: [1,12,-5,-6,50,3], k = 4
Output: 12.75
Explanation: Maximum average is (12-5-6+50)/4 = 51/4 = 12.75
 */

/**
 * 使用滑窗，因为长度一样，所以只需要判断滑窗内和的大小，找最大值即可。
 */

class Solution {
public:
    double findMaxAverage(vector<int>& nums, int k) {
        if(nums.empty()) return 0.;
        if(nums.size() == 1 || k == 1) return *std::max_element(begin(nums), end(nums));
        double max_ave = 0;
        int sum_val = accumulate(std::begin(nums), std::begin(nums) + k, 0);
        int tmp_sum = sum_val;
        for(int i = k; i < nums.size(); ++i) {
            tmp_sum -= nums[i - k];
            tmp_sum += nums[i];
            if(tmp_sum > sum_val) sum_val = tmp_sum;
        }
        return static_cast<double>(sum_val) / k;
    }
};