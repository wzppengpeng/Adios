/**
 * Given a binary search tree and the lowest and highest boundaries as L and R, trim the tree so that all its elements lies in [L, R] (R >= L). You might need to change the root of the tree, so the result should return the new root of the trimmed binary search tree.

Example 1:
Input: 
    1
   / \
  0   2

  L = 1
  R = 2

Output: 
    1
      \
       2
 */

/**
 * 重点是当前输入节点在不在范围内，如果在范围内就保留，然后分别处理左右子树
 * 如果不在的话，则根据小于最小值或大于最大值判断返回的节点是该出现在左子树还是右子树
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
    TreeNode* trimBST(TreeNode* root, int L, int R) {
        if(!root) return nullptr;
        TreeNode* head;
        if(root->val >= L && root->val <= R) {
            head = root;
            head->left = trimBST(head->left, L, R);
            head->right = trimBST(head->right, L, R);
        } else if(root->val < L) {
            head = trimBST(root->right, L, R);
        } else {
            head = trimBST(root->left, L, R);
        }
        return head;
    }
};