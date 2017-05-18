/**
 * There are two sorted arrays nums1 and nums2 of size m and n respectively.

Find the median of the two sorted arrays. The overall run time complexity should be O(log (m+n)).
 */

/**
 * Solution:
 * 使用std::merge进行归并排序，然后再根据奇偶找中位数。
 * 注意：复杂度不对，但是效率好想还可以
 */


class Solution {
public:
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        vector<int> dst;
        dst.reserve(nums1.size() + nums2.size());
        std::merge(nums1.begin(), nums1.end(), nums2.begin(), nums2.end(), std::back_inserter(dst));
        auto total_size = dst.size();
        return total_size % 2 == 0 ?
         (static_cast<double>(dst[total_size / 2]) + static_cast<double>(dst[total_size / 2 - 1])) / 2.0:
         static_cast<double>(dst[total_size / 2]);
    }
};