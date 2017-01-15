#ifndef AIOLOS_MATH_HPP
#define AIOLOS_MATH_HPP

#include <cmath>
#include <random>

#include "common.hpp"

namespace aiolos
{

namespace math
{

/**
 * log by any down value
 * @param  val  the value
 * @param  down down value
 * @return      log(val)
 */
inline Type aiolos_log(Type val, int down) {
    return log(val) / log(down);
}

template<typename T = unsigned>
/**
 * generate a randome int
 * @param  begin
 * @param  end [inclusive]
 * @return T
 */
inline T aiolos_random_int(int begin, int end) {
    std::random_device rd;
    std::default_random_engine e(rd());
    std::uniform_int_distribution<T> u(begin, end);
    return u(e);
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