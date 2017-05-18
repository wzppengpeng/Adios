/**
 * Given scores of N athletes, find their relative ranks and the people with the top three highest scores,
 *  who will be awarded medals: "Gold Medal", "Silver Medal" and "Bronze Medal".

Example 1:
Input: [5, 4, 3, 2, 1]
Output: ["Gold Medal", "Silver Medal", "Bronze Medal", "4", "5"]
Explanation: The first three athletes got the top three highest scores, so they got "Gold Medal", "Silver Medal" and "Bronze Medal". 
For the left two athletes, you just need to output their relative ranks according to their scores.
 */


/**
 * Use red-black tree to store the ranks, auto score from high to low
 * 再遍历红黑树得到最后的排名，注意特殊情况
 */

class Solution {
public:
    vector<string> findRelativeRanks(vector<int>& nums) {
        if(nums.empty()) return result;
        gen_ranks(nums);
        gen_res(nums);
        return result;
    }

private:
    map<int, int, greater<int> > ranks;
    vector<string> result;

private:
    void gen_ranks(const vector<int>& nums) {
        for (int i = 0; i < nums.size(); ++i)
        {
            ranks.emplace(nums[i], i);
        }
    }

    void gen_res(const vector<int>& nums) {
        result.resize(nums.size());
        if(nums.size() < 4) {
            small(nums);
        }
        else {
            large();
        }
    }

    void small(const vector<int>& nums) {
        if(nums.size() == 1) {
            result[0] = "Gold Medal";
        }
        else if (nums.size() == 2){
            auto it = ranks.begin();
            result[it->second] = "Gold Medal";
            ++it;
            result[it->second] = "Silver Medal";
        }
        else {
            auto it = ranks.begin();
            result[it->second] = "Gold Medal";
            ++it;
            result[it->second] = "Silver Medal";
            ++it;
            result[it->second] = "Bronze Medal";
        }
    }

    void large() {
        auto it = ranks.begin();
        result[it->second] = "Gold Medal";
        ++it;
        result[it->second] = "Silver Medal";
        ++it;
        result[it->second] = "Bronze Medal";
        ++it;
        auto cnt = 3;
        for(; it != ranks.end(); ++it) {
            result[it->second] = std::to_string(++cnt);
        }
    }
};