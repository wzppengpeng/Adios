/**
 * Given an array of strings, group anagrams together.

For example, given: ["eat", "tea", "tan", "ate", "nat", "bat"],
Return:

[
  ["ate", "eat","tea"],
  ["nat","tan"],
  ["bat"]
]
 */

/**
 * 将输入字符串进行排序，即化为字典序，作为其标识
 * 使用一个哈希表存该最小字符串的index
 */


class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        for(auto& str : strs) {
            string tmp = str;
            std::sort(begin(tmp), end(tmp));
            auto it = indexs.find(tmp);
            if(it != indexs.end()) {
                words[it->second].emplace_back(str);
            }
            else {
                indexs.emplace(tmp, words.size());
                words.emplace_back(vector<string>(1, str));
            }
        }
        return words;
    }

    vector<vector<string> > words;
    unordered_map<string, size_t> indexs;

};