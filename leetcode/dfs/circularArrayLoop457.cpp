/**
 * You are given an array of positive and negative integers.
 *  If a number n at an index is positive, then move forward n steps.
 *   Conversely, if it's negative (-n), move backward n steps.
 *    Assume the first element of the array is forward next to the last element,
 *     and the last element is backward next to the first element. Determine if there is a loop in this array.
 *     A loop starts and ends at a particular index with more than 1 element along the loop.
 *      The loop must be "forward" or "backward'.

Example 1: Given the array [2, -1, 1, 2, 2], there is a loop, from index 0 -> 2 -> 3 -> 0.

Example 2: Given the array [-1, 2], there is no loop.
 */

/**
 * Solution:深度优先，使用哈希表来存已经到达过的位置
 */

class Solution {
public:
    bool circularArrayLoop(vector<int>& nums) {
        N = nums.size();
        if(N < 2) return false;
        int start = 0;
        for(; start < N; ++start) {
            if(walk_indexs.count(start)) continue;
            if(walk(nums, start)) return true;
        }
        return false;
    }

private:
    unordered_set<int> walk_indexs;

    int N;

    inline int idx(int i, int go) {
        return (i + go) % N;
    }

    inline bool judge(int i, int j) {
        if((i < 0 && j > 0) || (i > 0 && j < 0)) return false;
        else return true;
    }

    bool walk(const vector<int>& nums, int start) {
        if(!judge(nums[start], nums[idx(start, nums[start])])) {
            walk_indexs.emplace(start);
            return false;
        }
        if(nums[start] % N == 0) return false;
        if(walk_indexs.count(start)) return true;
        walk_indexs.emplace(start);
        return walk(nums, idx(start, nums[start]));
    }
};