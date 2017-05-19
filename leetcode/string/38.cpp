/**
 * The count-and-say sequence is the sequence of integers beginning as follows:
1, 11, 21, 1211, 111221, ...

1 is read off as "one 1" or 11.
11 is read off as "two 1s" or 21.
21 is read off as "one 2, then one 1" or 1211.
Given an integer n, generate the nth sequence.
 */


/**
 * Solution:注意审题，说的是第n个，所以首先需要一个函数可以进行count，然后在主函数中迭代即可。
 * count的函数思路类似two point，设置区间进行遍历，注意结束条件
 */

class Solution {
public:
    string countAndSay(int n) {
        if(n == 1) return "1";
        string last("1");
        for(int i = 1; i < n - 1; ++i) {
            auto new_last = count(last);
            last = new_last;
        }
        return count(last);
    }

    string count(const string& nn) {
        string res;
        int i = 0, j = 1;
        while(j < nn.size()) {
            while(j < nn.size() && nn[j] == nn[i]) ++j;
            if(j < nn.size()) {
                res.append(to_string(j - i));
                res.push_back(nn[i]);
                i = j++;
            }
        }
        res.append(to_string(j - i));
        res.push_back(nn[i]);
        return res;
    }
};