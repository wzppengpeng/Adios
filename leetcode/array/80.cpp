/**
 * Follow up for "Remove Duplicates"(26):
What if duplicates are allowed at most twice?

For example,
Given sorted array nums = [1,1,1,2,2,3],

Your function should return length = 5, with the first five elements of nums being 1, 1, 2, 2 and 3. It doesn't matter what you leave beyond the new length.
 */


/**
 * 仍然使用erase，注意使用两个临时变量记录前一个的值和个数
 */

class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
        if(nums.size() < 3) return nums.size();
        int before = nums[0], num = 1;
        for(auto it = nums.begin() + 1; it != nums.end(); ) {
            if(*it == before) {
                if(num == 1) {
                    ++num;
                    ++it;
                } else {
                    nums.erase(it);
                }
            } else {
                num = 1;
                before = *it;
                ++it;
            }
        }
        return nums.size();
    }
};