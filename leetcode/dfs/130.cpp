/**
 * Given a 2D board containing 'X' and 'O' (the letter O), capture all regions surrounded by 'X'.

A region is captured by flipping all 'O's into 'X's in that surrounded region.

For example,
X X X X
X O O X
X X O X
X O X X
After running your function, the board should be:

X X X X
X X X X
X X X X
X O X X
 */


/**
 * 首先使用dfs对所有边界上出发的O将其特殊标记
 * 然后循环遍历，使得特殊标记的变回O，其他没有特殊标记的变为X
 * 逻辑就是不会被包围的一定起源于四条边
 * check函数写j > 1，是有一个比较特殊的case
 */


class Solution {
public:
    void solve(vector<vector<char>>& board) {
        if(board.empty()) return;
        row_ = board.size();
        col_ = board.front().size();
        for(int i = 0; i < row_; ++i) {
            check(board, i, 0);
            if(col_ > 1) check(board, i, col_ - 1);
        }
        for(int j = 1; j < col_ - 1; ++j) {
            check(board, 0, j);
            if(row_ > 1) check(board, row_ - 1, j);
        }
        for(auto& row : board) {
            for(auto& ch : row) {
                if(ch == 'O') ch = 'X';
                else if(ch == 'P') ch = 'O';
            }
        }
    }

    int row_;
    int col_;

    void check(vector<vector<char>>& board, int i, int j) {
        if(board[i][j] == 'O') {
            board[i][j] = 'P';
            if(i > 0) check(board, i - 1, j);
            if(j > 1) check(board, i, j - 1);
            if(i < row_ - 1) check(board, i + 1, j);
            if(j < col_ - 1) check(board, i, j + 1);
        }
    }

};