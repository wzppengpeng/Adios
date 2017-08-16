/**
 * Initially on a notepad only one character 'A' is present. You can perform two operations on this notepad for each step:

Copy All: You can copy all the characters present on the notepad (partial copy is not allowed).
Paste: You can paste the characters which are copied last time.
Given a number n. You have to get exactly n 'A' on the notepad by performing the minimum number of steps permitted. Output the minimum number of steps to get n 'A'.
 */

/**
 * n=1 1
 * n=2 2
 * n=3 3
 * n=4 2 * 2 || 2 + 2
 * n=5 5
 * n=6 3 + 2
 * 所以可以拆分为分解因式，寻找最简单因子递归
 */


class Solution {
public:
    int minSteps(int n) {
        if(n == 1) return 0;
        for(int i = 2; i < n; ++i) {
            if(n % i == 0) return i + minSteps(n / i);
        }
        return n;
    }
};