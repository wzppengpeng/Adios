/**
 * A message containing letters from A-Z is being encoded to numbers using the following mapping:

'A' -> 1
'B' -> 2
...
'Z' -> 26
Given an encoded message containing digits, determine the total number of ways to decode it.

For example,
Given encoded message "12", it could be decoded as "AB" (1 2) or "L" (12).

The number of ways decoding "12" is 2.
 */


/**
 * 使用动态规划，当前位置会跟前一个和前前个的值发生关系，取决于当前位置的字符和上一个字符
 * s[i]==0 && s[i- 1] == 0 || s[i - 1] > 2 不合理，直接返回0
 * s[i] == 0 || s[i - 1] == 0 得到dp[i - 2]
 * s[i-1]+s[i] <= 26 会有两种情况，分别对应dp[i-1]+dp[i-2]
 * 其他则为dp[i-1]，只能是从当前位置新开始解析
 */


class Solution {
public:
    int numDecodings(string s) {
        if(s.empty() || s.front() == '0') return 0;
        std::vector<int> dp(s.size(), 1);
        for(size_t i = 1; i < s.size(); ++i) {
            if(s[i] == '0' && (s[i - 1] == '0' || s[i - 1] > '2')) return 0;
            if(s[i] == '0' || s[i - 1] == '0') {
                if(i == 1) dp[i] = 1;
                else dp[i] = dp[i - 2];
            }
            else if(char_to_int(s[i - 1]) * 10 + char_to_int(s[i]) <= 26) {
                if(i == 1) dp[i] = 2;
                else dp[i] = dp[i - 1] + dp[i - 2];
            } else {
                dp[i] = dp[i - 1];
            }
        }
        return dp.back();
    }

    inline int char_to_int(char ch) {
        return ch - '0';
    }

};