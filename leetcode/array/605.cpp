/**
 * Suppose you have a long flowerbed in which some of the plots are planted and some are not. However, flowers cannot be planted in adjacent plots - they would compete for water and both would die.

Given a flowerbed (represented as an array containing 0 and 1, where 0 means empty and 1 means not empty), and a number n, return if n new flowers can be planted in it without violating the no-adjacent-flowers rule
 */


/**
 * 特别注意全空，开头和结尾的位置即可
 */


class Solution {
public:
    bool canPlaceFlowers(vector<int>& flowerbed, int n) {
        int total = 0;
        int i = 0;
        while(i < flowerbed.size() && flowerbed[i] == 0) ++i;
        if(i == flowerbed.size()) {
            total = (i % 2 == 0) ? i / 2 : (i / 2 + 1);
            return n <= total;
        }
        total += (i / 2);
        int last = i++;
        for(; i < flowerbed.size(); ++i) {
            if(flowerbed[i] == 1) {
                int range = i - last - 1;
                total +=  (range > 0 && range % 2 == 0) ? ((range / 2) - 1) : (range / 2);
                last = i;
            }
        }
        //last
        total += (flowerbed.size() - last - 1) / 2;
        return n <= total;
    }
};