/**
 * Given an unsorted array of integers, find the number of longest increasing subsequence.

Example 1:
Input: [1,3,5,4,7]
Output: 2
Explanation: The two longest increasing subsequence are [1, 3, 4, 7] and [1, 3, 5, 7].
 */

/**
 * 这道动态规划问题，需要记录两个值，一是到当前位置的最长序列长度，第二是其个数（相当于路径数目）
 * 在更新的时候注意每次更新需要将路径进行求和更新
 * 最后找到最大长度的路径总数
 * 整个算法复杂度为O(n*n)
 */


class Solution {
public:
    int findNumberOfLIS(vector<int>& nums) {
        if(nums.empty()) return 0;
        vector<pair<int, int>> max_cnt(nums.size(), {1, 1});
        for(size_t i = 1; i < nums.size(); ++i) {
            for(size_t j = 0; j < i; ++j) {
                if(nums[i] > nums[j]) {
                    if(1 + max_cnt[j].first > max_cnt[i].first) {
                        max_cnt[i] = std::make_pair(1 + max_cnt[j].first, max_cnt[j].second);
                    } else if(1 + max_cnt[j].first == max_cnt[i].first) {
                        max_cnt[i].second += max_cnt[j].second;
                    }
                }
            }
        }
        map<int, int, greater<int>> cnt;
        for(auto& p : max_cnt) {
            // print(p.first, p.second);
            if(cnt.find(p.first) != cnt.end()) {
                cnt[p.first] += p.second;
            } else {
                cnt.emplace(p.first, p.second);
            }
        }
        return cnt.begin()->second;
    }
};