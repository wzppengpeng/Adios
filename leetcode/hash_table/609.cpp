/**
 * Given a list of directory info including directory path, and all the files with contents in this directory, you need to find out all the groups of duplicate files in the file system in terms of their paths.

A group of duplicate files consists of at least two files that have exactly the same content.

A single directory info string in the input list has the following format:

"root/d1/d2/.../dm f1.txt(f1_content) f2.txt(f2_content) ... fn.txt(fn_content)"

It means there are n files (f1.txt, f2.txt ... fn.txt with content f1_content, f2_content ... fn_content, respectively) in directory root/d1/d2/.../dm. Note that n >= 1 and m >= 0. If m = 0, it means the directory is just the root directory.

The output is a list of group of duplicate file paths. For each group, it contains all the file paths of the files that have the same content. A file path is a string that has the following format:

"directory_path/file_name.txt"

Example 1:
Input:
["root/a 1.txt(abcd) 2.txt(efgh)", "root/c 3.txt(abcd)", "root/c/d 4.txt(efgh)", "root 4.txt(efgh)"]
Output:
[["root/a/2.txt","root/c/d/4.txt","root/4.txt"],["root/a/1.txt","root/c/3.txt"]]
 */

/**
 * 也没什么好说的，思路很简单，使用哈希表来记住当前分组的索引就可以了，唯一需要注意的是最终结果只包含有重复的，也就是分组中数量大于1的
 */


class Solution {
public:
    vector<vector<string>> findDuplicate(vector<string>& paths) {
        for(auto& path : paths) {
            auto lines = split_string(path, ' ');
            auto& dir = lines[0];
            for(size_t i = 1; i < lines.size(); ++i) {
                auto file_content = split_file_content(lines[i]);
                auto it = content_indexs.find(file_content.second);
                if(it != content_indexs.end()) {
                    m_res[it->second].emplace_back(dir + "/" + file_content.first);
                }
                else {
                    content_indexs.emplace(file_content.second, m_res.size());
                    m_res.emplace_back(vector<string>(1, dir + "/" + file_content.first));
                }
            }
        }
        vector<vector<string> > res; res.reserve(m_res.size());
        for(auto& strs : m_res) {
            if (strs.size() > 1) {
                res.emplace_back(strs);
            }
        }
        return res;
    }


    std::vector<std::string> split_string(const std::string& input, char split_delemeter) {
        std::vector<std::string> result;
        if (input.empty()) return std::move(result);
        std::istringstream ss(input);
        std::string cache;
        while (std::getline(ss, cache, split_delemeter)) {
            result.emplace_back(cache);
        }
        return std::move(result);
    }

    pair<string, string> split_file_content(const string& file_content) {
        auto pos = file_content.find_first_of('(');
        return make_pair(file_content.substr(0, pos),
            file_content.substr(pos + 1, file_content.size() - pos - 2));
    }

    vector<vector<string> > m_res;
    unordered_map<string, size_t> content_indexs;

};