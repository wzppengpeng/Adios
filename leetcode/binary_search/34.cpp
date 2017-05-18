/**
 * Given an array of integers sorted in ascending order, find the starting and ending position of a given target value.

Your algorithm's runtime complexity must be in the order of O(log n).

If the target is not found in the array, return [-1, -1].
 */

/**
 * 就是equal_range的定义，使用STL完成
 */


class Solution {
public:
    vector<int> searchRange(vector<int>& nums, int target) {
        if(nums.empty()) return {-1, -1};
        auto p = equal_range(std::begin(nums), std::end(nums), target);
        if(p.first != p.second) return { p.first - nums.begin(), p.second - nums.begin() - 1 };
        else return {-1, -1};
    }
};