/**
 * Given n pairs of parentheses,
 *  write a function to generate all combinations of well-formed parentheses.

For example, given n = 3, a solution set is:

[
  "((()))",
  "(()())",
  "(())()",
  "()(())",
  "()()()"
]
 */


/**
 * 回溯算法，重点要找到搜索空间（数据结构），这里就是一种数结构，按照dfs进行遍历，注意判断左右子树是否存在
 */


class Solution {
public:
    vector<string> generateParenthesis(int n) {
        vector<string> res;
        if(n == 0) return res;
        length = 2 * n;
        string parenthesi(length, '(');
        dfs(0, n, n, parenthesi, res, '(');
        return res;
    }

private:
    int length;

private:

    void dfs(int i, int left_num, int right_num, string& parenthesi, vector<string>& res, char ch) {
        //set the pos i->ch
        if(ch == '(') --left_num;
        else --right_num;
        parenthesi[i] = ch;
        //go left
        if(left_num > 0) {
            dfs(i+1, left_num, right_num, parenthesi, res, '(');
        }
        if(right_num > 0 && right_num > left_num) {
            dfs(i+1, left_num, right_num, parenthesi, res, ')');
        }
        if(left_num == 0 && right_num == 0) {
            res.emplace_back(parenthesi);
        }
    }
};