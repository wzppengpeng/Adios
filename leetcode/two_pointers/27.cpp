/**
 * Given an array and a value, remove all instances of that value in place and return the new length.

Do not allocate extra space for another array, you must do this in place with constant memory.

The order of elements can be changed. It doesn't matter what you leave beyond the new length.

Example:
Given input array nums = [3,2,2,3], val = 3

Your function should return length = 2, with the first two elements of nums being 2.
 */


/**
 * 使用双指针，重点在于一头一尾，将尾部不等的和头部等的交换，最后返回时关注尾指针的值
 */

class Solution {
public:
    int removeElement(vector<int>& nums, int val) {
        if(nums.empty()) return 0;
        int i = 0, j = nums.size() - 1;
        while(i < j) {
            if(nums[i] == val) {
                //swap
                while(i < j && nums[j] == val) --j;
                swap(nums[i], nums[j--]);
            }
            ++i;
        }
        return nums[j] == val ? j : j + 1;
    }
};