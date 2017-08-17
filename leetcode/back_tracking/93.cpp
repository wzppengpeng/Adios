/**
 * Given a string containing only digits, restore it by returning all possible valid IP address combinations.

For example:
Given "25525511135",

return ["255.255.11.135", "255.255.111.35"]. (Order does not matter)
 */

/**
 * 分别以3、2、1位数进行递归，其中条件为首字母不为0，大小在0 ~ 255之间
 */

class Solution {
public:
    vector<string> restoreIpAddresses(string s) {
        vector<string> res;
        backtrack(res, s, 0, "", 0);
        return res;
    }

    void backtrack(vector<string>& res, string s, int pos, string comb, int num){
        if(num > 4) return;  // IP address only allows for 4 segments
        if(pos >= s.size()){
            if(num == 4){
                comb.pop_back();
                res.push_back(comb);
            }
            return;
        }
        // 3 digit
        if(s[pos] != '0' && pos < s.size()-2 && stoi(s.substr(pos,3)) < 256)
            backtrack(res, s, pos + 3, comb + s.substr(pos,3) + ".", num + 1);
        // 2 digit
        if(s[pos] != '0' && pos < s.size()-1)
            backtrack(res, s, pos + 2, comb + s.substr(pos,2) + ".", num + 1);
        // 1 digit
        backtrack(res, s, pos + 1, comb + s.substr(pos,1) + ".", num + 1);
    }
};