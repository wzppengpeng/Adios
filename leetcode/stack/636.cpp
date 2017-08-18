/**
 * Given the running logs of n functions that are executed in a nonpreemptive single threaded CPU, find the exclusive time of these functions.

Each function has a unique id, start from 0 to n-1. A function may be called recursively or by another function.

A log is a string has this format : function_id:start_or_end:timestamp. For example, "0:start:0" means function 0 starts from the very beginning of time 0. "0:end:0" means function 0 ends to the very end of time 0.

Exclusive time of a function is defined as the time spent within this function, the time spent by calling other functions should not be considered as this function's exclusive time. You should return the exclusive time of each function sorted by their function id.

Example 1:
Input:
n = 2
logs =
["0:start:0",
 "1:start:2",
 "1:end:5",
 "0:end:6"]
Output:[3, 4]
 */


/**
 * 典型的压栈问题，栈顶为当前正在工作的id
 * 注意的是题中的时间戳针对开始和结束并不是完全对应的，需要先处理一下
 * 此外，每次结束出栈之后，需要将先前的fucntion id的起始时间进行更新
 */



class Solution {
public:
    vector<int> exclusiveTime(int n, vector<string>& logs) {
        vector<int> res(n, 0);
        stack<pair<int, int>> st;
        int function_id, time_stamp;
        for(auto& log : logs) {
            if(st.empty()) {
                parse_log(log, &function_id, &time_stamp);
                st.push(std::make_pair(function_id, time_stamp));
            } else {
                auto now_work = st.top();
                if(parse_log(log, &function_id, &time_stamp)) {
                    res[now_work.first] += time_stamp - now_work.second;
                    st.push(std::make_pair(function_id, time_stamp));
                } else {
                    res[now_work.first] += time_stamp - now_work.second;
                    st.pop();
                    if(!st.empty()) {
                        st.top().second = time_stamp;
                    }
                }
            }
        }
        return res;
    }

private:
    bool parse_log(const std::string& input, int* function_id, int* time_stamp) {
        std::vector<std::string> result; result.reserve(3);
        std::istringstream ss(input);
        std::string cache;
        while (std::getline(ss, cache, ':')) {
            result.emplace_back(cache);
        }
        *function_id = std::stoi(result[0]);
        if(result[1] == "start") {
            *time_stamp = std::stoi(result[2]);
            return true;
        } else {
            *time_stamp = std::stoi(result[2]) + 1;
            return false;
        }
    }

};