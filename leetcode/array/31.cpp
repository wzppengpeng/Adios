/**
 * Implement next permutation, which rearranges numbers into the lexicographically next greater permutation of numbers.

If such arrangement is not possible, it must rearrange it as the lowest possible order (ie, sorted in ascending order).

The replacement must be in-place, do not allocate extra memory.

Here are some examples. Inputs are in the left-hand column and its corresponding outputs are in the right-hand column.
 */

/**
 * 使用STL algorithm中的 next_permulation 和 sort
 */


class Solution {
public:
    void nextPermutation(vector<int>& nums) {
        if(!std::next_permutation(std::begin(nums), std::end(nums))) {
            std::sort(std::begin(nums), std::end(nums));
        }
    }
};