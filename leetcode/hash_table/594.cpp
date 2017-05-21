/**
 *We define a harmonious array is an array where the difference between its maximum value and its minimum value is exactly 1.

Now, given an integer array, you need to find the length of its longest harmonious subsequence among all its possible subsequences.

Example 1:
Input: [1,3,2,2,5,2,3,7]
Output: 5
Explanation: The longest harmonious subsequence is [3,2,2,2,3].
 */


/**
 * 看似是一个数组序列的问题，但是它表示所有的序列都在找的范围，因此该问题和数组的顺序是没有关系的
 * 使用哈希表统计数字出现次数，然后遍历哈希表，寻找最大的val 和val+1的次数和即可
 */

class Solution {
public:
    int findLHS(vector<int>& nums) {
        unordered_map<int, int> cnt;
        for(auto n : nums) {
            auto it = cnt.find(n);
            if(it != cnt.end()) ++it->second;
            else cnt.emplace(n, 1);
        }
        int max_num = 0, tmp_num = 0;
        for(auto& p : cnt) {
            auto it = cnt.find(1 + p.first);
            tmp_num = p.second;
            if(it != cnt.end()) tmp_num += it->second;
            else tmp_num = 0;
            max_num = max(max_num, tmp_num);
        }
        return max_num;
    }
};