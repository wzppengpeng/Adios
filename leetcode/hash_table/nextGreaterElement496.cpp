
/**
 * You are given two arrays (without duplicates) nums1 and nums2 where nums1’s elements are subset of nums2.
 *  Find all the next greater numbers for nums1's elements in the corresponding places of nums2.

The Next Greater Number of a number x in nums1 is the first greater number to its right in nums2.
 If it does not exist, output -1 for this number.
 */

/**
 * Solution:首先使用哈希表保存待搜索数在集合中的索引，然后从索引开始往后寻找更大的数
 */


class Solution {
public:
    vector<int> nextGreaterElement(vector<int>& findNums, vector<int>& nums) {
        gen_indexs(nums);
        vector<int> result(findNums.size());
        for (int i = 0; i < findNums.size(); ++i)
        {
            result[i] = search(nums, findNums[i]);
        }
        return result;
    }

private:
    unordered_map<int, int> indexs;

    inline void gen_indexs(const vector<int>& nums) {
        for (int i = 0; i < nums.size(); ++i)
        {
            indexs.emplace(nums[i], i);
        }
    }

    int search(const vector<int>& nums, int target) {
        auto it = indexs.find(target);
        auto index = it->second;//the search begin location
        if(index++ == nums.size() - 1) return -1;
        for(; index < nums.size(); ++index) {
            if(nums[index] > target) return nums[index];
        }
        return -1;
    }
};