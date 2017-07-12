/**
 * Given a m x n matrix, if an element is 0, set its entire row and column to 0. Do it in place.
 */

/**
 * 很显然，最快又节省空间的方法就是两次遍历矩阵，将为零的行和列的索引记下来，并且使用哈希表进行去重
 */


class Solution {
public:
    void setZeroes(vector<vector<int>>& matrix) {
        find_row_col(matrix);
        set_zero(matrix);
    }

    unordered_set<int> row_;
    unordered_set<int> col_;

    void find_row_col(const vector<vector<int>>& matrix) {
        for(int i = 0; i < matrix.size(); ++i) {
            for(int j = 0; j < matrix.front().size(); ++j) {
                if(matrix[i][j] == 0) {
                    row_.emplace(i);
                    col_.emplace(j);
                }
            }
        }
    }

    void set_zero_row(vector<vector<int>>& matrix, int i) {
        for(int j = 0; j < matrix.front().size(); ++j) {
            matrix[i][j] = 0;
        }
    }

    void set_zero_col(vector<vector<int>>& matrix, int j) {
        for(int i = 0; i < matrix.size(); ++i) {
            matrix[i][j] = 0;
        }
    }

    void set_zero(vector<vector<int>>& matrix) {
        for(auto i : row_) set_zero_row(matrix, i);
        for(auto j : col_) set_zero_col(matrix, j);
    }

};