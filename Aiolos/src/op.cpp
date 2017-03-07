#include "util/op.hpp"


#include <unordered_set>

#include "util/aiolos_math.hpp"

using namespace std;
using namespace wzp;

namespace aiolos {
namespace op {

Type cal_shannon_ent(const wzp::Matrix<int>& label) {
    //declare a map to store label values
    unordered_map<int, int> label_cnt;
    for(size_t i = 0; i < label.rows(); ++i) {
        auto it = label_cnt.find(label(i, 0));
        if(it != label_cnt.end())
            ++it->second;
        else
            label_cnt.emplace(label(i, 0), 1);
    }
    Type shannon_ent = 0.0;
    for(auto& p : label_cnt) {
        Type prob = static_cast<Type>(p.second) / static_cast<Type>(label.rows());
        shannon_ent -= prob * math::aiolos_log(prob, 2);
    }
    return shannon_ent;
}

int majority_cnt(const wzp::Matrix<int>& label) {
    //use a map to store label and its count
    if(label.empty()) return 0;
    unordered_map<int, int> label_cnt;
    for(size_t i = 0; i < label.rows(); ++i) {
        auto it = label_cnt.find(label(i, 0));
        if(it != label_cnt.end())
            ++it->second;
        else
            label_cnt.emplace(label(i, 0), 0);
    }
    int max_label_key = label_cnt.begin()->first;
    for(auto& p : label_cnt) {
        if(p.second > label_cnt[max_label_key])
            max_label_key = p.first;
    }
    return max_label_key;
}

int count_label(const wzp::Matrix<int>& label, int val) {
    auto sum = 0;
    for(size_t i = 0; i < label.rows(); ++i) {
        if(label(i, 0) == val) ++sum;
    }
    return sum;
}

std::unordered_map<int, int> count_label(const wzp::Matrix<int>& label) {
    unordered_map<int, int> label_cnt;
    for(size_t i = 0; i < label.rows(); ++i) {
        auto it = label_cnt.find(label(i, 0));
        if(it != label_cnt.end()) {
            ++it->second;
        }
        else {
            label_cnt.emplace(label(i, 0), 1);
        }
    }
    return std::move(label_cnt);
}


std::vector<size_t> random_choose(size_t max_axis, size_t length) {
    assert(length >= 1 && length <= max_axis);
    vector<size_t> select_random_axises(length);
    unordered_set<size_t> axises_have_been_selected;
    for(size_t i = 0; i < length; ++i) {
        auto select_axis = math::aiolos_random_int<size_t>(0, max_axis - 1);
        while(axises_have_been_selected.count(select_axis)) {
            select_axis = math::aiolos_random_int<size_t>(0, max_axis - 1);
        }
        select_random_axises[i] = select_axis;
        axises_have_been_selected.emplace(select_axis);
    }
    return std::move(select_random_axises);
}

} //op
} //aiolos