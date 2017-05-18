/**
 * We define the Perfect Number is a positive integer that is equal to the sum of all its positive divisors except itself.

Now, given an integer n, write a function that returns true when it is a perfect number and false when it is not.
 */

/**
 * Solution:重点在于判断数的因子，这里注意只有用判断一半即可，并且注意1和正好是开方数的情况
 */


class Solution {
public:
    bool checkPerfectNumber(int num) {
        if(num <= 2) return false;
        int half = static_cast<int>(sqrt(num));
        int sum_val = 1;
        for(int i = 2; i <= half; ++i) {
            if(num % i == 0) {
                sum_val += i;
                int d = num / i;
                if(d != i) sum_val += d;
            }
        }
        return sum_val == num;
    }
};