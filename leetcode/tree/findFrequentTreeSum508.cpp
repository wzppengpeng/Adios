/**
 * Given the root of a tree, you are asked to find the most frequent subtree sum.
 *  The subtree sum of a node is defined as the sum of all the node values formed by the subtree rooted at
 *   that node (including the node itself). So what is the most frequent subtree sum value? If there is a tie,
 *    return all the values with the highest frequency in any order.

Examples 1
Input:

  5
 /  \
2   -3
return [2, -3, 4], since all the values happen only once, return all of them in any order.
Examples 2
Input:

  5
 /  \
2   -5
return [2], since 2 happens twice, however -5 only occur once.
 */


/**
 * Solution:深度遍历树，使用一个哈希表来存，之后便利哈希表，找到最大的结果
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
    vector<int> findFrequentTreeSum(TreeNode* root) {
        vector<int> result;
        if(!root) return result;
        gen_cnt(root);
        get_frequent(result);
        return result;
    }

private:
    unordered_map<int, int> sum_count;

    int gen_cnt(TreeNode* work) {
        if(!work) return 0;
        int sum_res = gen_cnt(work->left)
         + gen_cnt(work->right) + work->val;
        auto it = sum_count.find(sum_res);
        if(it != sum_count.end()) {
            ++it->second;
        }
        else {
            sum_count.emplace(sum_res, 1);
        }
        return sum_res;
    }

    void get_frequent(vector<int>& result) {
        int max_frequent = 0;
        for(auto& p : sum_count) {
            if(p.second > max_frequent) {
                result.clear();
                result.push_back(p.first);
                max_frequent = p.second;
            }
            else if(p.second == max_frequent) {
                result.push_back(p.first);
            }
        }
    }
};