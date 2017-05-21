/**
 * Given a collection of candidate numbers (C) and a target number (T),
 * find all unique combinations in C where the candidate numbers sums to T.

Each number in C may only be used once in the combination.

Note:
All numbers (including target) will be positive integers.
The solution set must not contain duplicate combinations.
For example, given candidate set [10, 1, 2, 7, 6, 1, 5] and target 8,
A solution set is:
[
  [1, 7],
  [1, 2, 5],
  [2, 6],
  [1, 1, 6]
]
 */

/**
 * 总体思路和39相同，均是从当前位置小于和的角度出发，但是要注意，因为只能用一次，所以下一状态就是i+1
 * 同时最重要的是要跳过重复选项，因此外循环注意和前面相同的跳过，内循环中使用whild循环定位到下一个不同值的位置
 */


class Solution {
public:
    vector<vector<int>> combinationSum2(vector<int>& candidates, int target) {
        sort(candidates.begin(), candidates.end());
        m_target = target;
        auto it = lower_bound(begin(candidates), end(candidates), target);
        int end = ((it != candidates.end() && *it == target)) ? (it - candidates.begin() + 1) : (it - candidates.begin());
        for(int i = 0; i < end; ++i) {
            if(i && candidates[i] == candidates[i - 1]) continue;
            back(i, end, 0, vector<int>(), candidates);
        }
        return m_res;
    }

    int m_target;
    vector<vector<int>> m_res;

    void back(int i, int end, int sum, vector<int> arr, vector<int>& candidates) {
        if(sum == m_target) {
            m_res.emplace_back(arr);
            return;
        }
        if(i >= end) return;
        if(arr.empty()) {
            arr.emplace_back(candidates[i]);
            back(i + 1, end, sum + candidates[i], arr, candidates);
            arr.pop_back();
        }
        else {
            if(sum + candidates[i] < m_target) {
                arr.emplace_back(candidates[i]);
                back(i + 1, end, sum + candidates[i], arr, candidates);
                auto l = arr.back();
                arr.pop_back();
                while(i + 1 < end && candidates[i + 1] == l) {
                    ++i; //跳过相同的值
                }
                if(i + 1 < end) {
                    back(i + 1, end, sum, arr, candidates);
                }
            }
            else if(sum + candidates[i] == m_target) {
                arr.emplace_back(candidates[i]);
                m_res.emplace_back(arr);
                arr.pop_back();
            }
        }
    }

};