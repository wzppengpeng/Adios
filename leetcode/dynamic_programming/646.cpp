/**
 * You are given n pairs of numbers. In every pair, the first number is always smaller than the second number.

Now, we define a pair (c, d) can follow another pair (a, b) if and only if b < c. Chain of pairs can be formed in this fashion.

Given a set of pairs, find the length longest chain which can be formed. You needn't use up all the given pairs. You can select pairs in any order.

Example 1:
Input: [[1,2], [2,3], [3,4]]
Output: 2
Explanation: The longest chain is [1,2] -> [3,4]
 */

/**
 * 按照pair的第二个值进行排序，这样一定保证所有的线段结束都有大小顺序
 * 接下来只需要遍历进行判断起始点是否在当前最长的线段组中即可，使用一个变量记录最大长度，一个变量记录当前线段组结束位置
 */

class Solution {
public:
    int findLongestChain(vector<vector<int>>& pairs) {
        if(pairs.size() < 2) return static_cast<int>(pairs.size());
        sort(std::begin(pairs), std::end(pairs),
            [](const vector<int>& left, const vector<int>& right) {
                return left[1] < right[1];
            });
        int len = 1, fore_large = pairs[0][1];
        for(size_t i = 1; i < pairs.size(); ++i) {
            if(pairs[i][0] > fore_large) {
                ++len;
                fore_large = pairs[i][1];
            }
        }
        return len;
    }
};