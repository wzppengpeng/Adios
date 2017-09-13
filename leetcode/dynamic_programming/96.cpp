/**
 * Given n, how many structurally unique BST's (binary search trees) that store values 1...n?

For example,
Given n = 3, there are a total of 5 unique BST's.
 */

/**
 * f(n) = f(0)f(n-1) + f(1)f(n-2) + .......... + f(n-1)f(0)

formula to calculate number of BST from series 1,2,3...n

但是直接用递归会超时
这里使用数组暂存之前的结果然后进行循环即可
 */

class Solution {
public:
    int numTrees(int n) {
        if(n < 2) return n;
        std::vector<int> dp(n, 0);
        dp[0] = 1;
        dp[1] = 2;
        for(int i = 2; i < n; ++i) {
            dp[i] += dp[i - 1] * 2;
            for(int j = 1; j < i; ++j) {
                dp[i] += dp[j - 1] * dp[i - j - 1]; //left number * right number
            }
        }
        return dp.back();
    }
};