/**
 * Given a binary tree, you need to compute the length of the diameter of the tree.
 *  The diameter of a binary tree is the length of the longest path between any two nodes in a tree.
 *  This path may or may not pass through the root.
 */

/**
 * Solution:依次对每一个点的左右子树进行dfs得到深度，那么这个点的直径就是左右深度和，然后依次从头开始查找最大值
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
    int diameterOfBinaryTree(TreeNode* root) {
        walk(root);
        return max_diameter;
    }

    void walk(TreeNode* root) {
        if(!root) return;
        max_diameter = std::max(max_diameter, compute_diameter(root));
        walk(root->left);
        walk(root->right);
    }

    int compute_diameter(TreeNode* work) {
        if(!work) return 0;
        return length(work->left) + length(work->right);
    }

    int length(TreeNode* work) {
        if(!work) return 0;
        return 1 + max(length(work->left), length(work->right));
    }

    int max_diameter = 0;
};