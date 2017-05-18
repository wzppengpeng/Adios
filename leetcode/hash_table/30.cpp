/**
 * You are given a string, s, and a list of words, words,
 * that are all of the same length. Find all starting indices of substring(s) in s that is a concatenation of each word in words exactly once and without any intervening characters.

For example, given:
s: "barfoothefoobarman"
words: ["foo", "bar"]

You should return the indices: [0,9].
 */

/**
 * Solution:使用哈希表统计字典，依次向后遍历，确保每个词刚好对应，注意每次只移动一个char
 */

class Solution {
public:
    vector<int> findSubstring(string s, vector<string>& words) {
        if(s.empty() || words.empty()) return vector<int>();
        gen_map(words);
        if(s.size() < words_size * words_length) return vector<int>();
        vector<int> res;
        for(int begin = 0; begin <= s.size() - words_size * words_length; ++begin) {
            if(check(begin, s)) res.emplace_back(begin);
        }
        return res;
    }


    unordered_map<string, int> m_cnt;
    int words_size;
    int words_length;

    void gen_map(const vector<string>& words) {
        words_size = words.size();
        words_length = words.front().size();
        for(auto& w : words) {
            ++m_cnt[w];
        }
    }

    bool check(int begin, const string& s) {
        auto cnt = m_cnt;
        for(int j = 0; j < words_size; ++j) {
            auto it = cnt.find(s.substr(begin + j*words_length, words_length));
            if(it == cnt.end()) return false;
            if(it->second == 0) return false;
            --it->second;
        }
        return true;
    }
};