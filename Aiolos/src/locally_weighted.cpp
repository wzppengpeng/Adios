#include "linear_regression/locally_weighted.hpp"

#include <cmath>

#include "log/log.hpp"

#include "util/matrix_args.hpp"
#include "util/op.hpp"

using namespace std;
using wzp::Matrix;

namespace aiolos
{

ReflectionRegister(Regression, LocallyWeighted) regis_locally_weighted("locally_weighted");

void LocallyWeighted::init(wzp::ConfigParser* config_parser) {
    m_config_parser = config_parser;
    config_parser->get("k", k);
    config_parser->get("silent", m_is_silent);
}

void LocallyWeighted::train(wzp::Matrix<Type>& input_matrix, wzp::Matrix<Type>& input_label) {
    m_input_matrix = &input_matrix;
    m_input_label = &input_label;
    m = input_matrix.rows();
    n = input_matrix.cols();
    m_X = std::move(input_matrix);
    m_Y = std::move(input_label);
}

void LocallyWeighted::train(wzp::Matrix<Type>& input_matrix,
     wzp::Matrix<Type>& input_label, wzp::Matrix<Type>& validate_matrix,
     wzp::Matrix<Type>& validate_label) {
    m_input_matrix = &input_matrix;
    m_input_label = &input_label;
    m_validate_matrix = &validate_matrix;
    m_validate_label = &validate_label;
    m = input_matrix.rows();
    n = input_matrix.cols();
}

wzp::Matrix<Type> LocallyWeighted::predict(wzp::Matrix<Type>& predict_matrix) {
    Matrix<Type> res(predict_matrix.rows(), 1);
    for(size_t i = 0; i < predict_matrix.rows(); ++i) {
        if(m_is_silent == 0) {
            wzp::log::info("now process:", i+1, "total:", predict_matrix.rows());
        }
        auto row_vec = predict_matrix.slice(i, 0, i+1, predict_matrix.cols());
        res(i, 0) = lwlr(row_vec);
    }
    return res;
}

void LocallyWeighted::dump_model(const char* filename) {
    wzp::log::error("Can Not Use Mode Train, Please Use Both");
}

void LocallyWeighted::restore_model(const char* filename) {
    wzp::log::fatal("Can Not Use Mode Predict, Please Use Both");
}

Type LocallyWeighted::lwlr(wzp::Matrix<Type>& test_point) {
    //create weights matrix
    wzp::Matrix<Type> weights(m, wzp::MatrixType::Eyes);//m * m
    //use multi thread to spead this process
    auto row_indexs = op::generate<vector<size_t>>(m);
    std::for_each(row_indexs.begin(), row_indexs.end(),
    // wzp::ParallelForeach(row_indexs.begin(), row_indexs.end(),
     [&test_point, this, &weights](size_t j) {
        auto diff_mat = test_point - (m_X.slice(j, 0, j+1, n));
        weights(j, j) = exp((MatrixArgs::vector_product(diff_mat.row_at(0),
            diff_mat.row_at(0), n)) / (-2.0 * k * k));
    });
    Matrix<Type> XT(m_X);
    XT.t(); //n * m
    auto XTX = (XT) * (weights * (m_X));
    auto ws = (XTX.inv()) * (XT * (weights * (m_Y)));
    return (test_point * ws).at(0, 0);
}

} //aiolos