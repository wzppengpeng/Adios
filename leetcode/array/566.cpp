/**
 * You're given a matrix represented by a two-dimensional array, and two positive integers r and c representing the row number and column number of the wanted reshaped matrix, respectively.

The reshaped matrix need to be filled with all the elements of the original matrix in the same row-traversing order as they were.

If the 'reshape' operation with given parameters is possible and legal, output the new reshaped matrix; Otherwise, output the original matrix.
 */



class Solution {
public:
    vector<vector<int>> matrixReshape(vector<vector<int>>& nums, int r, int c) {
        if(nums.empty()) return nums;
        int row = nums.size();
        int col = nums[0].size();
        if(row * col != (r * c)) return nums;
        vector<vector<int> > new_maxtrix(r, vector<int>(c));
        int index = 0;
        for(int i = 0; i < row; ++i) {
            for(int j = 0; j < col; ++j) {
                auto new_loc = idx(index++, c);
                new_maxtrix[new_loc.first][new_loc.second] = nums[i][j];
            }
        }
        return std::move(new_maxtrix);
    }

    inline pair<int, int> idx(int index, int c) {
        pair<int, int> res;
        res.first = index / c;
        res.second = index % c;
        return std::move(res);
    }
};