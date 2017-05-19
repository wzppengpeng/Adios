/**
 * Given a set of candidate numbers (C) (without duplicates) and a target number (T),
 * find all unique combinations in C where the candidate numbers sums to T.

The same repeated number may be chosen from C unlimited number of times.

Note:
All numbers (including target) will be positive integers.
The solution set must not contain duplicate combinations.
For example, given candidate set [2, 3, 6, 7] and target 7
[
  [7],
  [2, 2, 3]
]
 */


/**
 * 一道典型的暴力回溯法，一定要理清楚从哪里来，到哪里去
 * 每一个状态理清楚只能从哪个状态来，下一步应该到哪个状态去
 * 比如该问题，应当设置所有的起始位置
 * 回溯时所有的状态都引申自和不满的情况，当不满足状态，函数返回就在和不到的时候，则直接下一个位置即可。
 */


class Solution {
public:
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        sort(candidates.begin(), candidates.end());
        m_target = target;
        auto it = lower_bound(begin(candidates), end(candidates), target);
        int end = ((it != candidates.end() && *it == target)) ? (it - candidates.begin() + 1) : (it - candidates.begin());
        vector<int> arr;
        vector<int> loc;
        for(int i = 0; i < end; ++i) {
            back(i, end, 0, arr, loc, candidates);
        }
        return m_res;
    }

    int m_target;
    vector<vector<int>> m_res;

    void back(int i, int end, int sum, vector<int> arr, vector<int> loc, vector<int>& candidates) {
        if(i >= end) return;
        if(sum == m_target) {
            m_res.emplace_back(arr);
            return;
        }
        if(arr.empty()) {
            arr.emplace_back(candidates[i]); loc.emplace_back(i);
            back(i, end, candidates[i], arr, loc, candidates);
            arr.pop_back(); loc.pop_back();
        }
        else {
            if(sum + candidates[i] < m_target) {
                arr.emplace_back(candidates[i]); loc.emplace_back(i);
                back(i, end, sum + candidates[i], arr, loc, candidates);
                arr.pop_back(); loc.pop_back();
                back(i + 1, end, sum, arr, loc, candidates);
            }
            else if(sum + candidates[i] == m_target) {
                arr.emplace_back(candidates[i]);
                m_res.emplace_back(arr);
                arr.pop_back();
            }
        }
    }
};