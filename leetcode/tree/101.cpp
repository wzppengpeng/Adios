/**
 * Given a binary tree, check whether it is a mirror of itself (ie, symmetric around its center).

For example, this binary tree [1,2,2,3,4,4,3] is symmetric:

    1
   / \
  2   2
 / \ / \
3  4 4  3
 */


/**
 * 其实贺判断树是否一样有点类似的思路
 * 用一个茯砖函数来判断两个树节点的情况，同时在dfs时判断应该是左节点的右子树和右节点的左子树比较
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
    bool isSymmetric(TreeNode* root) {
        if(!root) return true;
        return is_symmetric(root->left, root->right);
    }

    bool is_symmetric(TreeNode* left, TreeNode* right) {
        if(left == nullptr && right == nullptr) return true;
        if((left == nullptr && right != nullptr) || (left != nullptr && right == nullptr)) return false;
        if(left->val != right->val) return false;
        return is_symmetric(left->left, right->right) && is_symmetric(left->right, right->left);
    }
};