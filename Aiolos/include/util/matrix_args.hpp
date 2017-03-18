#ifndef AIOLOS_MATRIX_ARGS
#define AIOLOS_MATRIX_ARGS

#include <cmath>

#include <algorithm>

#include "common.hpp"

// #include "container/matrix.hpp"

namespace aiolos
{

class MatrixArgs
{

public:
    /**
     * [vector_product]
     * @param  a   vector
     * @param  b   vector
     * @param  len length of vector
     * @return     float result
     */
    inline static Type vector_product(const Type* a, const Type* b, size_t len) {
        Type sum = 0.0;
        for(size_t i = 0; i < len; ++i) {
            sum += a[i] * b[i];
        }
        return sum;
    }

    /**
     * [compute_squre_distance description]
     * @param  a   point
     * @param  b   point
     * @param  len length
     * @return     double
     */
    inline static Type compute_squre_distance(const Type* a, const Type* b, size_t len) {
        Type sum = 0.0;
        for(size_t i = 0; i < len; ++i) {
            sum += ((a[i] - b[i]) * (a[i] - b[i]));
        }
        return sum;
    }

    /**
     * [compute_distance description]
     * @param  a point
     * @param  b ponit
     * @return  float or double
     */
    inline static Type compute_distance(const Type* a, const Type* b, size_t len, int p = 2) {
        if(p == 2) return sqrt(MatrixArgs::compute_squre_distance(a, b, len));
        Type sum = 0.;
        if(p == 1) {
            for(size_t i = 0; i < len; ++i) {
                sum += std::abs(a[i] - b[i]);
            }
        }
        else {
            for(size_t i = 0; i < len; ++i) {
                sum = std::max(std::abs(a[i] - b[i]), sum);
            }
        }
        return sum;
    }

    /**
     * [arg_max description]
     * @param  a   point
     * @param  len length
     * @return     max_index
     */
    inline static size_t arg_max(const Type* a, size_t len) {
        if(len == 0) return 0;
        size_t max_index = 0;
        for(size_t i = 0; i < len; ++i) {
            if(a[i] > a[max_index]) max_index = i;
        }
        return max_index;
    }

    /**
     * [arg_min description]
     * @param  a   point
     * @param  len length
     * @return     min index
     */
    inline static size_t arg_min(const Type* a, size_t len) {
        if(len == 0) return 0;
        size_t min_index = 0;
        for(size_t i = 0; i < len; ++i) {
            if(a[i] < a[min_index]) min_index = i;
        }
        return min_index;
    }

};


}//ailos

#endif