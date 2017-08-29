/**
 * Given two integers n and k, you need to construct a list which contains n different positive integers ranging from 1 to n and obeys the following requirement: 
Suppose this list is [a1, a2, a3, ... , an], then the list [|a1 - a2|, |a2 - a3|, |a3 - a4|, ... , |an-1 - an|] has exactly k distinct integers.

If there are multiple answers, print any of them.
 */

/**
 * 设计一个数组为1,k+1,2,k,3,k-1...
 * 这样它们两两之间的差值刚刚为k
 */


class Solution {
public:
    vector<int> constructArray(int n, int k) {
        vector<int> res;
        res.reserve(n + 1);
        unordered_set<int> cnt;
        for(int i = 1; i <= n; ++i) cnt.emplace(i);
        int i = 1, r = k + 1;
        for(; i <= n; ++i, --r) {
            if(cnt.count(i)) {
                res.emplace_back(i);
                cnt.erase(i);
            }
            if(r > 0 && cnt.count(r)) {
                res.emplace_back(r);
                cnt.erase(r);
            }
        }
        return res;
    }
};