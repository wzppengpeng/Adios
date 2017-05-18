/**
 * Given a binary tree, return the tilt of the whole tree.

The tilt of a tree node is defined as the absolute difference between the sum of all left subtree node values and the sum of all right subtree node values. Null node has tilt 0.

The tilt of the whole tree is defined as the sum of all nodes' tilt.
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

/**
 * Solution:dfs向下，使用同一变量求和
 */

class Solution {
public:
    int findTilt(TreeNode* root) {
        int sum_val = 0;
        dfs(root, sum_val);
        return sum_val;
    }


    int dfs(TreeNode* work, int& sum_val) {
        if(!work) return 0;
        //add the tilt
        int left_val = dfs(work->left, sum_val);
        int right_val = dfs(work->right, sum_val);
        sum_val += abs(left_val - right_val);
        return left_val + right_val + work->val;
    }

};