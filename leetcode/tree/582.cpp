/**
 * Given n processes, each process has a unique PID (process id) and its PPID (parent process id).

Each process only has one parent process, but may have one or more children processes. This is just like a tree structure. Only one process has PPID that is 0, which means this process has no parent process. All the PIDs will be distinct positive integers.

We use two list of integers to represent a list of processes, where the first list contains PID for each process and the second list contains the corresponding PPID.

Now given the two lists, and a PID representing a process you want to kill, return a list of PIDs of processes that will be killed in the end. You should assume that when a process is killed, all its children processes will be killed. No order is required for the final answer.
 */


/**
 * 使用字典来存储树，之后dfs得到树的遍历
 */

class Solution {
public:
    vector<int> killProcess(vector<int>& pid, vector<int>& ppid, int kill) {
        int root;
        unordered_map<int, vector<int> >  pid_tree;
        for(size_t i = 0; i < pid.size(); ++i) {
            if(ppid[i] == 0) root = ppid[i];
            if(ppid[i]) pid_tree[ppid[i]].emplace_back(pid[i]);
        }
        vector<int> res;
        dfs(pid_tree, res, kill);
        return res;
    }

    void dfs(const unordered_map<int, vector<int> >& pid_tree, vector<int>& res, int k) {
        res.emplace_back(k);
        auto it = pid_tree.find(k);
        if(it != pid_tree.end()) {
            for(auto p : it->second) {
                dfs(pid_tree, res, p);
            }
        }
    }
};