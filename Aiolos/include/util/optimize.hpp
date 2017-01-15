#ifndef AIOLOS_OPTIMIZE_HPP
#define AIOLOS_OPTIMIZE_HPP

#include "container/matrix.hpp"

namespace aiolos
{

enum class LossTypes {
    Error01,
    RelativeLoss,
    MeanSqureError
};

namespace optimize
{

using wzp::Matrix;
/**
 * Define Loss
 */
//0-1 error
template<typename T>
T error_0_1(const wzp::Matrix<T>& predict_labels,
 const wzp::Matrix<T>& labels) {
    assert(predict_labels.cols() == 1 && labels.cols() == 1
     && predict_labels.rows() == labels.rows());
    T error_num = 0;
    for(size_t i = 0; i < labels.rows(); ++i) {
        if(predict_labels(i, 0) != labels(i, 0)) ++error_num;
    }
    return error_num;
}

//relative loss
template<typename T>
inline T relative_loss(T predict_label, T label) {
    return label - predict_label;
}

template<typename T>
wzp::Matrix<T> relative_loss(const Matrix<T>& predict_labels,
 const Matrix<T>& labels) {
    assert(predict_labels.rows() == labels.rows() &&
        predict_labels.cols() == labels.cols());
    wzp::Matrix<T> res(labels.rows(), labels.cols());
    for(size_t i = 0; i < labels.rows(); ++i) {
        for(size_t j = 0; j < labels.cols(); ++j) {
            res(i, j) = labels(i, j) - predict_labels(i, j);
        }
    }
    return std::move(res);
}


} //optimize

namespace eval
{

/**
 * eval the validate rate
 */
template<typename T = float, typename Function, typename I>
T eval(const Function& fun, const wzp::Matrix<I>& labels, LossTypes type) {
    auto predict_labels = fun();
    if(type == LossTypes::Error01) {
        return static_cast<T>(optimize::error_0_1(predict_labels, labels)) /
        static_cast<T>(labels.rows());
    }
    else {
        return T();
    }
}

} //eval

} //aiolos


#endif /*AIOLOS_OPTIMIZE_HPP*/