/**
 * Given two sorted integer arrays nums1 and nums2, merge nums2 into nums1 as one sorted array.

Note:
You may assume that nums1 has enough space (size that is greater or equal to m + n) to hold additional elements from nums2. The number of elements initialized in nums1 and nums2 are m and n respectively.
 */



/**
 * 一到垃圾题
 * 形式很坑爹
 * 但是思路很普遍，一道双指针题，用两个指针进行度量比较即可
 */

class Solution {
public:
    void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
        if(n == 0) return;
        if(m == 0) {
            nums1.swap(nums2); return;
        }
        std::vector<int> res;
        res.reserve(m + n);
        int i = 0, j = 0;
        while(i < m && j < n) {
            if(nums1[i] <= nums2[j]) res.emplace_back(nums1[i++]);
            else res.emplace_back(nums2[j++]);
        }
        if(j < n) std::copy(std::begin(nums2) + j, std::begin(nums2) + n, std::back_inserter(res));
        else if(i < m) std::copy(std::begin(nums1) + i, std::begin(nums1) + m, std::back_inserter(res));
        std::swap(nums1, res);
    }
};