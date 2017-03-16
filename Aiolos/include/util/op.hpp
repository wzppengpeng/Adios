#ifndef AIOLOS_OP_HPP
#define AIOLOS_OP_HPP
#include <algorithm>
#include <utility>
#include <vector>
#include <unordered_map>
// #include <unordered_set>


#include "container/matrix.hpp"

#include "aiolos_math.hpp"

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
inline Range generate(size_t len) {
    //gen 0 ~ len -1
    Range range_container(len);
    auto current = 0;
    std::generate_n(begin(range_container), len-1, [&current]{return ++current; });
    return std::move(range_container);
}

template<typename T>
inline wzp::RangeImpl<T> xrange(T len) {
    return wzp::range(len);
}

/**
 * randome select length axis from 0 ~ max_axis
 */
std::vector<size_t> random_choose(size_t max_axis, size_t length);

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

template<typename T>
inline T row_mean(const wzp::Matrix<T>& mat, size_t row_index) {
    return row_sum(mat, row_index) / static_cast<T>(mat.cols());
}

template<typename T>
T col_sum(const wzp::Matrix<T>& mat, size_t col_index = 0) {
    assert(col_index < mat.cols());
    T sum = 0;
    for(size_t i = 0; i < mat.rows(); ++i) {
        sum += mat(i, col_index);
    }
    return sum;
}

template<typename T>
inline T col_mean(const wzp::Matrix<T>& mat, size_t col_index) {
    return col_sum(mat, col_index) / static_cast<T>(mat.rows());
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

template<typename U, typename T>
std::vector<size_t> split_by_col_value(const wzp::Matrix<U>& data_set, T val, size_t col_index = 0) {
    vector<size_t> indexs;
    // indexs.reserve(data_set.rows());
    for(size_t i = 0; i < data_set.rows(); ++i) {
        if(static_cast<T>(data_set(i, col_index)) == val) {
            indexs.emplace_back(i);
        }
    }
    return std::move(indexs);
}


template<typename T>
wzp::Matrix<T> get_new_mat_by_rows(const wzp::Matrix<T>& data_set, const std::vector<size_t>& indexs) {
    wzp::Matrix<T> new_data_set(indexs.size(), data_set.cols());
    for(size_t i = 0; i < indexs.size(); ++i) {
        for(size_t j = 0; j < data_set.cols(); ++j) {
            new_data_set(i, j) = data_set(indexs[i], j);
        }
    }
    return std::move(new_data_set);
}

/**
 * booststrap, random choose a new dataset
 */
template<typename T, typename I>
std::pair<wzp::Matrix<T>, wzp::Matrix<I>> booststrap(const wzp::Matrix<T>& data_set, const wzp::Matrix<I>& labels) {
    wzp::Matrix<T> son_data_set(data_set.rows(), data_set.cols());
    wzp::Matrix<I> son_labels(labels.rows(), labels.cols());
    for(size_t i = 0; i < data_set.rows(); ++i) {
        auto choose_row = math::aiolos_random_int<size_t>(0, data_set.rows() - 1);
        son_data_set.place(data_set.row_at(choose_row), i);
        son_labels.place(labels.row_at(choose_row), i);
    }
    return {son_data_set, son_labels};
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

template<typename R = Type, typename T, typename U>
/**
 * two col vector vector_product
 * @param  left
 * @param  right [description]
 * @return       [description]
 */
R vector_product(const wzp::Matrix<T>& left, const wzp::Matrix<U>& right) {
    assert(left.rows() == right.rows());
    R sum = 0.0;
    for(size_t i = 0; i < left.rows(); ++i) {
        sum += (static_cast<R>(left(i, 0)) * static_cast<R>(right(i, 0)));
    }
    return sum;
}

/**
 * add a vector values to a matrix for every row
 */
template<typename T>
void add_vector(wzp::Matrix<T>& mat, const T* vec) {
    for(size_t i = 0; i < mat.rows(); ++i) {
        for(size_t j = 0; j < mat.cols(); ++j) {
            mat(i, j) += vec[j];
        }
    }
}

/**
 * Convert A Mat To Another Type Mat
 */
template<typename R, typename T>
wzp::Matrix<R> convert(const wzp::Matrix<T>& raw_mat) {
    wzp::Matrix<R> res(raw_mat.rows(), raw_mat.cols());
    for(size_t i = 0; i < raw_mat.rows(); ++i) {
        for(size_t j = 0; j < raw_mat.cols(); ++j) {
            res(i, j) = static_cast<R>(raw_mat(i, j));
        }
    }
    return std::move(res);
}

} //op

} //aiolos

#endif /*AIOLOS_OP_HPP*/