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

/**
 * 前缀树的方法
 */

class TerDict {
private:
    struct TerNode {
        char c;
        bool isWord;
        TerNode *left, *right, *mid;
        TerNode(char ch): c(ch), isWord(false), left(NULL), right(NULL), mid(NULL) {}
    };
    TerNode *root;
public:
    TerDict() {root = new TerNode('n');}

    void insert(string word) {
        if (word.empty()) return;
        int i = 0, size = word.length();
        TerNode *p = root;
        while (i < size) {
            if (p->c == word[i]) {
                ++i;
                if (i == size) break;
                if (!p->mid) p->mid = new TerNode(word[i]);
                p = p->mid;
            } else if (p->c > word[i]) {
                if (!p->left) p->left = new TerNode(word[i]);
                p = p->left;
            } else {
                if (!p->right) p->right = new TerNode(word[i]);
                p = p->right;
            }
        }
        p->isWord = true;
    }
    // Return the length of the shortest prefix we found
    int search(string word) {
        if (word.empty()) return 0;
        int i = 0, size = word.length();
        TerNode *p = root;
        while (true) {
            if (p->c == word[i]) {
                if (p->isWord) return i + 1;
                if (i == size - 1 || !p->mid) break;
                p = p->mid;
                ++i;
            } else if (p->c > word[i]) {
                if (!p->left) break;
                p = p->left;
            } else {
                if (!p->right) break;
                p = p->right;
            }
        }
        return size;
    }
};

class Solution {
public:
    string replaceWords(vector<string>& dict, string sentence) {
        stringstream ss(sentence);
        TerDict myDict;
        for (string s : dict) myDict.insert(s);
        string ret = "", tmp;
        while (ss >> tmp) {
            if (!ret.empty()) ret += ' ';
            ret += tmp.substr(0, myDict.search(tmp));
        }
        return ret;
    }
};