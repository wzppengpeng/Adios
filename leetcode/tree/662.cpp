/**
 * Given a binary tree, write a function to get the maximum width of the given tree. The width of a tree is the maximum width among all levels. The binary tree has the same structure as a full binary tree, but some nodes are null.

The width of one level is defined as the length between the end-nodes (the leftmost and right most non-null nodes in the level, where the null nodes between the end-nodes are also counted into the length calculation.

Example 1:
Input: 

           1
         /   \
        3     2
       / \     \  
      5   3     9 

Output: 4
Explanation: The maximum width existing in the third level with the length 4 (5,3,null,9).
 */


/**
 * 使用BFS，这里和普通的队列处理BFS的区别在于使用一个int来表示该节点在这一层的index
 */

class Solution {
public:
    int widthOfBinaryTree(TreeNode* root) {
        if(!root) return 0;
        queue<pair<TreeNode*, int>> que;
        que.push({root, 0});
        int num = 1, begin = 0, end = 0, max_width = 0, level = 1;
        pair<TreeNode*, int> temp;
        while(!que.empty()) {
            //the begining
            temp = que.front();
            if(num == level) begin = temp.second;
            que.pop();
            --num;
            //add new nodes
            if(temp.first->left) que.push({temp.first->left, temp.second * 2});
            if(temp.first->right) que.push({temp.first->right, temp.second * 2 + 1});
            if(num == 0) {
                end = temp.second;
                max_width = std::max(max_width, end - begin + 1);
                num = que.size();
                level = num;
            }
        }
        return max_width;
    }
};