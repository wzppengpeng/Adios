/**
 * Given a matrix of m x n elements (m rows, n columns), return all elements of the matrix in spiral order.

For example,
Given the following matrix:

[
 [ 1, 2, 3 ],
 [ 4, 5, 6 ],
 [ 7, 8, 9 ]
]
You should return [1,2,3,6,9,8,7,4,5]
 */

/**
 * 一个非常高效的答案，使用循环，不用递归
 * 每一个过程都是先右，再下，再左，最后上
 * 所以写一个这个过程的辅助函数，但是在这个过程中随时注意判断是否已经到了该极的重点位置，使用返回值来控制是否已经到达最后的位置
 */



class Solution {
public:
    vector<int> spiralOrder(vector<vector<int>>& matrix) {
        if(matrix.empty()) return res;
        m = matrix.size();
        n = matrix.front().size();
        int i = 0, j = 0, l = 1;
        while(walk(i++, j++, l++, matrix)) {}
        return res;
    }

    vector<int> res;
    int m;
    int n;

    bool walk(int i, int j, int level, vector<vector<int>>& matrix) {
        int right_end = n - level;
        int botton_end = m - level;
        int left_end = level - 1;
        int up_end = level;
        //walk right
        if(j > right_end) return false;
        while(j <= right_end) res.emplace_back(matrix[i][j++]);
        --j;
        //walk down
        if(i >= botton_end) return false;
        while(i < botton_end) res.emplace_back(matrix[++i][j]);
        //walk left
        if(j <= left_end) return false;
        while(j > left_end) res.emplace_back(matrix[i][--j]);
        if(i <= up_end) return false;
        //walk up
        while(i > up_end) res.emplace_back(matrix[--i][j]);
        return true;
    }
};