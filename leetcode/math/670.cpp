/**
 * Given a non-negative integer, you could swap two digits at most once to get the maximum valued number. Return the maximum valued number you could get.

Example 1:
Input: 2736
Output: 7236
Explanation: Swap the number 2 and the number 7.
Example 2:
Input: 9973
Output: 9973
Explanation: No swap.
 */

/**
 * 只需要将从左至有起的第一个数与后续比当前数大并且是最大的数同时该数的最后一个交换即可
 * 实现上首先把数字转换为字符串，可以记录其位置，使用哈希表记录最后的字符位置
 * 并且数字只有9个，所以可以用一个小的内循环控制
 */


class Solution {
public:
    int maximumSwap(int num) {
        auto num_str = std::to_string(num);
        unordered_map<char, size_t> last_location;
        for(size_t i = 0; i < num_str.size(); ++i) {
            last_location[num_str[i]] = i;
        }
        bool has_success = false;
        for(size_t i = 0; i < num_str.size(); ++i) {
            for(char ch = '9'; ch > num_str[i]; --ch) {
                if(last_location.find(ch) != last_location.end() && last_location[ch] > i) {
                    std::swap(num_str[i], num_str[last_location[ch]]);
                    has_success = true;
                    break;
                }
            }
            if(has_success) break;
        }
        return std::stoi(num_str);
    }
};