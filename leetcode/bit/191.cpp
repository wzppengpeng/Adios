/**
 * Write a function that takes an unsigned integer and returns the number of ’1' bits it has (also known as the Hamming weight).

For example, the 32-bit integer ’11' has binary representation 00000000000000000000000000001011, so the function should return 3
 */

/**
 * 使用C++11的bitset可以很方便的处理数字和字符串形式的二进制位数问题
 */


class Solution {
public:
    int hammingWeight(uint32_t n) {
        bitset<32> nn(n);
        return nn.count();
    }
};