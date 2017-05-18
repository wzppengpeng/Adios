/**
 * Determine whether an integer is a palindrome. Do this without extra space.
 */

/**
 * 首先排除负数，然后翻转数字进行比较
 */

class Solution {
public:
    bool isPalindrome(int x) {
        if(x < 0) return false;
        long long re = 0, origin = x;
        while(x) {
            re = re * 10 + x % 10;
            x /= 10;
        }
        return re == origin;
    }
};