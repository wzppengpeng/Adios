
/**
 * Given a circular array (the next element of the last element is the first element of the array),
 *  print the Next Greater Number for every element.
 *   The Next Greater Number of a number x is the first greater number to its traversing-order next in the array,
 *    which means you could search circularly to find its next greater number.
 *     If it doesn't exist, output -1 for this number.

Example 1:
Input: [1,2,1]
Output: [2,-1,2]
Explanation: The first 1's next greater number is 2;
The number 2 can't find next greater number;
The second 1's next greater number needs to search circularly, which is also 2.
 */


/**
 * Solution:用的暴力法，两层循环= =可以用栈
 */

class Solution {
public:
    vector<int> nextGreaterElements(vector<int>& nums) {
        N = nums.size();
        vector<int> result(N);
        for (int i = 0; i < N; ++i)
        {
            /* code */
            result[i] = search(nums, i);
        }
        return result;
    }

    /**
     *  Use Stack solution
     */
    vector<int> nextGreaterElements(vector<int>& nums) {
        vector<int> result(nums.size(), -1);
        stack<int> s;
        for (int i = 0; i < nums.size() * 2; ++i)
        {
            /* code */
            int num = nums[i % nums.size()];
            while(!s.empty() && nums[s.top()] < num) {
                result[s.top()] = num;
                s.pop();
            }
            if(i < nums.size()) s.push(i);
        }
        return result;
    }

private:
    int N;

    inline int idx(int x) {
        if (x < N) return x;
        else {
            return x - N;
        }
    }

    int search(const vector<int>& nums, int index) {
        for (int i = 1; i < N; ++i)
        {
            /* code */
            if(nums[idx(index + i)] > nums[index])
                return nums[idx(index + i)];
        }
        return -1;
    }
};