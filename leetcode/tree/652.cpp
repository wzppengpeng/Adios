/**
 * Given a binary tree, return all duplicate subtrees. For each kind of duplicate subtrees, you only need to return the root node of any one of them.

Two trees are duplicate if they have the same structure with same node values.

Example 1:
        1
       / \
      2   3
     /   / \
    4   2   4
       /
      4
The following are two duplicate subtrees:
      2
     /
    4
and
    4
Therefore, you need to return above trees' root in the form of a list.
 */

/**
 * 这个问题是找相同的子树，可以使用将数进行序列化（字符串）的方法，来存储字符串
 * 这样就可以使用哈希表来存储了，这里注意一点，使用DFS的方法来遍历，同时使用后序遍历
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
    vector<TreeNode*> findDuplicateSubtrees(TreeNode* root) {
        vector<TreeNode*> res;
        dfs(root, &res);
        return res;
    }

private:
    unordered_map<string, int> m_cnt;

    //transpose the tree to string, use hash tables
    string dfs(TreeNode* work, vector<TreeNode*>* res) {
        if(!work) return "*";
        auto id = dfs(work->left, res) + dfs(work->right, res) + std::to_string(work->val);
        if(m_cnt.find(id) != m_cnt.end()) {
            if(m_cnt[id] == 1) {
                res->emplace_back(work);
                ++m_cnt[id];
            }
        } else {
            m_cnt.emplace(id, 1);
        }
        return id;
    }

};