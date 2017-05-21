/**
 * Given a collection of distinct numbers, return all possible permutations.
 */

/**
 * 原本的方法应该是确定初始状态，搜索空间为一维向量的回溯问题
 * 不过简单一点的话可以使用std::next_permutation搞定，该函数就是求区间的全排列，十分方便
 */


class Solution {
public:
    vector<vector<int>> permute(vector<int>& nums) {
        std::sort(begin(nums), end(nums));
        vector<vector<int>> res;
        do {
            res.emplace_back(nums);
        } while(std::next_permutation(std::begin(nums), std::end(nums)));
        return res;
    }
};