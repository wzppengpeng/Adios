/**
 * Given a string of numbers and operators, return all possible results from computing all the different possible ways to group numbers and operators. The valid operators are +, - and *.


Example 1
Input: "2-1-1".

((2-1)-1) = 0
(2-(1-1)) = 2
Output: [0, 2]


Example 2
Input: "2*3-4*5"

(2*(3-(4*5))) = -34
((2*3)-(4*5)) = -14
((2*(3-4))*5) = -10
(2*((3-4)*5)) = -10
(((2*3)-4)*5) = 10
Output: [-34, -14, -10, -10, 10]
 */


/**
 * 根据运算符进行划分递归，两层循环分别得到左边的值加上右边的值的最后结果
 */


class Solution {
public:
    vector<int> diffWaysToCompute(string input) {
        if(input.size() ==0) return {};
        vector<int> result;
        for(int i = 0; i < input.size(); i++)
        {
            if(input[i]!='+' &&input[i]!= '-' &&input[i]!= '*') continue;
            auto vec1 = diffWaysToCompute(input.substr(0, i));
            auto vec2 = diffWaysToCompute(input.substr(i+1));
            for(auto val1: vec1)
            {
                for(auto val2: vec2)
                {
                    if(input[i]=='+') result.push_back(val1+ val2);
                    else if(input[i]=='-') result.push_back(val1 - val2);
                    else if(input[i]== '*') result.push_back(val1 * val2);
                }
            }
        }
        return result.empty()?vector<int>{stoi(input)}:result;
    }
};