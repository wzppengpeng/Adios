/**
 * Given a Binary Search Tree and a target number, return true if there exist two elements in the BST such that their sum is equal to the given target.

Example 1:
Input:
    5
   / \
  3   6
 / \   \
2   4   7

Target = 9

Output: True
Example 2:
Input:
    5
   / \
  3   6
 / \   \
2   4   7

Target = 28

Output: False
 */

/**
 * 使用普通的遍历建立哈希表，在哈希表中来处理
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
    bool findTarget(TreeNode* root, int k) {
        dfs(root);
        for(auto& p : m_cnt) {
            auto temp = k - p.first;
            if(temp == p.first) {
                if(p.second > 1) return true;
            } else {
                if(m_cnt.find(temp) != m_cnt.end()) return true;
            }
        }
        return false;
    }

private:
    unordered_map<int, int> m_cnt;

    void dfs(TreeNode* root) {
        if(!root) return;
        dfs(root->left);
        ++m_cnt[root->val];
        dfs(root->right);
    }

};