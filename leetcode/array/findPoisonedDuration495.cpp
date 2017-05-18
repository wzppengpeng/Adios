
/**
 * In LLP world, there is a hero called Teemo and his attacking can make his enemy Ashe be in poisoned condition.
 *  Now, given the Teemo's attacking ascending time series towards Ashe and the poisoning time duration per Teemo's attacking,
 *  you need to output the total time that Ashe is in poisoned condition.
 */

/**
 * Solution:便利一遍即可，时刻存下最后的end的位置
 */

class Solution {
public:
    int findPoisonedDuration(vector<int>& timeSeries, int duration) {
        if(timeSeries.empty()) return 0;
        start = timeSeries.front();
        end = start + duration;
        if(timeSeries.size() == 1) return duration;
        work(timeSeries, duration);
        return res_all + (end - start);
    }

    inline void work(vector<int>& timeSeries, int duration) {
        for(size_t i = 1; i < timeSeries.size(); ++i) {
            if(timeSeries[i] < end) {
                end = timeSeries[i] + duration;
            }
            else {
                res_all += (end - start);
                start = timeSeries[i];
                end = start + duration;
            }
        }
    }

private:
    int start;
    int end;
    int res_all = 0;
};