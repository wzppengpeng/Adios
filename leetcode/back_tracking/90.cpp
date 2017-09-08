/**
 * Given a collection of integers that might contain duplicates, nums, return all possible subsets.
 * Note: The solution set must not contain duplicate subsets.
 */

/**
 * 首先需要对数组进行排序，这样好区分重复的情况
 * 之后进行回溯迭代
 * 将当前位置的数据添加后加入到结果中，之后直接向下迭代
 * 回溯之后去掉重复元素的方法向下迭代
 */

class Solution {
public:
    vector<vector<int>> subsetsWithDup(vector<int>& nums) {
        vector<vector<int>> res;
        res.emplace_back(vector<int>());
        if(nums.empty()) return res;
        std::sort(begin(nums), end(nums));
        back(nums, 0, {}, res);
        return res;
    }

    void back(const vector<int>& nums, int i, vector<int> tmp, vector<vector<int>>& res) {
        if(i == nums.size()) return;
        tmp.emplace_back(nums[i]);
        res.emplace_back(tmp);
        back(nums, i + 1, tmp, res);
        tmp.pop_back();
        int j = i + 1;
        while(j < nums.size() && nums[j] == nums[i]) ++j;
        if(j < nums.size()) back(nums, j, tmp, res);
    }
};

