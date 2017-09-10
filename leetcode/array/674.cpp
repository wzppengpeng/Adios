/**
 * Given an unsorted array of integers, find the length of longest continuous increasing subsequence.

Example 1:
Input: [1,3,5,4,7]
Output: 3
Explanation: The longest continuous increasing subsequence is [1,3,5], its length is 3.
 */


/**
 * 最基本的记录当前最大值和当前最长临时值，并注意更新即可
 */

class Solution {
public:
    int findLengthOfLCIS(vector<int>& nums) {
        if(nums.empty()) return 0;
        int tmp_max_element = nums[0], max_len = 1, tmp_max_len = 1;
        for(size_t i = 1; i < nums.size(); ++i) {
            if(nums[i] > tmp_max_element) {
                ++tmp_max_len;
                if(tmp_max_len > max_len) max_len = tmp_max_len;
            } else {
                tmp_max_len = 1;
            }
            tmp_max_element = nums[i];
        }
        return max_len;
    }
};