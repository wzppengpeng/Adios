/**
 * Given two strings s1 and s2, write a function to return true if s2 contains the permutation of s1.
 *  In other words, one of the first string's permutations is the substring of the second string.
 */

/**
 * Solution:使用滑窗，滑窗中使用字典可以表示排列，但是由于只有26个字母，所以使用列表会更快
 */


class Solution {
public:
    bool checkInclusion(string s1, string s2) {
        if(s1.size() > s2.size()) return false;
        m_cnt.resize(26, 0);
        for(int i = 0; i < s1.size(); ++i) {
            ++m_cnt[s1[i] - 'a'];
            --m_cnt[s2[i] - 'a'];
        }
        if(all_zero()) return true;
        int i = 0, j = s1.size();
        for(;j < s2.size(); ++i, ++j) {
            ++m_cnt[s2[i] - 'a'];
            --m_cnt[s2[j] - 'a'];
            if(all_zero()) return true;
        }
        return false;
    }

    vector<int> m_cnt;

    inline bool all_zero() {
        for(auto c : m_cnt) {
            if(c != 0) return false;
        }
        return true;
    }

};