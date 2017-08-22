/**
 * Given a string representing an expression of fraction addition and subtraction, you need to return the calculation result in string format. The final result should be irreducible fraction. If your final result is an integer, say 2, you need to change it to the format of fraction that has denominator 1. So in this case, 2 should be converted to 2/1.

Example 1:
Input:"-1/2+1/2"
Output: "0/1"
Example 2:
Input:"-1/2+1/2+1/3"
Output: "1/3"
Example 3:
Input:"1/3-1/2"
Output: "-1/6"
Example 4:
Input:"5/3+1/3"
Output: "2/1"
 */

/**
 * 使用istringstream来处理结构化的字符串
 * 第二，GNU内置了求大大公约数的函数，但是注意使用绝对值来做保证负号在分子上
 */


class Solution {
public:
    string fractionAddition(string expression) {
    istringstream in(expression);
    int A = 0, B = 1, a, b;
    char _;
    while (in >> a >> _ >> b) {
        A = A * b + a * B;
        B *= b;
        int g = abs(__gcd(A, B));
        A /= g;
        B /= g;
    }
    return to_string(A) + '/' + to_string(B);
}
};