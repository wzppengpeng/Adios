/**
 * Given a binary tree and a sum, find all root-to-leaf paths where each path's sum equals the given sum.

For example:
Given the below binary tree and sum = 22,
              5
             / \
            4   8
           /   / \
          11  13  4
         /  \    / \
        7    2  5   1
return
[
   [5,4,11,2],
   [5,8,4,5]
]
 */


/**
 * 实现思路参考112
 * 唯一多的地方就是使用一个数组保存当前的状态
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
    vector<vector<int>> pathSum(TreeNode* root, int sum) {
        vector<vector<int>> res;
        dfs(root, sum, {}, res);
        return res;
    }

    void dfs(TreeNode* work, int sum, std::vector<int> tmp, vector<vector<int>>& res) {
        if(!work) return;
        tmp.emplace_back(work->val);
        if(!work->left && !work->right) {
            if(sum == work->val) res.emplace_back(tmp);
        } else {
            dfs(work->left, sum - work->val, tmp, res);
            dfs(work->right, sum - work->val, tmp, res);
        }
    }
};