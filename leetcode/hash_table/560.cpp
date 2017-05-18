/**
 * Given an array of integers and an integer k,
 * you need to find the total number of continuous subarrays whose sum equals to k.
 */

/**
 * 第一，当前位置到前面的和的差可以表示任意位置的区间和
 * 第二，使用哈希表来记录前序和，则当前和与目标差即为区间距离
 */


class Solution {
public:
    int subarraySum(vector<int>& nums, int k) {
        int sum_val = 0, count = 0;
        unordered_map<int, int> cnt {{0, 1}};
        for(auto num : nums) {
            sum_val += num;
            if(cnt.find(sum_val - k) != cnt.end()) count += cnt[sum_val - k];
            ++cnt[sum_val];
        }
        return count;
    }
};