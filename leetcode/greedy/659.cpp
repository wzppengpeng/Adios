/**
 * You are given an integer array sorted in ascending order (may contain duplicates), you need to split them into several subsequences, where each subsequences consist of at least 3 consecutive integers. Return whether you can make such a split.

Example 1:
Input: [1,2,3,3,4,5]
Output: True
Explanation:
You can split them into two consecutive subsequences :
1, 2, 3
3, 4, 5
Example 2:
Input: [1,2,3,3,4,4,5,5]
Output: True
Explanation:
You can split them into two consecutive subsequences :
1, 2, 3, 4, 5
3, 4, 5
Example 3:
Input: [1,2,3,4,4,5]
Output: False
 */


/**
 * 使用两个哈希表
 * 第一个哈希表来记录当前使用过的数字
 * 第二个哈希表记录当前以x结尾的序列
 * 首先如果该数字可以接在前面的序列上，那么优先将该数字接在前面的序列上，调整结尾数字的记录
 * 如果是新起的数字，则使用贪心算法，优先满足至少三个的条件
 */

class Solution {
public:
    bool isPossible(vector<int>& nums) {
        if(nums.size() < 3) return false;
        unordered_map<int, int> cnt, ends;
        for(auto num : nums) ++cnt[num];
        for(auto num : nums) {
            if(cnt[num] == 0) continue;
            if(ends[num - 1]) {
                --ends[num - 1];
                ++ends[num];
            } else if(cnt[num + 1] > 0 && cnt[num + 2] > 0) {
                --cnt[num + 1];
                --cnt[num + 2];
                ++ends[num + 2];
            } else {
                return false;
            }
            --cnt[num];
        }
        return true;
    }

};