/**
 * Implement strStr().

Returns the index of the first occurrence of needle in haystack,
 or -1 if needle is not part of haystack.
 */

/**
 * Use std::string find
 */


class Solution {
public:
    int strStr(string haystack, string needle) {
        if(needle.size() > haystack.size()) return -1;
        auto pos = haystack.find(needle);
        return pos == string::npos ? -1 : pos;
    }

};