/**
 * Given a string and a string dictionary,
 *  find the longest string in the dictionary that can be formed by deleting some characters of the given string.
 *   If there are more than one possible results, return the longest word with the smallest lexicographical order.
 *    If there is no possible result, return the empty string.

Example 1:
Input:
s = "abpcplea", d = ["ale","apple","monkey","plea"]

Output:
"apple"
 */

/**
 * Solution:两部分构成，第一：使用双指针方法确定该单词是否是原始的子集，然后与外部保存的最优解进行排序比较
 */

class Solution {
public:
    string findLongestWord(string s, vector<string>& d) {
        string res;
        for(auto& t : d) {
            if(judge_words(s, t)) {
                if(comare_words(t, res)) {
                    res = t;
                }
            }
        }
        return res;
    }

private:
    bool judge_words(const string& s, const string& t) {
        if(t.size() > s.size()) return false;
        int i = 0, j = 0;
        while(j < t.size()) {
            if(i >= s.size()) return false;
            if(s[i] == t[j]) ++j;
            ++i;
        }
        return true;
    }

    bool comare_words(const string& l, const string& r) {
        if(l.size() != r.size()) {
            return l.size() > r.size();
        }
        else {
            return l < r;
        }
    }
};