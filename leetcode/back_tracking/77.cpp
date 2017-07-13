/**
 * Given two integers n and k, return all possible combinations of k numbers out of 1 ... n.

For example,
If n = 4 and k = 2, a solution is:

[
  [2,4],
  [3,4],
  [2,3],
  [1,2],
  [1,3],
  [1,4],
]
 */

/**
 * 回溯使用递归，递归的时候使用for循环来处理当前位置下一个数所有可以放入数组的可能性
 * 可能性再算的时候就是根据当前数组长度以及n和k的情况来得到下一个数最大到什么位置
 */

class Solution {
public:
    vector<vector<int>> combine(int n, int k) {
        if(k == 0) return res;
        for(int i = 1; i <= n - k  + 1; ++i) {
            back(n, k, i, vector<int>());
        }
        return res;
    }

private:
    vector<vector<int>> res;

    void back(int n, int k, int now, vector<int> arr) {
        // push back a value
        arr.emplace_back(now);
        if(now > n) return;
        if(arr.size() == k) {
            res.emplace_back(std::move(arr));
            return;
        }
        for(int i = now + 1; i <= n - k + arr.size() + 1; ++i) {
            back(n, k, i, arr);
        }
    }
};