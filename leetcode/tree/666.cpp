/**
 * If the depth of a tree is smaller than 5, then this tree can be represented by a list of three-digits integers.

For each integer in this list:
The hundreds digit represents the depth D of this node, 1 <= D <= 4.
The tens digit represents the position P of this node in the level it belongs to, 1 <= P <= 8. The position is the same as that in a full binary tree.
The units digit represents the value V of this node, 0 <= V <= 9.
Given a list of ascending three-digits integers representing a binary with the depth smaller than 5. You need to return the sum of all paths from the root towards the leaves.
 */


/**
 * 使用了一个数组加哈希表的结构来表示每一层的数，每一层的哈希表记录到当前位置的和
 * 求结果时遍历数组，当下一级为空或者键值全部存在时则为叶子节点，将结果加起来
 */



class Solution {
public:
    int pathSum(vector<int>& nums) {
        if(nums.empty()) return 0;
        int res = 0;
        vector<unordered_map<int, int> > node_sums(6);
        node_sums[0][0] = get_value(nums[0]);
        for(size_t i = 1; i < nums.size(); ++i) {
            int depth = get_depth(nums[i]) - 1;
            int location = get_location(nums[i]) - 1;
            int value = get_value(nums[i]);
            int sum_now = (node_sums[depth - 1][location / 2]) + value;
            node_sums[depth][location] = sum_now;
        }
        for(size_t i = 0; i < node_sums.size(); ++i) {
            if(node_sums[i].empty()) break;
            for(auto& p : node_sums[i]) {
                int location = p.first;
                if(node_sums[i + 1].empty() ||
                    (node_sums[i + 1].find(location * 2) == node_sums[i + 1].end()
                    && node_sums[i + 1].find(location * 2 + 1) == node_sums[i + 1].end())) {
                    res += p.second;
                }
            }
        }
        return res;
    }


    inline int get_depth(int n) {
        return n / 100;
    }

    inline int get_location(int n) {
        return (n % 100) / 10;
    }

    inline int get_value(int n) {
        return (n % 100) % 10;
    }
};