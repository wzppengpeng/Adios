/**
 * Given an array of integers and an integer k, you need to find the number of unique k-diff pairs in the array. Here a k-diff pair is defined as an integer pair (i, j), where i and j are both numbers in the array and their absolute difference is k.

Example 1:
Input: [3, 1, 4, 1, 5], k = 2
Output: 2
Explanation: There are two 2-diff pairs in the array, (1, 3) and (3, 5).
Although we have two 1s in the input, we should only return the number of unique pairs.
 */


/**
 * Solution:先排序，然后使用双指针，逐渐向后便利，最后复杂度为 nlogn
 */

class Solution {
public:
    int findPairs(vector<int>& nums, int k) {
        if(nums.size() < 2 || k < 0) return 0;
        std::sort(begin(nums), end(nums));
        int i = 0, j = 1, res = 0;
        while(j < nums.size()) {
            if(nums[j] - nums[i] >= k) {
                if(nums[j] - nums[i] == k) ++res;
                ++i;
                while(i < nums.size() && nums[i] == nums[i - 1]) ++i;
                j = i;
            }
            ++j;
        }
        return res;
    }


};