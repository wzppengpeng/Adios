/**
 * Now you are given a string S, which represents a software license key which we would like to format. The string S is composed of alphanumerical characters and dashes. The dashes split the alphanumerical characters within the string into groups. (i.e. if there are M dashes, the string is split into M+1 groups). The dashes in the given string are possibly misplaced.

We want each group of characters to be of length K (except for possibly the first group, which could be shorter, but still must contain at least one character). To satisfy this requirement, we will reinsert dashes. Additionally, all the lower case letters in the string must be converted to upper case.

So, you are given a non-empty string S, representing a license key to format, and an integer K. And you need to return the license key formatted according to the description above.
 */

/**
 * Solution
 * 先便利将字母和数字取出来，然后根据取出来的字符判断第一节应该有多大，再便利一遍生成所需要求的动态字符串即可，有点像join的过程
 */

class Solution {
public:
    string licenseKeyFormatting(string S, int K) {
        walk(S);
        if(license.size() <= K) return string(begin(license), end(license));
        //compute the first
        auto first_range = license.size() % K;
        string res;
        int start_pos = 0;
        if(first_range != 0) {
            push(res, 0, first_range);
            start_pos += first_range;
        }
        else {
            push(res, 0, K);
            start_pos += K;
        }
        while(start_pos < license.size()) {
            res.push_back('-');
            push(res, start_pos, K);
            start_pos += K;
        }
        return res;
    }

private:
    vector<char> license;

    void walk(const string& S) {
        license.reserve(S.size());
        for(auto ch : S) {
            if(ch >= '0' && ch <= '9')
                license.push_back(ch);
            else if(ch == '-') continue;
            else
                license.emplace_back(toupper(ch));
        }
    }

    void push(string& res, int start_pos, int num) {
        for(int i = 0; i < num; ++i) {
            res.push_back(license[start_pos+i]);
        }
    }
};