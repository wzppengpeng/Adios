/**
 * You need to construct a string consists of parenthesis and integers from a binary tree with the preorder traversing way.

The null node needs to be represented by empty parenthesis pair "()".
And you need to omit all the empty parenthesis pairs that don't affect the one-to-one mapping relationship between the string and the original binary tree.
 */


/**
 * solution:使用dfs,注意判断是否是左节点即可
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
    string tree2str(TreeNode* t) {
        if(!t) return "";
        string res;
        dfs(t, res);
        return res;
    }


    void dfs(TreeNode* t, string& res) {
        res.append(std::to_string(t->val));
        //left
        if((!t->left) && (!t->right)) return;
        //left
        res.push_back('(');
        if(t->left) {
            dfs(t->left, res);
        }
        res.push_back(')');
        if(t->right) {
            res.push_back('(');
            dfs(t->right, res);
            res.push_back(')');
        }
    }
};