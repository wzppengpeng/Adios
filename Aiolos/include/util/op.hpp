#ifndef AIOLOS_OP_HPP
#define AIOLOS_OP_HPP
#include <algorithm>
#include <utility>
#include <vector>


#include "container/matrix.hpp"

#include "common.hpp"
/**
 * Some Operations to handle datas
 */
namespace aiolos
{
namespace op
{

/**
 * generate function, to gen a range
 */
template<typename Range>
Range generate(size_t len) {
    //gen 0 ~ len -1
    Range range_container(len);
    auto current = 0;
    std::generate_n(begin(range_container), len-1, [&current]{return ++current; });
    return std::move(range_container);
}

/**
 * arg_min(col), row is in matrix_args
 */
template<typename T>
size_t arg_min(const wzp::Matrix<T>& mat, size_t col_index) {
    assert(col_index < mat.cols());
    size_t min_index = 0;
    for(size_t i = 0; i < mat.rows(); ++i) {
        if(mat(i, col_index) < mat(min_index, col_index))
            min_index = i;
    }
    return min_index;
}

/**
 * arg_max(col), row is in matrix_args
 */
template<typename T>
size_t arg_max(const wzp::Matrix<T>& mat, size_t col_index) {
    assert(col_index < mat.cols());
    size_t max_index = 0;
    for(size_t i = 0; i < mat.rows(); ++i) {
        if(mat(i, col_index) > mat(max_index, col_index))
            max_index = i;
    }
    return max_index;
}

/**
 * arg_min_max(col)
 */
template<typename T>
/**
 * first is arg_min, second is arg_max
 */
std::pair<size_t,size_t> arg_min_max(const wzp::Matrix<T>& mat, size_t col_index) {
    assert(col_index < mat.cols());
    size_t min_index = 0, max_index = 0;
    for(size_t i = 0; i < mat.rows(); ++i) {
        if(mat(i, col_index) < mat(min_index, col_index))
            min_index = i;
        if(mat(i, col_index) > mat(max_index, col_index))
            max_index = i;
    }
    return {min_index, max_index};
}

/**
 * auto norm
 */
inline void auto_norm(wzp::Matrix<Type>& mat) {
    //use multi thread
    auto indexs = generate<std::vector<size_t>>(mat.cols());
    wzp::ParallelForeach(begin(indexs), end(indexs),
     [&mat](size_t col_index) {
        auto min_max = arg_min_max(mat, col_index);
        auto min_val = mat(min_max.first, col_index);
        auto max_val = mat(min_max.second, col_index);
        auto dis = max_val - min_val;
        for(size_t i = 0; i < mat.rows(); ++i) {
            if(dis == 0)
                mat(i, col_index) = mat(i, col_index) - min_val;
            else {
                mat(i, col_index) =
                 (mat(i, col_index) - min_val) / dis;
            }
        }
     });
}

} //op
} //aiolos

#endif /*AIOLOS_OP_HPP*/