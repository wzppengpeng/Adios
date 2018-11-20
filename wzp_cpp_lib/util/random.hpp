#ifndef WZP_UTIL_RANDOM_HPP_
#define WZP_UTIL_RANDOM_HPP_

#include <vector>
#include <random>
#include <algorithm>

namespace wzp {

// help function
// random get a seed number from device
inline static unsigned __get_random_seed() {
    std::random_device rd;
    return rd();
}

/**
 * The Class to generate random uniform int numbers
 */
template<typename T = unsigned int>
class RandomInt {
public:
    typedef std::mt19937 rand_engine_type;
    typedef T data_type;
    typedef T res_type;
public:
    // constructor, inclusive
    RandomInt(T begin, T end,
        const rand_engine_type& rand = rand_engine_type(__get_random_seed()))
        : rand_(rand), distribution_(begin, end) {}

    inline T Next() {
        return distribution_(rand_);
    }

    std::vector<T> NextRange(size_t N) {
        std::vector<T> random_ints;
        random_ints.reserve(N);
        std::generate_n(std::back_inserter(random_ints), N, [this]{ return this->Next(); });
        return random_ints;
    }

private:
    rand_engine_type rand_;
    std::uniform_int_distribution<T> distribution_;
};

/**
 * The Class to generate random uniform real floats
 */
template<typename T = double>
class RandomFloats {
public:
    typedef std::mt19937 rand_engine_type;
    typedef T data_type;
    typedef T res_type;
public:
    // constructor, inclusive
    RandomFloats(T begin, T end,
        const rand_engine_type& rand = rand_engine_type(__get_random_seed()))
        : rand_(rand), distribution_(begin, end) {}

    inline T Next() {
        return distribution_(rand_);
    }

    std::vector<T> NextRange(size_t N) {
        std::vector<T> random_floats;
        random_floats.reserve(N);
        std::generate_n(std::back_inserter(random_floats), N, [this]{ return this->Next(); });
        return random_floats;
    }

private:
    rand_engine_type rand_;
    std::uniform_real_distribution<T> distribution_;
};

/**
 * The Class to generate random real noraml gauss
 */
template<typename T = double>
class RandomNormal {
public:
    typedef std::mt19937 rand_engine_type;
    typedef T data_type;
    typedef T res_type;
public:
    // constructor, gauss distribution
    RandomNormal(T mean, T val,
        const rand_engine_type& rand = rand_engine_type(__get_random_seed()))
        : rand_(rand), distribution_(mean, val) {}

    inline T Next() {
        return distribution_(rand_);
    }

    std::vector<T> NextRange(size_t N) {
        std::vector<T> random_ints;
        random_ints.reserve(N);
        std::generate_n(std::back_inserter(random_ints), N, [this]{ return this->Next(); });
        return random_ints;
    }

private:
    rand_engine_type rand_;
    std::normal_distribution<T> distribution_;
};

/**
 * The Class to generate random bernoulli
 */
template<typename T = double>
class RandomBernoulli {
public:
    typedef std::mt19937 rand_engine_type;
    typedef T data_type;
    typedef bool res_type;
public:
    // constructor, gauss distribution
    RandomBernoulli(T prob,
        const rand_engine_type& rand = rand_engine_type(__get_random_seed()))
        : rand_(rand), distribution_(prob) {}

    inline bool Next() {
        return distribution_(rand_);
    }

    std::vector<bool> NextRange(size_t N) {
        std::vector<bool> random_ints;
        random_ints.reserve(N);
        std::generate_n(std::back_inserter(random_ints), N, [this]{ return this->Next(); });
        return random_ints;
    }

private:
    rand_engine_type rand_;
    std::bernoulli_distribution distribution_;
};

/**
 * the help function to generate one single random value
 * input first / second is the valid inputs
 */
template<typename Random> inline
typename Random::res_type random(typename Random::data_type a) {
    Random rand(a); // construct
    return rand.Next();
}

template<typename Random> inline
typename Random::res_type random(typename Random::data_type a, typename Random::data_type b) {
    Random rand(a, b);
    return rand.Next();
}

/**
 * The help function to generate a list of numbers
 * input first / second is the range based value, the last is the size
 */
template<typename Random> inline
std::vector<typename Random::res_type> random_range(typename Random::data_type a, size_t N) {
    Random rand(a); // construct
    return rand.NextRange(N);
}

// the two generate type
template<typename Random> inline
std::vector<typename Random::res_type> random_range(typename Random::data_type a, typename Random::data_type b, size_t N) {
    Random rand(a, b); // construct
    return rand.NextRange(N);
}

} //wzp

#endif /*WZP_UTIL_RANDOM_HPP_*/