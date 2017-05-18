/**
 * Given a List of words,
 *  return the words that can be typed using letters of alphabet on only one row's of American keyboard
 *   like the image below.
 */

/**
 * Solution:初始化一个字典列表，对第一个单词查看索引，之后便利每个字母，如果都在同一索引中，则添加该单词 到最终结果中
 */


class Solution {
public:
    vector<string> findWords(vector<string>& words) {
        vector<string> result;
        result.reserve(words.size());
        for(auto& str : words) {
            //judge the first letter
            int i = 0;
            for(; i < alphabet.size(); ++i) {
                if(alphabet[i].count((char)tolower(str[0]))) break;
            }
            bool is_ok = true;
            for(auto j = 1; j < str.size(); ++j) {
                if(alphabet[i].find((char)tolower(str[j])) == alphabet[i].end()) {
                    is_ok = false;
                    break;
                }
            }
            if(is_ok) {
                result.emplace_back(str);
            }
        }
        return result;
    }

private:
    vector<unordered_set<char>> alphabet
    = {{'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p'},
     {'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l'},
      {'z', 'x', 'c', 'v', 'b', 'n', 'm'}};
};