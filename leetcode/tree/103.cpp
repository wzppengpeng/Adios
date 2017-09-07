/**
 * Given a binary tree, return the zigzag level order traversal of its nodes' values. (ie, from left to right, then right to left for the next level and alternate between).

For example:
Given binary tree [3,9,20,null,null,15,7],
    3
   / \
  9  20
    /  \
   15   7
return its zigzag level order traversal as:
[
  [3],
  [20,9],
  [15,7]
]
 */


/**
 * 使用queue进行bfs，用一个Bool控制是否是偶数行，偶数行使用reverse进行翻转
 */



/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
        vector<vector<int>> res;
        if(!root) return res;
        vector<int> row;
        bool is_even = false;
        queue<TreeNode*> que;
        que.push(root);
        int num = 1;
        while(!que.empty()) {
            TreeNode* tmp = que.front();
            que.pop();
            --num;
            row.emplace_back(tmp->val);
            if(tmp->left) que.push(tmp->left);
            if(tmp->right) que.push(tmp->right);
            if(num == 0) {
                num = que.size();
                if(is_even) std::reverse(begin(row), end(row));
                res.emplace_back(row);
                row.clear();
                is_even = !is_even;
            }
        }
        return res;
    }
};