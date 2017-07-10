/**
 * Given a non-empty binary tree, return the average value of the nodes on each level in the form of an array.

Example 1:
Input:
    3
   / \
  9  20
    /  \
   15   7
Output: [3, 14.5, 11]
Explanation:
The average value of nodes on level 0 is 3,  on level 1 is 14.5, and on level 2 is 11. Hence return [3, 14.5, 11].
 */

/**
 * 使用BFS即可，但是注意求和的时候溢出的问题
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
    vector<double> averageOfLevels(TreeNode* root) {
        queue<TreeNode*> que;
        que.push(root);
        int que_size = 1, level_num = 1;
        long long sum_val = 0;
        vector<double> res;
        while(!que.empty()) {
            auto tmp = que.front();
            que.pop();
            --que_size;
            if(tmp->left) que.push(tmp->left);
            if(tmp->right) que.push(tmp->right);
            sum_val += tmp->val;
            if(que_size == 0) {
                res.emplace_back(static_cast<double>(sum_val) / static_cast<double>(level_num));
                sum_val = 0;
                que_size = que.size();
                level_num = que_size;
            }
        }
        return res;
    }
};