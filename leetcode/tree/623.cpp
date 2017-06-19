/**
 * Given the root of a binary tree, then value v and depth d, you need to add a row of nodes with value v at the given depth d. The root node is at depth 1.

The adding rule is: given a positive integer depth d, for each NOT null tree nodes N in depth d-1, create two tree nodes with value v as N's left subtree root and right subtree root. And N's original left subtree should be the left subtree of the new left subtree root, its original right subtree should be the right subtree of the new right subtree root. If depth d is 1 that means there is no depth d-1 at all, then create a tree node with value v as the new root of the whole original tree, and the original tree is the new root's left subtree.
 */

/**
 * 使用dfs即可，注意初始判断为空和d=1的情况
 * 普通情况则记录当前深度，到d-1时新建节点，并将缓存的节点安插到正确位置即可
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
    TreeNode* addOneRow(TreeNode* root, int v, int d) {
        if(!root) return nullptr;
        if(d == 1) {
            TreeNode* new_root = new TreeNode(v);
            new_root->left = root;
            return new_root;
        }
        add(root, v, d, 1);
        return root;
    }


    void add(TreeNode* work, int v, int d, int now) {
        if(now == d - 1) {
            TreeNode* l = work->left;
            TreeNode* r = work->right;
            TreeNode* left_one = new TreeNode(v);
            TreeNode* right_one = new TreeNode(v);
            work->left = left_one;
            work->right = right_one;
            left_one->left = l;
            right_one->right = r;
        }
        else if(now < d - 1) {
            if(work->left) add(work->left, v, d, now + 1);
            if(work->right) add(work->right, v, d, now + 1);
        }
    }

};