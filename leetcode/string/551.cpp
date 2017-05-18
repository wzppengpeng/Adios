/**
 * You are given a string representing an attendance record for a student. The record only contains the following three characters:

'A' : Absent.
'L' : Late.
'P' : Present.
A student could be rewarded if his attendance record doesn't contain more than one 'A' (absent) or more than two continuous 'L' (late).

You need to return whether the student could be rewarded according to his attendance record.
 */


class Solution {
public:
    bool checkRecord(string s) {
        for (auto ch : s) {
            if(ch == 'A') {
                if(A_cnt) return false;
                ++A_cnt;
                L_cnt = 0;
            }
            else if(ch == 'L') {
                if(L_cnt >= 2) return false;
                ++L_cnt;
            }
            else {
                L_cnt = 0;
            }
        }
        return true;
    }

    int A_cnt = 0;
    int L_cnt = 0;
};