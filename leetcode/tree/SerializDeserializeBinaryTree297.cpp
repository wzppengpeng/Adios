/**
 * Design an algorithm to serialize and deserialize a binary tree.
 * There is no restriction on how your serialization/deserialization algorithm should work.
 * You just need to ensure that a binary
 * tree can be serialized to a string and this string can be deserialized to the original tree structure.
 */

/**
 * use bfs solution
 */

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Codec {
public:

    // Encodes a tree to a single string.
    string serialize(TreeNode* root) {
        if(!root) return "";
        std::vector<string> tree_vec;
        help_serialize(root, tree_vec);
        return join_string(tree_vec, ',');
    }

    // Decodes your encoded data to tree.
    TreeNode* deserialize(string data) {
        if(data.empty()) return nullptr;
        auto tree_vec = split_string(data, ',');
        return help_deserialize(tree_vec);
    }

private:
    std::vector<std::string> split_string(const std::string& input, char split_delemeter) {
        std::vector<std::string> result;
        if (input.empty()) return std::move(result);
        std::stringstream ss;
        ss << input;
        std::string cache;
        while (std::getline(ss, cache, split_delemeter)) {
            result.emplace_back(cache);
        }
        return std::move(result);
    }

    std::string join_string(const std::vector<std::string>& strs, char delimiter) {
        if (strs.size() <= 0) {
            return std::string("");
        }
        std::stringstream ss;
        ss << strs[0];
        for (size_t i = 1; i < strs.size(); ++i) {
            ss << delimiter;
            ss << strs[i];
        }
        return std::move(ss.str());
    }

    //the help function
    void help_serialize(TreeNode* root, std::vector<string>& tree_vec) {
        queue<TreeNode*> que;
        que.push(root);
        int num = 1;
        while(!que.empty()) {
            auto temp = que.front();
            que.pop();
            --num;
            if(temp) {
                que.push(temp->left);
                que.push(temp->right);
                tree_vec.emplace_back(std::to_string(temp->val));
            }
            else {
                tree_vec.emplace_back("#");
            }
            if(num == 0) num = que.size();
        }
    }

    TreeNode* help_deserialize(std::vector<string>& tree_vec) {
        TreeNode* head = nullptr;
        queue<TreeNode**> que;
        que.push(&head);
        size_t i = 0;
        while(i < tree_vec.size() && !que.empty()) {
            auto temp = que.front();
            que.pop();
            if(tree_vec[i] != "#") {
                *temp = new TreeNode(atoi(tree_vec[i].c_str()));
                que.push(&((*temp)->left));
                que.push(&((*temp)->right));
            }
            ++i;
        }
        return head;
    }

};

// Your Codec object will be instantiated and called as such:
// Codec codec;
// codec.deserialize(codec.serialize(root));