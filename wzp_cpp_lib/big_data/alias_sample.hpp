#ifndef WZP_BIG_DATA_ALIAS_SAMPLE_HPP_
#define WZP_BIG_DATA_ALIAS_SAMPLE_HPP_

#include <vector>
#include <unordered_map>
#include <queue>

#include "wzp_cpp_lib/util/random.hpp"

namespace wzp {

template<typename Key>
class AliasSampler {
    // declarations of alias queue
    using Queue = std::queue<std::pair<Key, double>>;

public:
    // constructor
    AliasSampler(const std::unordered_map<Key, size_t>& count_dict) {
        InitTable(count_dict);
    }

    // sample function to get the Key
    const Key& Sample() const {
        auto pos = wzp::random<RandomInt<size_t>>(0, names_.size() - 1);
        auto prob = wzp::random<RandomFloats<double>>(0., 1.);
        return prob <= probs_[pos] ? names_[pos] : choosers_[pos];
    }

    inline const Key& operator() () const {
        return Sample();
    }

private:
    // private member functions
    // construct the alias table
    void InitTable(const std::unordered_map<Key, size_t>& count_dict) {
        auto N = count_dict.size();
        init_vectors(N);
        size_t count = 0;
        for(const auto& p : count_dict) {
            count += p.second;
        }
        Queue que1, que2;
        // init
        for(const auto& p : count_dict) {
            auto prob = static_cast<double>(p.second * N) / static_cast<double>(count);
            InsertAccordProb(p.first, prob, que1, que2);
        }
        while(!(que1.empty() && que2.empty())) {
            const auto& ele1 = que1.front();
            const auto& ele2 = que2.front();
            probs_.emplace_back(ele2.second);
            choosers_.emplace_back(ele1.first);
            names_.emplace_back(ele2.first);
            InsertAccordProb(ele1.first, ele1.second - (1. - ele2.second), que1, que2);
            que1.pop();
            que2.pop();
        }
    }

    void InsertAccordProb(const Key& name, double prob, Queue& que1, Queue& que2) {
        if(prob > 1.0 + 1e-6) {
            que1.push({name, prob});
        } else if(prob < 1.0 - 1e-6) {
            que2.push({name, prob});
        } else {
            probs_.emplace_back(1.0);
            choosers_.emplace_back(name);
            names_.emplace_back(name);
        }
    }

    inline void init_vectors(size_t N) {
        names_.reserve(N);
        choosers_.reserve(N);
        probs_.reserve(N);
    }

private:
    // three vector lists to contain the prob, the main name and the alias chooser
    std::vector<Key> names_; //the main name
    std::vector<Key> choosers_; // the alias chooser
    std::vector<double> probs_; // the main prob
};

} //wzp

#endif /*WZP_BIG_DATA_ALIAS_SAMPLE_HPP_*/