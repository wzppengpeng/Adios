/**
 * Given an array consists of non-negative integers, your task is to count the number of triplets chosen from the array that can make triangles if we take them as side lengths of a triangle.

Example 1:
Input: [2,2,3,4]
Output: 3
Explanation:
Valid combinations are:
2,3,4 (using the first 2)
2,3,4 (using the second 2)
2,2,3
 */


/**
 * 首先搞成有顺序的（STL的sort很快）
 * 在有顺序的情况下，使用two point，确定两个指针确定的位置的作为较短边
 * 内层在寻找当中结合到有序的特点使用二分查找，得到索引距离，即当前短边情况下的长边的上限，相减即个数
 * 整个复杂度平均 (n^2logn)
 */


class Solution {
public:
    int triangleNumber(vector<int>& nums) {
        if(nums.size() < 3) return 0;
        std::sort(std::begin(nums), std::end(nums));
        int res = 0;
        for(size_t i = 0; i < nums.size() - 2; ++i) {
            for(size_t j = i + 1; j < nums.size() - 1; ++j) {
                int max_val = nums[i] + nums[j];
                auto end_iter = std::lower_bound(std::begin(nums) + j + 1, std::end(nums), max_val);
                size_t end_pos = end_iter - std::begin(nums);
                res += static_cast<int>(end_pos - j - 1);
            }
        }
        return res;
    }
};