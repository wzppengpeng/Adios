/**
 * Given a word, you need to judge whether the usage of capitals in it is right or not.

We define the usage of capitals in a word to be right when one of the following cases holds:

All letters in this word are capitals, like "USA".
All letters in this word are not capitals, like "leetcode".
Only the first letter in this word is capital if it has more than one letter, like "Google".
Otherwise, we define that this word doesn't use capitals in a right way.
 */

/**
 * Solution: walk through
 */

class Solution {
public:
    bool detectCapitalUse(string word) {
        //judge the first
        bool first_capital = judge_capital(word[0]);
        if(first_capital) {
            if(word.size() == 2) return true;
            if(judge_capital(word[1])) {
                for(size_t i = 2; i < word.size(); ++i) {
                    if(!judge_capital(word[i])) return false;
                }
                return true;
            }
            else {
                for(size_t i = 2; i < word.size(); ++i) {
                    if(judge_capital(word[i])) return false;
                }
                return true;
            }
        }
        else {
            for(size_t i = 1; i < word.size(); ++i) {
                if(judge_capital(word[i])) return false;
            }
            return true;
        }
    }

private:
    inline bool judge_capital(char ch) {
        if(ch >= 'A' && ch <= 'Z') {
            return true;
        }
        else {
            return false;
        }
    }
};