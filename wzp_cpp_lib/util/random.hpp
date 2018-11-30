#ifndef WZP_UTIL_RANDOM_HPP_
#define WZP_UTIL_RANDOM_HPP_

#include <vector>
#include <random>
#include <algorithm>
#include <type_traits>

namespace wzp {

/**
 * How to use:
 * 1. random_uniform(bottom, up) : input a [bottom, up] range, output a value in uniform distribution, will auto get the type needed
 * 2. random_uniform(bottom, up, N) : input a [bottom, up] range with the vector size N, output a vector of uniform random values
 * 3. random normal
 * 4. random bernoulli
 */

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
        unsigned seed = __get_random_seed())
        : rand_(seed), distribution_(begin, end) {}

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
        unsigned seed = __get_random_seed())
        : rand_(seed), distribution_(begin, end) {}

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
        unsigned seed = __get_random_seed())
        : rand_(seed), distribution_(mean, val) {}

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
        unsigned seed = __get_random_seed())
        : rand_(seed), distribution_(prob) {}

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
    return Random(a).Next();
}

template<typename Random> inline
typename Random::res_type random(typename Random::data_type a, typename Random::data_type b) {
    return Random(a, b).Next();
}

/**
 * The help function to generate a list of numbers
 * input first / second is the range based value, the last is the size
 */
template<typename Random> inline
std::vector<typename Random::res_type> random_range(typename Random::data_type a, size_t N) {
    return Random(a).NextRange(N);
}

// the two generate type
template<typename Random> inline
std::vector<typename Random::res_type> random_range(typename Random::data_type a, typename Random::data_type b, size_t N) {
    return Radnom(a, b).NextRange(N);
}


/**
 * The Help function to use Uniform Random Class
 * judge if the input is interger or floats
 */

/**
 * random generate a value in uniform type
 * @param  bottom the bottom value (int or size_t)
 * @param  up     the up value of the random (int or size_t)
 * @return        return a random value between [bottom, up]
 */
template<typename T> inline
typename std::enable_if<std::is_integral<T>::value, T>::type random_uniform(T bottom, T up) {
    return wzp::RandomInt<T>(bottom, up).Next();
}

/**
 * random generate a value in uniform floats type
 * @param  bottom the bottom value (float or double)
 * @param  up     the up value (float or double)
 * @return        return a random value between [bottom, up]
 */
template<typename T> inline
typename std::enable_if<std::is_floating_point<T>::value, T>::type random_uniform(T bottom, T up) {
    return wzp::RandomFloats<T>(bottom, up).Next();
}

/**
 * random generate a range value in uniform type
 * @param  bottom the bottom value (int or size_t)
 * @param  up     the up value of the random (int or size_t)
 * @param   N     the size of range
 * @return        return a random range value between [bottom, up]
 */
template<typename T> inline
std::vector<typename std::enable_if<std::is_integral<T>::value, T>::type> random_uniform(T bottom, T up, size_t N) {
    return wzp::RandomInt<T>(bottom, up).NextRange(N);
}

/**
 * random generate a range value in uniform type
 * @param  bottom the bottom value (float or double)
 * @param  up     the up value of the random (float or double)
 * @param   N     the size of range
 * @return        return a random range value between [bottom, up]
 */
template<typename T> inline
std::vector<typename std::enable_if<std::is_floating_point<T>::value, T>::type> random_uniform(T bottom, T up, size_t N) {
    return wzp::RandomFloats<T>(bottom, up).NextRange(N);
}

} //wzp

#endif /*WZP_UTIL_RANDOM_HPP_*/