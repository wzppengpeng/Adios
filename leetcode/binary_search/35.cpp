/**
 * Given a sorted array and a target value, return the index if the target is found. If not, return the index where it would be if it were inserted in order.

You may assume no duplicates in the array
 */

/**
 * 一行使用STL的lower_bound得到该值的插入位置
 */


class Solution {
public:
    int searchInsert(vector<int>& nums, int target) {
        return std::lower_bound(begin(nums), end(nums), target) - nums.begin();
    }
};