/**
 * Given two non-empty binary trees s and t, check whether tree t has exactly the same structure and node values with a subtree of s. A subtree of s is a tree consists of a node in s and all of this node's descendants.
 * The tree s could also be considered as a subtree of itself.
 */

/**
 * Solution:bfs外循环产生子树，dfs判断两个树是否全等，
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
    bool isSubtree(TreeNode* s, TreeNode* t) {
        if(!s) return false;
        queue<TreeNode*> que;
        que.push(s);
        while(!que.empty()) {
            TreeNode* tmp = que.front();
            que.pop();
            if(tmp->left) que.push(tmp->left);
            if(tmp->right) que.push(tmp->right);
            if(is_same(tmp, t)) return true;
        }
        return false;
    }

    bool is_same(TreeNode* s_, TreeNode* t_) {
        if(s_ == nullptr && t_ == nullptr) return true;
        if((s_ != nullptr && t_ == nullptr) || (s_ == nullptr && t_ != nullptr)) return false;
        if(s_->val != t_ ->val) return false;
        return is_same(s_->left, t_->left) && is_same(s_->right, t_->right);
    }
};