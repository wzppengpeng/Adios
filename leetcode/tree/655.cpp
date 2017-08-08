/**
 * Print a binary tree in an m*n 2D string array following these rules:

The row number m should be equal to the height of the given binary tree.
The column number n should always be an odd number.
The root node's value (in string format) should be put in the exactly middle of the first row it can be put. The column and the row where the root node belongs will separate the rest space into two parts (left-bottom part and right-bottom part). You should print the left subtree in the left-bottom part and print the right subtree in the right-bottom part. The left-bottom part and the right-bottom part should have the same size. Even if one subtree is none while the other is not, you don't need to print anything for the none subtree but still need to leave the space as large as that for the other subtree. However, if two subtrees are none, then you don't need to leave space for both of them.
Each unused space should contain an empty string "".
Print the subtrees following the same rules.
Example 1:
Input:
     1
    /
   2
Output:
[["", "1", ""],
 ["2", "", ""]]
Example 2:
Input:
     1
    / \
   2   3
    \
     4
Output:
[["", "", "", "1", "", "", ""],
 ["", "2", "", "", "", "3", ""],
 ["", "", "4", "", "", "", ""]]
 */


/**
 * 先得到高度，在得到宽度，这样使用类似二分查找的方法，将数字全部安置在中间位置即可
 */


class Solution {
public:
    vector<vector<string>> printTree(TreeNode* root) {
        int h = get_height(root), w = get_width(root);
        vector<vector<string>> ans(h, vector<string>(w, ""));
        helper(ans, root, 0, 0, w-1);
        return ans;
    }
private:
    int get_height(TreeNode* p) {
        if (!p) return 0;
        int left = get_height(p->left), right = get_height(p->right);
        return max(left, right)+1;
    }
    // width is the max(left, right)*2+1
    int get_width(TreeNode* p) {
        if (!p) return 0;
        int left = get_width(p->left), right = get_width(p->right);
        return max(left, right)*2+1;
    }
    // always put the value in the middle of the range.
    void helper(vector<vector<string>>& ans, TreeNode* p, int level, int l, int r) {
        if (!p) return;
        int mid = l+(r-l)/2;
        ans[level][mid] = to_string(p->val);
        helper(ans, p->left, level+1, l, mid-1);
        helper(ans, p->right, level+1, mid+1, r);
    }
};