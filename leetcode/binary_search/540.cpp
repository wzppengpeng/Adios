/**
 * Given a sorted array consisting of only integers where every element appears twice except for one element which appears once.
 * Find this single element that appears only once
 */


/**
 * 直接线搜索效率居然最高。。。
 */

class Solution {
public:
    int singleNonDuplicate(vector<int>& nums) {
        if(nums.size() == 1) return nums.front();
        auto before = nums.front();
        bool search_second = true;
        for(size_t i = 1; i < nums.size(); ++i) {
            if(search_second) {
                if(nums[i] != before) return before;
            }
            else {
                before = nums[i];
            }
            search_second = !search_second;
        }
        return before;
    }
};