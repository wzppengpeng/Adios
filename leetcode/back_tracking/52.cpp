/**
 * Follow up for N-Queens problem.

Now, instead outputting board configurations, return the total number of distinct solutions.
 */


/**
 * 经典的八皇后问题，要求不再同一行同一列同对角线，解空间为数组，然后根据前面数组记录的情况进行判断递归即可
 */

class Solution {
public:
    int totalNQueens(int n) {
        if(n < 2) return n;
        int res = 0;
        std::vector<int> state;
        for(int j = 0; j < n; ++j) {
            state.emplace_back(j);
            is_ok(1, state, n, res);
            state.pop_back();
        }
        return res;
    }

    void is_ok(int i, vector<int> state, int n, int& res) {
        if(i == n) {
            ++res;
            return;
        }
        for(int j = 0; j < n; ++j) {
            bool can_go = true;
            for(int k = 0; k < i; ++k) {
                if(j == state[k] || (i - k) == (j - state[k]) || (k - i) == (j - state[k])) {
                    can_go = false;
                    break;
                }
            }
            if(can_go) {
                state.emplace_back(j);
                is_ok(i + 1, state, n, res);
                state.pop_back();
            }
        }
    }
};