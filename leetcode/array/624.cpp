/**
 * Given m arrays, and each array is sorted in ascending order. Now you can pick up two integers from two different arrays (each array picks one) and calculate the distance. We define the distance between two integers a and b to be their absolute difference |a-b|. Your task is to find the maximum distance.
 */

/**
 * 需要注意第二大和第二小的，都要记录一下
 */

class Solution {
public:
    int maxDistance(vector<vector<int>>& arrays) {
        int min1 = INT_MAX, max1 = INT_MIN, min2 = INT_MAX, max2 = INT_MIN;
        int minIdx = 0, maxIdx = 0;
        for (int i = 0; i < arrays.size(); ++i) {
            int minTmp = arrays[i][0];
            if (minTmp <= min1) min2 = min1, min1 = minTmp, minIdx = i;
            else if (minTmp <= min2) min2 = minTmp;

            int maxTmp = arrays[i].back();
            if (maxTmp >= max1) max2 = max1, max1 = maxTmp, maxIdx = i;
            else if (maxTmp >= max2) max2 = maxTmp;
        }
        if (minIdx == maxIdx)
            return std::max(max2 - min1, max1 - min2);
        return max1 - min1;
    }
};