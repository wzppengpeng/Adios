/**
 * Given a Binary Search Tree (BST),
 *  convert it to a Greater Tree such that every key of the original BST is changed to the original key plus sum of all keys greater than the original key in BST.
 *

Example:

Input: The root of a Binary Search Tree like this:
              5
            /   \
           2     13

Output: The root of a Greater Tree like this:
             18
            /   \
          20     13
 */

/**
 * Solution: 使用dfs进行遍历，第一遍计算和，第二遍依次与当前值相减得到新的和，并更新值
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
    TreeNode* convertBST(TreeNode* root) {
        if(!root) return root;
        //first compute the sum
        compute_sum(root);
        //then minus val
        minus_val(root);
        return root;
    }

    void compute_sum(TreeNode* root) {
        if(!root) return;
        compute_sum(root->left);
        tree_sum += root->val;
        compute_sum(root->right);
    }

    void minus_val(TreeNode* root) {
        if(!root) return;
        minus_val(root->left);
        auto new_tree_sum = tree_sum - root->val;
        root->val = tree_sum;
        tree_sum = new_tree_sum;
        minus_val(root->right);
    }

    int tree_sum = 0;

};