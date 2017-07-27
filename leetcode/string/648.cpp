/**
 * In English, we have a concept called root, which can be followed by some other words to form another longer word - let's call this word successor. For example, the root an, followed by other, which can form another word another.

Now, given a dictionary consisting of many roots and a sentence. You need to replace all the successor in the sentence with the root forming it. If a successor has many roots can form it, replace it with the root with the shortest length.

You need to output the sentence after the replacement.

Example 1:
Input: dict = ["cat", "bat", "rat"]
sentence = "the cattle was rattled by the battery"
Output: "the cat was rat by the bat"
 */

/**
 * 就是挨个遍历，使用find去查有没有这个前缀即可，排序使的短的在前面
 */


class Solution {
public:
    string replaceWords(vector<string>& dict, string sentence) {
        std::sort(std::begin(dict), std::end(dict),
            [](const string& left, const string& right) {
                return left.size() < right.size();
            });
        istringstream ss(sentence);
        string word, ans;
        ans.reserve(sentence.size());
        while(ss >> word) {
            for(auto d : dict) {
                if(word.find(d) == 0) {
                    word = d;
                    break;
                }
            }
            ans.append(word);
            ans.push_back(' ');
        }
        ans.pop_back();
        return std::move(ans);
    }

};