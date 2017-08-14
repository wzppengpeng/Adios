/**
 * Initially, there is a Robot at position (0, 0). Given a sequence of its moves, judge if this robot makes a circle, which means it moves back to the original place.

The move sequence is represented by a string. And each move is represent by a character. The valid robot moves are R (Right), L (Left), U (Up) and D (down). The output should be true or false representing whether the robot makes a circle.
 */


/**
 * 分治算法，对每一步的位置进行分解，最终得到最后的位置是不是在原位即可
 */

class Solution {
public:
    bool judgeCircle(string moves) {
        int x = 0, y = 0;
        for(auto ch : moves) {
            move(x, y, ch);
        }
        return x == 0 && y == 0;
    }

    inline void move(int& x, int& y, char ch) {
        if(ch == 'R') {
            ++x;
        } else if(ch == 'L') {
            --x;
        } else if(ch == 'U') {
            ++y;
        } else {
            --y;
        }
    }
};