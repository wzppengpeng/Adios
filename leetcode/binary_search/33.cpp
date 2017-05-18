/**
 * Suppose an array sorted in ascending order is rotated at some pivot unknown to you beforehand.

(i.e., 0 1 2 4 5 6 7 might become 4 5 6 7 0 1 2).

You are given a target value to search. If found in the array return its index, otherwise return -1.

You may assume no duplicate exists in the array.
 */

/**
 * 先用二分查找找到顺序出错的位置，然后再在分出来的区间中进行二分查找
 * 其实直接线性查找的时间差不多= =
 */


class Solution {
public:
    int search(vector<int>& nums, int target) {
        if(nums.empty()) return -1;
        int lo = 0, hi = nums.size() - 1;
        while(lo < hi) {
            int mid = (lo + hi) / 2;
            if(nums[mid] > nums[hi]) lo = mid + 1;
            else hi = mid;
        }
        //lo is the smallest val in array
        if(target == nums.back()) return nums.size() - 1;
        else if(target < nums.back()) {
            auto it = lower_bound(nums.begin() + lo, nums.end(), target);
            if(it != nums.end() && *it == target) return std::distance(nums.begin(), it);
            else return -1;
        }
        else {
            auto it = lower_bound(nums.begin(), nums.begin() + lo, target);
            if(it != (nums.begin() + lo) && *it == target) return std::distance(nums.begin(), it);
            else return -1;
        }
    }
};