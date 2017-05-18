/**
 * There is a brick wall in front of you. The wall is rectangular and has several rows of bricks. The bricks have the same height but different width. You want to draw a vertical line from the top to the bottom and cross the least bricks.

The brick wall is represented by a list of rows. Each row is a list of integers representing the width of each brick in this row from left to right.

If your line go through the edge of a brick, then the brick is not considered as crossed. You need to find out how to draw the line to cross the least bricks and return the number of crossed bricks.

You cannot draw a line just along one of the two vertical edges of the wall, in which case the line will obviously cross no bricks.

Example:
Input:
[[1,2,2,1],
 [3,1,2],
 [1,3,2],
 [2,4],
 [3,1,2],
 [1,3,1,1]]
Output: 2
 */

/**
 * 将这个问题转化为判断最多的缝隙，即对缝隙进行统计，使用一个哈希表计数，之后对每一行求和即是缝隙
 */

class Solution {
public:
    int leastBricks(vector<vector<int>>& wall) {
        unordered_map<int, int> cnts;
        for(size_t i = 0; i < wall.size(); ++i) {
            int sum = 0;
            for(size_t j = 0; j < wall[i].size() - 1; ++j) {
                sum += wall[i][j];
                cnts[sum] += 1;
            }
        }
        if(cnts.size() == 0) return wall.size();
        int max_val = 0;
        for(auto& p : cnts) {
            if(p.second > max_val) {
                max_val = p.second;
            }
        }
        return wall.size() - max_val;
    }
};