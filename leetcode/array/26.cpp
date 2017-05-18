/**
 * Given a sorted array, remove the duplicates in place such that each element appear only once and
 *  return the new length.

    Do not allocate extra space for another array, you must do this in place with constant memory.
 */

/**
 * 使用vector::erase，性能很不错
 */


class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
        //use std::erase
        if(nums.empty()) return 0;
        int now = nums[0];
        for(auto it = nums.begin() + 1; it != nums.end(); ) {
            if(*it == now) {
                nums.erase(it);
            }
            else {
                now = *it;
                ++it;
            }
        }
        return nums.size();
    }
};