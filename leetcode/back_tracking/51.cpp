/**
 * The n-queens puzzle is the problem of placing n queens on an n×n chessboard such that no two queens attack each other.
 * Given an integer n, return all distinct solutions to the n-queens puzzle.

Each solution contains a distinct board configuration of the n-queens' placement, where 'Q' and '.' both indicate a queen and an empty space respectively.
 */

/**
 * same with 52, but need to print the result
 */

class Solution {
public:
    vector<vector<string>> solveNQueens(int n) {
        vector<vector<string>> res;
        std::vector<int> state;
        for(int j = 0; j < n; ++j) {
            state.emplace_back(j);
            is_ok(1, state, n, res);
            state.pop_back();
        }
        return res;
    }

    void is_ok(int i, vector<int> state, int n, vector<vector<string>>& res) {
        if(i == n) {
            vector<string> solution(n, string(n, '.'));
            for(int i = 0; i < n; ++i) {
                for(int j = 0; j < n; ++j) {
                    if(state[i] == j) solution[i][j] = 'Q';
                }
            }
            res.emplace_back(std::move(solution));
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