/**
 * Given a binary array, find the maximum number of consecutive 1s in this array.
 */

/**
 * Solution: o(1)
 */

class Solution {
public:
    int findMaxConsecutiveOnes(vector<int>& nums) {
        if(num.empty()) return 0;
        int consecutive_num = 0, max_len = 0;
        for(auto num : nums) {
            if(num == 1) {
                ++consecutive_num;
            }
            else {
                max_len = max(max_len, consecutive_num);
                consecutive_num = 0;
            }
        }
        return max(max_len, consecutive_num);
    }
};