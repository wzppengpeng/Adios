/**
 * Given an integer array, you need to find one continuous subarray that if you only sort this subarray in ascending order, then the whole array will be sorted in ascending order, too.

You need to find the shortest such subarray and output its length.
 */

/**
 * 其实搞个副本直接排序一遍然后找前后相同的位置即可
 */

class Solution {
public:
    int findUnsortedSubarray(vector<int>& nums) {
        if(nums.size() < 2) return 0;
        auto new_num = nums;
        std::sort(begin(new_num), end(new_num));
        int i = 0, j = new_num.size() - 1;
        while(i < j && new_num[i] == nums[i]) ++i;
        while(i < j && new_num[j] == nums[j]) --j;
        return i == j ? 0 : j - i + 1;
    }
};