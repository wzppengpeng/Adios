/**
 * Given a 2D integer matrix M representing the gray scale of an image, you need to design a smoother to make the gray scale of each cell becomes the average gray scale (rounding down) of all the 8 surrounding cells and itself. If a cell has less than 8 surrounding cells, then use as many as you can.

Example 1:
Input:
[[1,1,1],
 [1,0,1],
 [1,1,1]]
Output:
[[0, 0, 0],
 [0, 0, 0],
 [0, 0, 0]]
Explanation:
For the point (0,0), (0,2), (2,0), (2,2): floor(3/4) = floor(0.75) = 0
For the point (0,1), (1,0), (1,2), (2,1): floor(5/6) = floor(0.83333333) = 0
For the point (1,1): floor(8/9) = floor(0.88888889) = 0
 */

/**
 * 写一个辅助函数来求近邻的平均值即可，注意边界
 */


class Solution {
public:
    vector<vector<int>> imageSmoother(vector<vector<int>>& M) {
        if(M.empty()) return vector<vector<int>>();
        int m = M.size(), n = M.front().size();
        vector<vector<int>> res(m, vector<int>(n, 0));
        for(int i = 0; i < m; ++i) {
            for(int j = 0; j < n; ++j) {
                res[i][j] = compute(M, i, j, m, n);
            }
        }
        return res;
    }

    int compute(const vector<vector<int>>& M, int i, int j, int m, int n) {
        int fenzi = 0, fenmu = 0;
        for(int ii = i - 1; ii <= i + 1; ++ii) {
            if(ii == -1 || ii == m) continue;
            for(int jj = j - 1; jj <= j + 1; ++jj) {
                if(jj == -1 || jj == n) continue;
                fenzi += M[ii][jj];
                ++fenmu;
            }
        }
        return fenzi / fenmu;
    }
};