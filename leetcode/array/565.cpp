/**
 * A zero-indexed array A consisting of N different integers is given. The array contains all integers in the range [0, N - 1].

Sets S[K] for 0 <= K < N are defined as follows:

S[K] = { A[K], A[A[K]], A[A[A[K]]], ... }.

Sets S[K] are finite for each K and should NOT contain duplicates.

Write a function that given an array A consisting of N integers, return the size of the largest set S[K] for this array.

Example 1:
Input: A = [5,4,0,3,1,6,2]
Output: 4
Explanation:
A[0] = 5, A[1] = 4, A[2] = 0, A[3] = 3, A[4] = 1, A[5] = 6, A[6] = 2.

One of the longest S[K]:
S[0] = {A[0], A[5], A[6], A[2]} = {5, 6, 2, 0}
 */


/**
 * 使用一个一维数组来记录已经遍历过得位置，其实就是找圈，记录一下来过的位置，使用类似dfs的思想
 */


class Solution {
public:
    int arrayNesting(vector<int>& nums) {
        if(nums.size() < 2) return nums.size();
        vector<int> arrive(nums.size(), 0);
        int total = 0;
        for(int i = 0; i < nums.size(); ++i) {
            if(arrive[i] == 0) {
                total = max(dfs(nums, i, arrive), total);
            }
        }
        return total;
    }


    int dfs(const vector<int>& nums, int loc, vector<int>& arrive) {
        int s = 0;
        while(arrive[loc] == 0) {
            arrive[loc] = 1;
            ++s;
            loc = nums[loc];
        }
        return s;
    }

};