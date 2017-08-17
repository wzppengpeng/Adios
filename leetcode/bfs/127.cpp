/**
 * Given two words (beginWord and endWord), and a dictionary's word list, find the length of shortest transformation sequence from beginWord to endWord, such that:

Only one letter can be changed at a time.
Each transformed word must exist in the word list. Note that beginWord is not a transformed word.
For example,

Given:
beginWord = "hit"
endWord = "cog"
wordList = ["hot","dot","dog","lot","log","cog"]
As one shortest transformation is "hit" -> "hot" -> "dot" -> "dog" -> "cog",
return its length 5.
 */


/**
 * 使用一个树状结构，遍历得到每一级能够得到的单词，注意在每一层级处理完之后，将已经遍历过得单词去掉
 * BFS使用一个queue来控制，并使用一个计数器来记录每一级的个数
 */


class Solution {
public:
    int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
        unordered_set<string> has_not_walked(std::begin(wordList), std::end(wordList));
        if(has_not_walked.count(endWord) == 0) return 0;
        queue<string> que;
        int len = 1, num = 1;
        que.push(beginWord);
        has_not_walked.erase(beginWord);
        vector<string> erased;
        while(!que.empty()) {
            auto tmp = que.front();
            que.pop();
            --num;
            if(tmp == endWord) return len;
            for(auto& word : has_not_walked) {
                if(compare(tmp, word)) {
                    que.push(word);
                    erased.emplace_back(word);
                }
            }
            if(num == 0) {
                num = que.size();
                for(auto& word : erased) {
                    has_not_walked.erase(word);
                }
                erased.clear();
                ++len;
            }
        }
        return 0;
    }

    bool compare(const string& left, const string& right) {
        int diff = 0;
        for(size_t i = 0; i < left.size(); ++i) {
            if(left[i] != right[i]) ++diff;
            if(diff > 1) return false;
        }
        return diff == 1;
    }
};