/**
 * Given a picture consisting of black and white pixels, find the number of black lonely pixels.

The picture is represented by a 2D char array consisting of 'B' and 'W', which means black and white pixels respectively.

A black lonely pixel is character 'B' that located at a specific position where the same row and same column don't have any other black pixels.
 */

/**
 * Solution:使用哈希来保存已经考察过的行和列
 */



class Solution {
public:
    int findLonelyPixel(vector<vector<char>>& picture) {
        if(picture.empty()) return 0;
        int res = 0;
        for(int i = 0; i < picture.size(); ++i) {
            for(int j = 0; j < picture[0].size(); ++j) {
                if(picture[i][j] == 'B') {
                    if(rows.count(i) || cols.count(j)) {
                        rows.emplace(i);
                        cols.emplace(j);
                        continue;
                    }
                    bool is_only = true;
                    for(int k = i + 1; k < picture.size(); ++k) {
                        if(picture[k][j] == 'B') {
                            is_only = false;
                            rows.emplace(i);
                            cols.emplace(j);
                        }
                    }
                    for(int k = j + 1; k < picture[0].size(); ++k) {
                        if(picture[i][k] == 'B') {
                            is_only = false;
                            rows.emplace(i);
                            cols.emplace(j);
                        }
                    }
                    if(is_only) ++res;
                }
            }
        }
        return res;
    }

private:
    unordered_set<int> rows;
    unordered_set<int> cols;

};
