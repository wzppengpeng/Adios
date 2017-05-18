/**
 *
 * Given an integer, return its base 7 string representation.
 */

class Solution {
public:
    string convertToBase7(int num) {
        if(num == 0) return "0";
        if(num < 0) {
            auto res = convertToBase7(-num);
            // std::reverse(begin(res), end(res));
            return "-" + res;
        }
        string s;
        while(num) {
            auto tmp = num % 7;
            num = num / 7;
            s = std::to_string(tmp) + s;
        }
        return std::move(s);
    }
};