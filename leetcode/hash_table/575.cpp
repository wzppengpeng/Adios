/**
 * Given an integer array with even length, where different numbers in this array represent different kinds of candies. Each number means one candy of the corresponding kind. You need to distribute these candies equally in number to brother and sister.
 * Return the maximum number of kinds of candies the sister could gain
 */


/**
 * Solution:一行解决，结果应该是种类数和最大分到数中的最小值。
 */

class Solution {
public:
    int distributeCandies(vector<int>& candies) {
        return std::min(candies.size() / 2, unordered_set<int>(begin(candies), end(candies)).size());
    }
};