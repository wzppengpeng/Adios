/**
 * Given a char array representing tasks CPU need to do. It contains capital letters A to Z where different letters represent different tasks.Tasks could be done without original order. Each task could be done in one interval. For each interval, CPU could finish one task or just be idle.

However, there is a non-negative cooling interval n that means between two same tasks, there must be at least n intervals that CPU are doing different tasks or just be idle.

You need to return the least number of intervals the CPU will take to finish all the given tasks.
 */


/**
 * 最好的方法，统计个数，然后计算出来，注意最后有个max，要不然就是通过空闲来满足，要不然就刚好分配好
 */

class Solution {
public:
    int leastInterval(vector<char>& tasks, int n) {
        if(tasks.empty()) return 0;
        if(n == 0) return tasks.size();
        vector<int> task_nums(26, 0);
        for(auto ch : tasks) ++task_nums[ch - 'A'];
        std::sort(std::begin(task_nums), std::end(task_nums), std::greater<int>());
        int i = 1;
        while(i < 26 && task_nums[i] == task_nums[0]) ++i;
        return std::max(static_cast<int>(tasks.size()), (task_nums.front() - 1) * (n + 1) + i);
    }
};