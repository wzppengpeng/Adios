/**
 * Given a non-empty special binary tree consisting of nodes with the non-negative value, where each node in this tree has exactly two or zero sub-node. If the node has two sub-nodes, then this node's value is the smaller value among its two sub-nodes.

Given such a binary tree, you need to output the second minimum value in the set made of all the nodes' value in the whole tree.

If no such second minimum value exists, output -1 instead.
 */

/**
 * 向左子树和右子树分别寻找第一个比预设最小值大的数，并根据返回值的大小来决定最终的返回值
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
    int findSecondMinimumValue(TreeNode* root) {
        if(!root) return -1;
        int first_min = root->val;
        return find(root, first_min);
    }

    int find(TreeNode* root, int first_min) {
        if(!root) return -1;
        int left_min = -1, right_min = -1;
        if(root->left) {
            if(root->left->val > first_min) left_min = root->left->val;
            else left_min = find(root->left, first_min);
        }
        if(root->right) {
            if(root->right->val > first_min) right_min = root->right->val;
            else right_min = find(root->right, first_min);
        }
        if(left_min == first_min && right_min == first_min) return -1;
        else if(left_min == -1) return right_min;
        else if(right_min == -1) return left_min;
        else return min(left_min, right_min);
    }

};