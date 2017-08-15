/**
 * Given a sorted array, two integers k and x, find the k closest elements to x in the array. The result should also be sorted in ascending order. If there is a tie, the smaller elements are always preferred.

Example 1:
Input: [1,2,3,4,5], k=4, x=3
Output: [1,2,3,4]
Example 2:
Input: [1,2,3,4,5], k=4, x=-1
Output: [1,2,3,4]
 */

/**
 * 使用二分搜索首先确定中心位置，之后使用双指针法确定区间
 * 需要注意lower_bound确定的是大于该值的位置，所以首先需要把左指针调整为最接近的那个值，然后根据左右的差值进行移动
 */


class Solution {
public:
    vector<int> findClosestElements(vector<int>& arr, int k, int x) {
        int i = std::lower_bound(std::begin(arr), std::end(arr), x) - arr.begin();
        int len = arr.size();
        if(i == len || (i > 0 && arr[i] != x && arr[i] - x > x - arr[i - 1])) --i;
        int j = i + 1;
        while(j - i < k) {
            if(i == 0) ++j;
            else if(j == len) -- i;
            else
                arr[j] - x < x - arr[i - 1] ? ++j : --i;
        }
        return vector<int>(std::begin(arr) + i, std::begin(arr) + j);
    }
};