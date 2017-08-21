/**
 * Given a binary tree with n nodes, your task is to check if it's possible to partition the tree to two trees which have the equal sum of values after removing exactly one edge on the original tree.

Example 1:
Input:
    5
   / \
  10 10
    /  \
   2   3

Output: True
Explanation: 
    5
   / 
  10
      
Sum: 15

   10
  /  \
 2    3

Sum: 15
 */

/**
 * 问题关键在于转换思路，切分两个相同和的树，即当前节点的和为总体和的一半，又只需要返回bool，所以不需要记录所有的节点位置的值
 * 只需要一个数组来保存所有向下的节点和，然后遍历数组，当恰好为总体和的一半时，则返回yes
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
    bool checkEqualTree(TreeNode* root) {
        int sum_val = get_tree_node_sum(root);
        if(!root || m_node_sum.size() < 2) return false;
        for(auto val : m_node_sum) {
            if(2 * val == sum_val) return true;
        }
        return false;
    }

    vector<int> m_node_sum;

    int get_tree_node_sum(TreeNode* work) {
        if(!work) return 0;
        int sum_val = get_tree_node_sum(work->left) + work->val + get_tree_node_sum(work->right);
        m_node_sum.emplace_back(sum_val);
        return sum_val;
    }
};