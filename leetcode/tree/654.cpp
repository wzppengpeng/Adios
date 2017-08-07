/**
 * Given an integer array with no duplicates. A maximum tree building on this array is defined as follow:

The root is the maximum number in the array.
The left subtree is the maximum tree constructed from left part subarray divided by the maximum number.
The right subtree is the maximum tree constructed from right part subarray divided by the maximum number.
Construct the maximum tree by the given array and output the root node of this tree.
 */

/**
 * 使用max_element来寻找区间中的最大值的迭代器，使用一组迭代器来控制区间范围
 * 使用二分查找类似的代码逻辑来完成代码
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
    TreeNode* constructMaximumBinaryTree(vector<int>& nums) {
        TreeNode* root = nullptr;
        construct(nums, 0, nums.size(), root);
        return root;
    }


private:
    void construct(const vector<int>& nums, int i, int j, TreeNode*& work) {
        if(i >= j) return;
        int loc = (std::max_element(nums.begin() + i, nums.begin() + j)) - nums.begin();
        work = new TreeNode(nums[loc]);
        construct(nums, i, loc, work->left);
        construct(nums, loc + 1, j, work->right);
    }

};