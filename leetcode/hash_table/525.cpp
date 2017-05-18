/**
 * Given a binary array, find the maximum length of a contiguous subarray with equal number of 0 and 1.
 */


/**
 * hash table:第一，仍然是使用求和可以表示区间；第二，初始位置可以考虑一个index为（-1）的位置
 * 第三，这里将为0的情况化为-1，这样当求和与前面相等的情况出现，说明出现了一个区间总体和为零，使用哈希表来保存坐标。
 */


class Solution {
public:
    int findMaxLength(vector<int>& nums) {
        if(nums.size() < 2) return 0;
        unordered_map<int, int> sums {{0, -1}};
        int sum = 0, len = 0;
        for(int i = 0; i < nums.size(); ++i) {
            sum += nums[i] ? 1 : -1;
            if(sums.find(sum) != sums.end()) {
                len = max(len, i - sums[sum]);
            }
            else sums.emplace(sum, i);
        }
        return len;
    }
};