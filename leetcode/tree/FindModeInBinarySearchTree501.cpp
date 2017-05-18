/**
 * Given a binary search tree (BST) with duplicates,
 *  find all the mode(s) (the most frequently occurred element) in the given BST.
 */

/**
 * Solution:中序遍历得到数组，然后便利统计出来的数据，得到最大值
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
    vector<int> findMode(TreeNode* root) {
        vector<int> res;
        if(!root) return res;
        walk(root);
        get_max_cnt(res);
        return res;
    }

    void walk(TreeNode* work) {
        if(work->left) {
            walk(work->left);
        }
        insert_one_val(work->val);
        if(work->right) {
            walk(work->right);
        }
    }

    inline void insert_one_val(int val) {
        auto it = nums.find(val);
        if(it != nums.end()) {
            ++it->second;
        }
        else {
            nums.emplace(val, 1);
        }
    }

    void get_max_cnt(vector<int>& res) {
        int max_cnt = 0;
        for(auto& p : nums) {
            if(p.second > max_cnt) {
                res.clear();
                res.push_back(p.first);
                max_cnt = p.second;
            }
            else if(p.second == max_cnt) {
                res.push_back(p.first);
            }
        }
    }

private:
    unordered_map<int, int> nums;
};