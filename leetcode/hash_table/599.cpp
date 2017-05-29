/**
 * Suppose Andy and Doris want to choose a restaurant for dinner, and they both have a list of favorite restaurants represented by strings.

You need to help them find out their common interest with the least list index sum. If there is a choice tie between answers, output all of them with no order requirement. You could assume there always exists an answer.
 */


/**
 * 很简单，不想写了
 */

class Solution {
public:
    vector<string> findRestaurant(vector<string>& list1, vector<string>& list2) {
        add1(list1);
        add2(list2);
        vector<string> min_res;
        int min_val = INT_MAX;
        for(auto& p : m_index_cnt_total) {
            if(p.second < min_val) {
                min_res.clear();
                min_res.emplace_back(p.first);
                min_val = p.second;
            }
            else if(p.second == min_val) {
                min_res.emplace_back(p.first);
            }
        }
        return min_res;
    }

    unordered_map<string, int> m_index_cnt;
    unordered_map<string, int> m_index_cnt_total;

    void add1(vector<string>& l) {
        for(int i = 0; i < l.size(); ++i) {
            m_index_cnt.emplace(l[i], i);
        }
    }

    void add2(vector<string>& l) {
        for(int i = 0; i < l.size(); ++i) {
            auto it = m_index_cnt.find(l[i]);
            if(it != m_index_cnt.end()) {
                m_index_cnt_total.emplace(l[i], i + it->second);
            }
        }
    }

};