/**
 * You need to find the largest value in each row of a binary tree.
 */

/**
 * Solution : bfs use queue
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
    vector<int> largestValues(TreeNode* root) {
        vector<int> result;
        if(!root) return result;
        result.reserve(500);// little trick to optimize the runtime
        queue<TreeNode*> que;
        int num = 1;
        int tmp_largest = root->val;
        que.push(root);
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
            //handle the largest
            tmp_largest = max(tmp_largest, tmp->val);
            if(num == 0) {
                result.push_back(tmp_largest);
                num = que.size();
                if(!que.empty())
                    tmp_largest = que.front()->val;
            }
        }
        return result;
    }

};