/**
 * Given an array with n objects colored red, white or blue, sort them so that objects of the same color are adjacent, with the colors in the order red, white and blue.

Here, we will use the integers 0, 1, and 2 to represent the color red, white, and blue respectively.
 */

/**
 * 看似是排序问题，但是不允许使用sort库，同时也有更快的方法
 * A rather straight forward solution is a two-pass algorithm using counting sort.
First, iterate the array counting number of 0's, 1's, and 2's, then overwrite array with total number of 0's, then 1's and followed by 2's.
 */


class Solution {
public:
    void sortColors(vector<int>& nums) {
        int zero_num = 0, one_num = 0, two_num = 0;
        for(auto num : nums) {
            if(num == 0) ++zero_num;
            else if(num == 1) ++one_num;
            else ++two_num;
        }
        nums.clear();
        back_insert(nums, zero_num, 0);
        back_insert(nums, one_num, 1);
        back_insert(nums, two_num, 2);
    }

    void back_insert(vector<int>& nums, int num, int target) {
        for(int i = 0; i < num; ++i) {
            nums.emplace_back(target);
        }
    }
};