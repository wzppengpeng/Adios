/**
 * Implement atoi to convert a string to an integer.

Hint: Carefully consider all possible input cases. If you want a challenge, please do not see below and ask yourself what are the possible input cases.
 */

/**
 * Solution:
 * 注意边界
 */

class Solution {
public:
    int myAtoi(string str) {
        long long temp = std::atoll(str.c_str());
        if(temp > INT_MAX)
            return INT_MAX;
        if(temp < INT_MIN)
            return INT_MIN;
        return (int)temp;
    }
};