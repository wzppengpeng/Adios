#ifndef AIOLOS_MATH_HPP
#define AIOLOS_MATH_HPP

#include <cmath>
#include <random>
#include "io.hpp"

#include "common.hpp"

namespace aiolos
{

namespace math
{

/**
 * clip a number, let the value in a right range
 */
template<typename T>
inline T aiolos_clip(T& val, T L, T H) {
    if(val < L) {
        val = L;
    }
    else if (val > H)
    {
        val = H;
    }
    return val;
}

template<typename T = unsigned>
/**
 * generate a randome int
 * @param  begin
 * @param  end [inclusive]
 * @return T
 */
inline T aiolos_random_int(T begin, T end) {
    static std::random_device rd;
    static std::default_random_engine e(rd());
    std::uniform_int_distribution<T> u(begin, end);
    return u(e);
}

template<typename T>
/**
 * generate random float or double
 * @param  begin the begin of range
 * @param  end   the end of range
 * @return       float random value
 */
inline Type aiolos_random_real(T begin, T end) {
    static std::default_random_engine er;
    static std::uniform_real_distribution<Type> ur(begin, end);
    return ur(er);
}

template<typename T = unsigned, typename I>
/**
 * select a radom one except the previous one
 */
inline T aiolos_select_rand(T i, I begin, I end) {
    auto j = i;
    while(j == i) {
        j = aiolos_random_int<T>(begin, end); // includesive
    }
    return j;
}


/**
 * math
 * **************************************************
 */
/**
 * log by any down value
 * @param  val  the value
 * @param  down down value
 * @return      log(val)
 */
inline Type aiolos_log(Type val, int down) {
    return log(val) / log(down);
}


/**
 * sigmoid for single float
 * @param  val
 * @return
 */
inline Type aiolos_sigmoid(Type val) {
    return 1.0 / (1.0 + exp(-val));
}

template<typename Mat>
Mat aiolos_matrix_exp(const Mat& m) {
    Mat res(m.rows(), m.cols());
    for(size_t i = 0; i < m.rows(); ++i) {
        for(size_t j = 0; j < m.cols(); ++j) {
            res(i, j) = exp(m(i, j));
        }
    }
    return std::move(res);
}

template<typename Mat>
Mat aiolos_matrix_sigmoid(const Mat& m) {
    Mat res(m.rows(), m.cols());
    for(size_t i = 0; i < m.rows(); ++i) {
        for(size_t j = 0; j < m.cols(); ++j) {
            res(i, j) = aiolos_sigmoid(m(i, j));
        }
    }
    return std::move(res);
}

} //math

}//aiolos


#endif /*AIOLOS_MATH_HPP*/