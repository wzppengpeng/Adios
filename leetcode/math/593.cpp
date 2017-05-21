/**
 *
The coordinate (x,y) of a point is represented by an integer array with two integers.

Example:
Input: p1 = [0,0], p2 = [1,1], p3 = [1,0], p4 = [0,1]
Output: True
 */

/**
 * 一道数学题，我采用判断等距，垂直，对角垂直，距离为根号2的办法
 * 唯一需要注意的是，一开始如果先1,2,3这三个点，会出现三种可能，是边，一边一对角
 * 分情况讨论一下，并排除相同点的情况
 */


class Solution {
public:
    bool validSquare(vector<int>& p1, vector<int>& p2, vector<int>& p3, vector<int>& p4) {
        if(compute_distane(p1, p2) == compute_distane(p1, p3)) {
            if(judge_same_point(p1, p2) || judge_same_point(p2, p3)) return false;
            if(!judge_vertival(p1, p2, p1, p3)) return false;
            if(!judge_vertival(p1, p4, p2, p3)) return false;
            if(compute_distane(p1, p4) != 2 * compute_distane(p1, p3)) return false;
            return true;
        }
        else if(compute_distane(p1, p3) == 2 * compute_distane(p1, p2)) {
            if(compute_distane(p1, p2) != compute_distane(p1, p4)) return false;
            if(judge_same_point(p1, p2) || judge_same_point(p2, p4)) return false;
            if(!judge_vertival(p1, p2, p1, p4)) return false;
            if(!judge_vertival(p1, p3, p2, p4)) return false;
            return true;
        }
        else if(compute_distane(p1, p2) == 2 * compute_distane(p1, p3)) {
            if(compute_distane(p1, p3) != compute_distane(p1, p4)) return false;
            if(judge_same_point(p1, p3) || judge_same_point(p3, p4)) return false;
            if(!judge_vertival(p1, p3, p1, p4)) return false;
            if(!judge_vertival(p1, p2, p3, p4)) return false;
            return true;
        }
        else return false;
    }

private:
    //help functions
    int compute_distane(vector<int>& p1, vector<int>& p2) {
        return (p1[0] - p2[0]) * ((p1[0] - p2[0])) + (p1[1] - p2[1]) * (p1[1] - p2[1]);
    }

    bool judge_vertival(vector<int>& p1, vector<int>& p2, vector<int>& p3, vector<int>& p4) {
        vector<int> l12(2);
        l12[0] = p2[0] - p1[0];
        l12[1] = p2[1] - p1[1];
        vector<int> l34(2);
        l34[0] = p4[0] - p3[0];
        l34[1] = p4[1] - p3[1];
        return (l12[0] * l34[0] + l12[1] * l34[1]) == 0;
    }

    inline bool judge_same_point(vector<int>& p1, vector<int>& p2) {
        return (p1[0] == p2[0] && p1[1] == p2[1]);
    }
};
