/**
 * Given a non-negative integer represented as a non-empty array of digits, plus one to the integer.

You may assume the integer do not contain any leading zero, except the number 0 itself.

The digits are stored such that the most significant digit is at the head of the list
 */


/**
 * 反过来往前更新，如果小于九直接加一返回原数组即可，否则则设为零，往前更新
 * 最后返回一个1000000类似的结构（针对999999的情况）
 */

class Solution {
public:
    vector<int> plusOne(vector<int>& digits) {
        size_t n = digits.size();
        for(auto it = digits.rbegin(); it != digits.rend(); ++it) {
            if(*it < 9) {
                ++(*it);
                return digits;
            }
            *it = 0;
        }
        vector<int> res(1, 1); res.reserve(n + 1);
        std::copy(std::begin(digits), std::end(digits), std::back_inserter(res));
        return res;
    }
};