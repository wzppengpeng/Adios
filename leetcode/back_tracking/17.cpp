/**
 * Given a digit string, return all possible letter combinations that the number could represent.

A mapping of digit to letters (just like on the telephone buttons) is given below.
 */

/**
 * Solution:首先建立映射得到一个二维空间作为解空间，之后将将其作为树状结构，按照树的dfs遍历方法进行处理
 */


class Solution {
public:
    vector<string> letterCombinations(string digits) {
        auto search = std::move(generate(digits));
        vector<string> res;
        if(search.empty()) return res;
        //generate the result
        string tmp(search.size(), '0');
        for(int j = 0; j < search[0].size(); ++j)
            back(search, 0, j, res, tmp);
        return res;
    }

    vector<string> generate(const string& digits) {
        vector<string> search;
        search.reserve(digits.size());
        for(auto ch : digits) {
            if(ch != '0' && ch != '1') {
                search.emplace_back(digits_maps[ch]);
            }
        }
        return std::move(search);
    }

    void back(const vector<string>& search, int i, int j, vector<string>& res, string& tmp) {
        tmp[i] = search[i][j];
        if(i == search.size() -1) {
            res.emplace_back(tmp);
            return;
        }
        for(int k = 0; k < search[i+1].size(); ++k) {
            back(search, i + 1, k, res, tmp);
        }
    }

private:
    unordered_map<char, string> digits_maps {{'2',"abc"},
    {'3',"def"},{'4',"ghi"},{'5',"jkl"},{'6',"mno"},{'7',"pqrs"},
    {'8',"tuv"},{'9',"wxyz"}};
};