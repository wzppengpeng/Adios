/**
 * Given an array of scores that are non-negative integers. Player 1 picks one of the numbers from either end of the array followed by the player 2 and then player 1 and so on. Each time a player picks a number, that number will not be available for the next player. This continues until all the scores have been chosen. The player with the maximum score wins.

Given an array of scores, predict whether player 1 is the winner. You can assume each player plays to maximize his score.

Example 1:
Input: [1, 5, 2]
Output: False
Explanation: Initially, player 1 can choose between 1 and 2.
If he chooses 2 (or 1), then player 2 can choose from 1 (or 2) and 5. If player 2 chooses 5, then player 1 will be left with 1 (or 2). 
So, final score of player 1 is 1 + 2 = 3, and player 2 is 5.
Hence, player 1 will never be the winner and you need to return False.
Example 2:
Input: [1, 5, 233, 7]
Output: True
Explanation: Player 1 first chooses 1. Then player 2 have to choose between 5 and 7. No matter which number player 2 choose, player 1 can choose 233.
Finally, player 1 has more score (234) than player 2 (12), so you need to return True representing player1 can win.
 */

/**
 * MINMAX GAME
 * 实际的解空间为树，使用两种状态下两个方向的递归即可
 * 需要注意的是在A时只需要任意一边大于B得分即可
 * 在B时需要两边均大于
 */


class Solution {
    // the type of turn
    enum class Turn { A, B };

public:
    bool PredictTheWinner(vector<int>& nums) {
        if(nums.size() <= 2) return true;
        return dp(nums, 0, nums.size() - 1, 0, 0, Turn::A);
    }

private:

    bool dp(const vector<int>& nums, int left, int right, int score_A, int score_B, Turn turn) {
        if(left > right) return score_A >= score_B;
        if(turn == Turn::A) {
            return dp(nums, left + 1, right, score_A + nums[left], score_B, Turn::B) ||
                dp(nums, left, right - 1, score_A + nums[right], score_B, Turn::B);
        }
        else {
            return dp(nums, left + 1, right, score_A, score_B + nums[left], Turn::A) &&
                dp(nums, left, right - 1, score_A, score_B + nums[right], Turn::A);
        }
    }

};