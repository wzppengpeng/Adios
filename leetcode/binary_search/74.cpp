/**
 * Write an efficient algorithm that searches for a value in an m x n matrix. This matrix has the following properties:

Integers in each row are sorted from left to right.
The first integer of each row is greater than the last integer of the previous row.
For example,

Consider the following matrix:

[
  [1,   3,  5,  7],
  [10, 11, 16, 20],
  [23, 30, 34, 50]
]
Given target = 3, return true.
 */


/**
 * 首先排除空的情况，然后遍历矩阵的行，只有当该行的区间可能包含target才使用二分搜索
 */


class Solution {
public:
    bool searchMatrix(vector<vector<int>>& matrix, int target) {
        if(matrix.empty() || matrix.front().empty()) return false;
        for(size_t i = 0; i < matrix.size(); ++i) {
            if(target == matrix[i].front() || target == matrix[i].back()) return true;
            if(target > matrix[i].front() && target < matrix[i].back()) {
                return std::binary_search(std::begin(matrix[i]), std::end(matrix[i]), target);
            }
        }
        return false;
    }
};