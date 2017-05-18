/**
 * So, given a specific rectangular web page’s area, your job by now is to design a rectangular web page, whose length L and width W satisfy the following requirements:

1. The area of the rectangular web page you designed must equal to the given target area.

2. The width W should not be larger than the length L, which means L >= W.

3. The difference between length L and width W should be as small as possible.
 */

/**
 * Solution:利用中间值处理，先找平方根，然后向后搜索
 */

class Solution {
public:
    vector<int> constructRectangle(int area) {
        vector<int> res(2, 1);
        if (area == 1) return res;
        int i = sqrt(area);
        if (i * i == area && i != 1) {
            res[0] = i;
            res[1] = i;
            return res;
        }
        ++i;
        for (; i <= area; ++i) {
            if(area % i == 0) break;
        }
        res[0] = i;
        res[1] = area / i;
        return res;
    }
};