/**
 * Given two binary trees and imagine that when you put one of them to cover the other, some nodes of the two trees are overlapped while the others are not.

You need to merge them into a new binary tree. The merge rule is that if two nodes overlap, then sum node values up as the new value of the merged node. Otherwise, the NOT null node will be used as the node of new tree.

Example 1:
Input:
    Tree 1                     Tree 2
          1                         2
         / \                       / \
        3   2                     1   3
       /                           \   \
      5                             4   7
Output:
Merged tree:
         3
        / \
       4   5
      / \   \
     5   4   7
 */


/**
 * solution:遍历，分情况讨论即可
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
    TreeNode* mergeTrees(TreeNode* t1, TreeNode* t2) {
        if(!t1) return t2;
        if(!t2) return t1;
        merge(t1, t2);
        return t1;
    }

    void merge(TreeNode* t1, TreeNode* t2) {
        t1->val += t2->val;
        if(t1->left && t2->left) merge(t1->left, t2->left);
        else if(!t1->left && t2->left) t1->left = t2->left;
        if(t1->right && t2->right) merge(t1->right, t2->right);
        else if(!t1->right && t2->right) t1->right = t2->right;
    }


};