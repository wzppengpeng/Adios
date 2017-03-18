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