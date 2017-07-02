/**
 * Given a non-negative integer c, your task is to decide whether there're two integers a and b such that a2 + b2 = c.
 */


/**
 * 比想象中的简单，sqrt函数相当快
 * sqrt得到一个浮点数，将它转为整数，再反过来看这两个数和是不是等于c即可
 */

class Solution {
public:
    bool judgeSquareSum(int c) {
        for(int i = 0; i <= sqrt(c); ++i) {
            int sum_l = c - i * i;
            int j = sqrt(sum_l);
            if(j * j + i * i == c) return true;
        }
        return false;
    }
};