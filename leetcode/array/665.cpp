/**
 * Given an array with n integers, your task is to check if it could become non-decreasing by modifying at most 1 element.

We define an array is non-decreasing if array[i] <= array[i + 1] holds for every i (1 <= i < n).
 */

/**
 * This problem is like a greedy problem.
 * When you find nums[i-1] > nums[i] for some i,
 * you will prefer to change nums[i-1]'s value,
 * since a larger nums[i] will give you more risks that you get inversion errors after position i.
 * But, if you also find nums[i-2] > nums[i], then you have to change nums[i]'s value instead,
 * or else you need to change both of nums[i-2]'s and nums[i-1]'s values.
 */


class Solution {
public:
    bool checkPossibility(vector<int>& nums) {
        int cnt = 0;                                                                    //the number of changes
        for(int i = 1; i < nums.size() && cnt<=1 ; i++){
            if(nums[i-1] > nums[i]){
                cnt++;
                if(i-2<0 || nums[i-2] <= nums[i])nums[i-1] = nums[i];                    //modify nums[i-1] of a priority
                else nums[i] = nums[i-1];                                                //have to modify nums[i]
            }
        }
        return cnt<=1;
    }
};