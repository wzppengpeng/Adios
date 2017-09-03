/**
 * Given a binary tree, return the level order traversal of its nodes' values. (ie, from left to right, level by level).

For example:
Given binary tree [3,9,20,null,null,15,7],
    3
   / \
  9  20
    /  \
   15   7
return its level order traversal as:
[
  [3],
  [9,20],
  [15,7]
]
**/


/**
 * 最典型的使用队列进行BFS的过程，队列控制树节点，一个数字控制到哪一个level
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
    vector<vector<int>> levelOrder(TreeNode* root) {
        vector<vector<int>> res;
        if(!root) return res;
        vector<int> row;
        queue<TreeNode*> que;
        que.push(root);
        int num = 1;
        while(!que.empty()) {
            auto tmp = que.front();
            que.pop();
            --num;
            row.emplace_back(tmp->val);
            if(tmp->left) que.push(tmp->left);
            if(tmp->right) que.push(tmp->right);
            if(num == 0) {
                res.emplace_back(row);
                row.clear();
                num = que.size();
            }
        }
        return res;
    }
};