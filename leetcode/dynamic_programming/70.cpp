/**
 * You are climbing a stair case. It takes n steps to reach to the top.

Each time you can either climb 1 or 2 steps. In how many distinct ways can you climb to the top?
 */

/**
 * 解空间为线性空间，递推关系很简单，就是前一个和前前一个数量的和，所以可以最简单化代码
 */


class Solution {
public:
    int climbStairs(int n) {
        if(n < 3) return n;
        int before_two = 1, before_one = 2, final = 0;
        for(int i = 3; i <= n; ++i) {
            final = before_one + before_two;
            before_two = before_one;
            before_one = final;
        }
        return final;
    }
};