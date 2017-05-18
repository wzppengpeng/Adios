/**
 * The string "PAYPALISHIRING" is written in a zigzag pattern on a given number of rows like this:
 *  (you may want to display this pattern in a fixed font for better legibility)

P   A   H   N
A P L S I I G
Y   I   R
And then read line by line: "PAHNAPLSIIGYIR"
 */

/**
 * Solution:在本地就可以完成，对于不同的位置通过控制方向，算下一个该读取位置的坐标，尤其注意第一行和最后一行应该只有一个方向
 */


class Solution {
public:
    string convert(string s, int numRows) {
        if(s.empty() || numRows == 1) return s;
        string res;
        auto len = s.size();
        res.reserve(len);
        for(int i = 0; i < numRows; ++i) {
            int now_pos = i;
            bool is_down = i == numRows - 1 ? false : true;
            while(now_pos < len) {
                res.push_back(s[now_pos]);
                if(is_down) {
                    now_pos += ((numRows - i - 1) + (numRows - i - 1));
                }
                else {
                    now_pos += ((i) + (i));
                }
                if(i == 0 || i == numRows - 1) continue;
                is_down = !is_down;
            }
        }
        return res;
    }
};