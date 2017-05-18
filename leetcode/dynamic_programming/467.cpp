/**
 * Consider the string s to be the infinite wraparound string of "abcdefghijklmnopqrstuvwxyz", so s will look like this: "...zabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcd....".

Now we have another string p. Your job is to find out how many unique non-empty substrings of p are present in s. In particular, your input is the string p and you need to output the number of different non-empty substrings of p in the string s.

Note: p consists of only lowercase English letters and the size of p might be over 10000.
 */


/**
 * 看似为dfs实际是动态规划问题，随时记录当前状态，最后求和，很巧妙
 * 只需要处理结尾字符即可
 */


/**
 * After failed with pure math solution and time out with DFS solution, I finally realized that this is a DP problem...
The idea is, if we know the max number of unique substrings in p ends with 'a', 'b', ..., 'z', then the summary of them is the answer. Why is that?

The max number of unique substring ends with a letter equals to the length of max contiguous substring ends with that letter. Example "abcd", the max number of unique substring ends with 'd' is 4, apparently they are "abcd", "bcd", "cd" and "d".
If there are overlapping, we only need to consider the longest one because it covers all the possible substrings. Example: "abcdbcd", the max number of unique substring ends with 'd' is 4 and all substrings formed by the 2nd "bcd" part are covered in the 4 substrings already.
No matter how long is a contiguous substring in p, it is in s since s has infinite length.
Now we know the max number of unique substrings in p ends with 'a', 'b', ..., 'z' and those substrings are all in s. Summary is the answer, according to the question.
 */

class Solution {
public:
    int findSubstringInWraproundString(string p) {
        vector<int> cnt(26, 0);
        int max_len = 0;
        for(size_t i = 0; i < p.size(); ++i) {
            if(i && ((p[i] == p[i - 1] + 1) || (p[i] == 'a' && p[i - 1] == 'z'))) {
                ++max_len;
            } else {
                max_len = 1;
            }
            cnt[idx(p[i])] = max(cnt[idx(p[i])], max_len);
        }
        return accumulate(begin(cnt), end(cnt), 0);
    }

    inline int idx(char ch) {
        return ch - 'a';
    }
};