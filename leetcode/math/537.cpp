/**
 * Given two strings representing two complex numbers.

You need to return a string representing their multiplication. Note i2 = -1 according to the definition.

Example 1:
Input: "1+1i", "1+1i"
Output: "0+2i"
Explanation: (1 + i) * (1 + i) = 1 + i2 + 2 * i = 2i, and you need convert it to the form of 0+2i.
Example 2:
Input: "1+-1i", "1+-1i"
Output: "0+-2i"
Explanation: (1 - i) * (1 - i) = 1 + i2 - 2 * i = -2i, and you need convert it to the form of 0+-2i.
 */

/**
 * s = (ac - bd) + (bc + ad)i
 */


class Solution {
public:
    string complexNumberMultiply(string a, string b) {
        auto complex_a = transform(a);
        auto complex_b = transform(b);
        pair<int, int> res {0, 0};
        res.first = (complex_a.first * complex_b.first) - (complex_a.second * complex_b.second);
        res.second = (complex_a.second * complex_b.first) + (complex_a.first * complex_b.second);
        return back(res);
    }

private:
    pair<int, int> transform(const string& s) {
        auto pos = s.find_first_of('+');
        auto shibu = std::stoi(s.substr(0, pos));
        auto xubu = std::stoi(s.substr(pos+1, s.size() - 2 - pos));
        return {shibu, xubu};
    }

    string back(const pair<int, int>& res) {
        return std::to_string(res.first) + "+" +std::to_string(res.second) + "i";
    }
};