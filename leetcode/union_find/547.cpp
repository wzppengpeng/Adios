/**
 * There are N students in a class. Some of them are friends, while some are not. Their friendship is transitive in nature. For example, if A is a direct friend of B, and B is a direct friend of C, then A is an indirect friend of C. And we defined a friend circle is a group of students who are direct or indirect friends.

Given a N*N matrix M representing the friend relationship between students in the class. If M[i][j] = 1, then the ith and jth students are direct friends with each other, otherwise not. And you have to output the total number of friend circles among all the students.
 */


/**
 * 典型的并查集算法，核心是用一个list来表示各个点的id，然后进行更新，更新的时候可以用哈希表来保存之前的位置，减小开销
 */

class Solution {
public:
    int findCircleNum(vector<vector<int>>& M) {
        int n = M.size();
        if (n < 2) return n;
        UF(n);
        for(int i = 0; i < n; ++i) {
            for(int j = i + 1; j < n; ++j) {
                if(M[i][j]) {
                    union_find(i, j);
                }
            }
        }
        return cnt;
    }

    void UF(int n) {
        cnt = n;
        id.resize(n);
        for(int i = 0; i < n; ++i) {
            id[i] = i;
            route.emplace(i, vector<int>(1, i));
        }
    }

    void union_find(int p, int q) {
        if(id[p] == id[q]) return;
        int pid = id[p], qid = id[q];
        for(auto l : route[pid]) {
            id[l] = qid;
            route[qid].emplace_back(l);
        }
        --cnt;
    }


private:
    int cnt;
    vector<int> id;
    unordered_map<int, vector<int>> route;
};