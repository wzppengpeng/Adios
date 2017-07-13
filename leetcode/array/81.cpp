/**
 * Suppose an array sorted in ascending order is rotated at some pivot unknown to you beforehand.

(i.e., 0 1 2 4 5 6 7 might become 4 5 6 7 0 1 2).

Write a function to determine if a given target is in the array.

The array may contain duplicates.
 */

/**
 * 事实证明直接O（N）搜索更快
 */


class Solution {
public:
    bool search(vector<int>& nums, int target) {
        for(auto num : nums) {
            if(num == target) return true;
        }
        return false;
    }
};