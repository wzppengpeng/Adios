/**
 * Solve a given equation and return the value of x in the form of string "x=#value". The equation contains only '+', '-' operation, the variable x and its coefficient.

If there is no solution for the equation, return "No solution".

If there are infinite solutions for the equation, return "Infinite solutions".

If there is exactly one solution for the equation, we ensure that the value of x is an integer.

Example 1:
Input: "x+5-3+x=6+x-2"
Output: "x=2"
Example 2:
Input: "x=x"
Output: "Infinite solutions"
Example 3:
Input: "2x=x"
Output: "x=0"
Example 4:
Input: "2x+3x-6x=x+2"
Output: "x=-1"
Example 5:
Input: "x=x+2"
Output: "No solution"
 */

/**
 * 主要是过程有点繁琐，分情况讨论较多。
 * 首先找出所有的符号，将x全部移到左边，数字全部移到右边，确定两个系数，在左右两边给予不同的正负性。
 * 尤其是各个阶段开头的位置，注意带正负号和不带正负号的情况
 */


class Solution {
public:
    string solveEquation(string equation) {
        find_sysbom_locs(equation);
        size_t j = add_left(equation);
        add_right(equation, j);
        if(m_x_weight == 0) {
            return m_num_weight == 0 ? "Infinite solutions" : "No solution";
        }
        m_num_weight /= m_x_weight;
        return "x=" + std::to_string(m_num_weight);
    }

private:
    // get the equation loc
    size_t equa_loc = 0;
    // the sysbom loc
    std::vector<size_t> sysbom_loc;

    // the weight of x
    int m_x_weight = 0;
    // the weight of num
    int m_num_weight = 0;

private:
    void find_sysbom_locs(const string& equation) {
        for(size_t i = 0; i < equation.size(); ++i) {
            if(equation[i] == '+' || equation[i] == '-' || equation[i] == '=') {
                sysbom_loc.emplace_back(i);
                if(equation[i] == '=') equa_loc = i;
            }
        }
        sysbom_loc.emplace_back(equation.size());
    }

    size_t add_left(const string& equation) {
        size_t i = 0, j = 0;
        if (sysbom_loc[i] == 0) ++j;
        while(sysbom_loc[j] < equa_loc) {
            auto element = equation.substr(i, sysbom_loc[j] - i);
            if(element.back() == 'x') {
                if(element.size() == 1) {
                    m_x_weight += 1;
                }
                else if(element.size() == 2 && (element.front() == '+' || element.front() == '-')) {
                    if(element.front() == '+') m_x_weight += 1;
                    else m_x_weight -= 1;
                }
                else {
                    int w = std::stoi(element.substr(0, element.size() - 1));
                    m_x_weight += w;
                }
            }
            else {
                int w = std::stoi(element);
                m_num_weight += (-1 * w);
            }
            i = sysbom_loc[j];
            ++j;
        }
        auto element = equation.substr(i, equa_loc - i);
        if(element.back() == 'x') {
            if(element.size() == 1) {
                m_x_weight += 1;
            }
            else if(element.size() == 2 && (element.front() == '+' || element.front() == '-')) {
                if(element.front() == '+') m_x_weight += 1;
                else m_x_weight -= 1;
            }
            else {
                int w = std::stoi(element.substr(0, element.size() - 1));
                m_x_weight += w;
            }
        }
        else {
            int w = std::stoi(element);
            m_num_weight += (-1 * w);
        }
        return j;
    }

    void add_right(const string& equation, size_t j) {
        size_t i = equa_loc + 1;
        if(sysbom_loc[++j] == i) ++j;
        while(j < sysbom_loc.size()) {
            auto element = equation.substr(i, sysbom_loc[j] - i);
            if(element.back() == 'x') {
                if(element.size() == 1) {
                    m_x_weight -= 1;
                }
                else if(element.size() == 2 && (element.front() == '+' || element.front() == '-')) {
                    if(element.front() == '+') m_x_weight -= 1;
                    else m_x_weight += 1;
                }
                else {
                    int w = std::stoi(element.substr(0, element.size() - 1));
                    m_x_weight -= w;
                }
            }
            else {
                int w = std::stoi(element);
                m_num_weight += w;
            }
            i = sysbom_loc[j];
            ++j;
        }
    }

};