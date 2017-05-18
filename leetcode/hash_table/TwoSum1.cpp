
/**
 * Given an array of integers, return indices of the two numbers such that they add up to a specific target.

You may assume that each input would have exactly one solution.
 */

/**
 * Solution:
 * 使用哈希表，建立value到index的映射，然后再一次遍历数组，查看该值的差是否在哈希表中
 */

class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        vector<int> res(2);
        gen_map(nums);
        for (int i = 0; i < nums.size(); ++i)
        {
            auto it = m_val_index.find(target - nums[i]);
            if(it != m_val_index.end() && i != it->second) {
                res[0] = i;
                res[1] = it->second;
            }
        }
        return res;
    }

private:
    unordered_map<int, int> m_val_index;

    void gen_map(const vector<int> &nums) {
        for (int i = 0; i < nums.size(); ++i)
        {
            m_val_index.emplace(nums[i], i);
        }
    }
};