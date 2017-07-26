/**
 * The set S originally contains numbers from 1 to n. But unfortunately, due to the data error, one of the numbers in the set got duplicated to another number in the set, which results in repetition of one number and loss of another number.

Given an array nums representing the data status of this set after the error. Your task is to firstly find the number occurs twice and then find the number that is missing. Return them in the form of an array.

Example 1:
Input: nums = [1,2,2,4]
Output: [2,3]
 */

/**
 * 使用两个哈希表，分别记录出现过的数字和消除的数字即可
 */

class Solution {
public:
    vector<int> findErrorNums(vector<int>& nums) {
        vector<int> res(2, 0);
        init(nums.size());
        for(auto num : nums) {
            if(m_cnt.count(num)) {
                res[0] = num;
            } else {
                m_cnt.emplace(num);
                m_total.erase(num);
            }
        }
        res[1] = *m_total.begin();
        return res;
    }

    unordered_set<int> m_total;
    unordered_set<int> m_cnt;

    void init(size_t n) {
        for(size_t i = 1; i <= n; ++i) {
            m_total.emplace(i);
        }
    }

};