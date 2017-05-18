/**
 * Reverse digits of an integer.

Example1: x = 123, return 321
Example2: x = -123, return -321

The input is assumed to be a 32-bit signed integer.
 Your function should return 0 when the reversed integer overflows.
 */

/**
 * 不要再调用函数，直接处理是否溢出
 */


class Solution {
public:
    int reverse(int x) {
        long long res = 0;
        while(x) {
            res = res * 10 + x % 10;
            x /= 10;
        }
        return (res < INT_MIN || res > INT_MAX) ? 0 : res;
    }
};