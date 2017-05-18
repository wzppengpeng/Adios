/**
 * Given a list of 24-hour clock time points in "Hour:Minutes" format,
 * find the minimum minutes difference between any two time points in the list.

Example 1:
Input: ["23:59","00:00"]
Output: 1
 */

/**
 * Solution:先将输入全部化成int形式的minutes，然后排序，比较相邻元素的差距，注意首尾的比较，头乘以昨天的分钟数
 */


class Solution {
public:
    int findMinDifference(vector<string>& timePoints) {
        if(timePoints.size() < 2) return 0;
        trainsform(timePoints);
        std::sort(begin(timestamp), end(timestamp));
        int min_minutes = INT_MAX;
        int tmp_minutes;
        for(size_t i = 1; i < timestamp.size(); ++i) {
            tmp_minutes = timestamp[i] - timestamp[i - 1];
            if(tmp_minutes < min_minutes) min_minutes = tmp_minutes;
        }
        //for the last
        tmp_minutes = timestamp.front() + 1440 - timestamp.back();
        if(tmp_minutes < min_minutes) min_minutes = tmp_minutes;
        return min_minutes;
    }

private:
    std::vector<int> timestamp;

    void trainsform(const vector<string>& timePoints) {
        timestamp.reserve(timePoints.size());
        for(const auto& s : timePoints) {
            auto s_ = split_string(s, ':');
            timestamp.emplace_back(s_[0] * 60 + s_[1]);
        }
    }

    inline static std::vector<int> split_string(const std::string& input, char split_delemeter) {
        std::vector<int> result;
        result.reserve(2);
        if (input.empty()) return std::move(result);
        std::istringstream ss(input);
        std::string cache;
        while (std::getline(ss, cache, split_delemeter)) {
            result.emplace_back(stoi(cache));
        }
        return std::move(result);
    }

};