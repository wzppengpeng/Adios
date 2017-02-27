#ifndef AIOLOS_OP_HPP
#define AIOLOS_OP_HPP
#include <algorithm>
#include <utility>
#include <vector>
#include <unordered_map>


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
 * Get nonzero list
 */
template<typename T>
std::vector<size_t> nonzero(const wzp::Matrix<T>& m, size_t col) {
    std::vector<size_t> nonzero_list;
    for(size_t i = 0; i < m.rows(); ++i) {
        if(m(i, col) != 0) nonzero_list.emplace_back(i);
    }
    return nonzero_list;
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

/**
 * For Decision Tree, Some Calculation
 */
/**
 * calcute shannon ent
 */
/**
 * [cal_shannon_ent description]
 * @param  label n*1 vector
 * @return       float or double ent
 */
Type cal_shannon_ent(const wzp::Matrix<int>& label);

/**
 * count the majoryty in labels
 * @param  labels
 * @return majoruty count label
 */
int majority_cnt(const wzp::Matrix<int>& label);

/**
 * [count_label description]
 * @param  label
 * @return count result
 */
int count_label(const wzp::Matrix<int>& label, int val);

/**
 * count all labels, return a map to store each label and its count
 */
std::unordered_map<int, int> count_label(const wzp::Matrix<int>& label);

template<typename T>
/**
 * compute a sum of a row in matrix
 * @param  mat
 * @param  row_index a row index
 * @return           sum result
 */
T row_sum(const wzp::Matrix<T>& mat, size_t row_index) {
    assert(row_index < mat.rows());
    T sum = 0;
    for(size_t j = 0; j < mat.cols(); ++j) {
        sum += mat(row_index, j);
    }
    return sum;
}

/**
 * Split Data Set
 */
template<typename T>
/**
 * a fucntion template to split a matrix by col
 */
std::pair<wzp::Matrix<T>, wzp::Matrix<int>> split_matrix_by_col(const wzp::Matrix<T>& data_set,
    const wzp::Matrix<int>& labels, size_t axis, T value) {
    assert(data_set.cols() > axis && data_set.rows() == labels.rows());
    using std::vector;
    vector<vector<T>> ret_data_set;
    vector<vector<int>> ret_labels;
    for(size_t i = 0; i < data_set.rows(); ++i) {
        if(data_set(i, axis) == value) {
            vector<T> reduced_feature_vec;
            vector<int> reduced_label_vec(1);
            reduced_feature_vec.reserve(data_set.cols() - 1);
            size_t j = 0;
            for(; j < axis; ++j) {
                reduced_feature_vec.emplace_back(data_set(i, j));
            }
            for(j = axis + 1; j < data_set.cols(); ++j) {
                reduced_feature_vec.emplace_back(data_set(i, j));
            }
            reduced_label_vec[0] = labels(i, 0);
            ret_data_set.emplace_back(std::move(reduced_feature_vec));
            ret_labels.emplace_back(std::move(reduced_label_vec));
        }
    }
    if(ret_data_set.empty()) return {wzp::Matrix<T>(), wzp::Matrix<int>()};
    else {
        return {std::move(wzp::Matrix<T>(ret_data_set.size(),
         ret_data_set[0].size(), ret_data_set)),
          std::move(wzp::Matrix<int>(ret_labels.size(), (size_t)1, ret_labels))};
    }
}


/**
 * Some Matrix Operations
 */
template<typename R = Type, typename T, typename U>
wzp::Matrix<R> multiply(const wzp::Matrix<T>& left, const wzp::Matrix<U>& right) {
    assert(left.rows() == right.rows());
    wzp::Matrix<R> res(left.rows(), 1);
    for(size_t i = 0; i < left.rows(); ++i) {
        res.at(i, 0) = (static_cast<R>(left.at(i, 0)) * static_cast<R>(right.at(i, 0)));
    }
    return std::move(res);
}

} //op

} //aiolos

#endif /*AIOLOS_OP_HPP*/