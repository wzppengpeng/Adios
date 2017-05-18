/**
 * Given a positive 32-bit integer n,
 *  you need to find the smallest 32-bit integer which has exactly the same digits existing in the integer n and is greater in value than n. If no such positive 32-bit integer exists,
 *   you need to return -1.
 */

/**
 * solution:转换为字符串，然后寻找下一个最小字典序的排列，不过注意这个数是不是超过int上限
 */


class Solution {
public:
    int nextGreaterElement(int n) {
        string s(std::to_string(n));
        if(std::next_permutation(std::begin(s), std::end(s))) {
            auto res_check = std::stoll(s);
            if(res_check > INT_MAX) return -1;
            else return res_check;
        }
        else {
            return -1;
        }
    }
};