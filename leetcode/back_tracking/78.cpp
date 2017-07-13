/**
 * Given a set of distinct integers, nums, return all possible subsets.

Note: The solution set must not contain duplicate subsets.

For example,
If nums = [1,2,3], a solution is:

[
  [3],
  [1],
  [2],
  [1,2,3],
  [1,3],
  [2,3],
  [1,2],
  []
]
 */

/**
 * 基本思路其实是基于77.cpp的
 * forward中构建要求的不同数组长度下的子集，仍然使用先确定起点向下装入数字的方法，然后一个外层循环确定不同的起点
 * 然后一个外层循环生成1~nums.size()大小的子集
 */


class Solution {
public:
    vector<vector<int>> subsets(vector<int>& nums) {
        if(nums.empty()) return res;
        for(size_t i = 1; i <= nums.size(); ++i) forward(nums, i);
        return res;
    }

private:
    vector<vector<int>> res {{}};

    void forward(const vector<int>& nums, size_t k) {
        for(size_t i = 0; i <= nums.size() - k; ++i) {
            back(nums, k, i, vector<int>());
        }
    }

    void back(const vector<int>& nums, size_t k, size_t next_index, vector<int> sub) {
        if(next_index >= nums.size()) return;
        sub.emplace_back(nums[next_index]);
        if(sub.size() == k) {
            res.emplace_back(sub);
            return;
        }
        // next level
        for(size_t i = next_index + 1; i <= nums.size() - k + sub.size(); ++i) {
            back(nums, k, i, sub);
        }
    }

};