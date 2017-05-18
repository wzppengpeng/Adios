/**
 * Given a binary tree, find the leftmost value in the last row of the tree.
 */

/**
 * Solution: bfs use queue
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
    int findBottomLeftValue(TreeNode* root) {
        int leftmost = root->val;
        queue<TreeNode*> que;
        que.push(root);
        int num = 1;
        while(!que.empty()) {
            auto tmp = que.front();
            que.pop();
            --num;
            if(tmp->left) {
                que.push(tmp->left);
            }
            if(tmp->right) {
                que.push(tmp->right);
            }
            if(num == 0) {
                if(!que.empty())
                    leftmost = que.front()->val;
                num = que.size();
            }
        }
        return leftmost;
    }
};