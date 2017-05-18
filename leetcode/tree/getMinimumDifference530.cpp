
/**
 * Given a binary search tree with non-negative values,
 *  find the minimum absolute difference between values of any two nodes.

Example:

Input:

   1
    \
     3
    /
   2

Output:
1

Explanation:
The minimum absolute difference is 1, which is the difference between 2 and 1 (or between 2 and 3).
 */


/**
 * Solution:每个节点的遍历，当前节点最接近的点往左是一直右，往右是一直左
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
    int getMinimumDifference(TreeNode* root) {
        if(!root) return 0;
        walk(root);
        return m_min_val;
    }

    int get_left(TreeNode* work) {
        if(!work->right) {
            return work->val;
        }
        else {
            return get_left(work->right);
        }
    }

    int get_right(TreeNode* work) {
        if(!work->left) {
            return work->val;
        }
        else {
            return get_right(work->left);
        }
    }

    int each(TreeNode* work) {
        int min_val = INT_MAX;
        if(work->left) {
            min_val = min(min_val, abs(get_left(work->left)-(int)work->val));
        }
        if(work->right) {
            min_val = min(min_val, abs(get_right(work->right))-(int)work->val);
        }
        return min_val;
    }

    void walk(TreeNode* root) {
        if(!root->left && !root->right) {
            return;
        }
        m_min_val = min(m_min_val, each(root));
        if(root->left) walk(root->left);
        if(root->right) walk(root->right);
    }

private:
    int m_min_val = INT_MAX;
};