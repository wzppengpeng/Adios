/**
 * Given a binary tree, return the inorder traversal of its nodes' values.

For example:
Given binary tree [1,null,2,3],
   1
    \
     2
    /
   3
return [1,3,2].
 */

/**
 * 典型的dfs递归，需要注意中序遍历是先左再中再右
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
    vector<int> inorderTraversal(TreeNode* root) {
        vector<int> res;
        inorder(root, res);
        return res;
    }


    void inorder(TreeNode* work, vector<int>& res) {
        if(!work) return;
        inorder(work->left, res);
        res.emplace_back(work->val);
        inorder(work->right, res);
    }


};