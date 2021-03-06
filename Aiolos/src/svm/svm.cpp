#include "svm/svm.hpp"

#include "log/log.hpp"

#include "svm/def.hpp"

using namespace std;
using wzp::Matrix;

namespace aiolos
{

//reflection
ReflectionRegister(Classify, SVM) regis_svm("svm");

//init function
void SVM::init(wzp::ConfigParser* config_parser) {
    m_config_parser = config_parser;
    //get silent
    config_parser->get("silent", m_silent);
    config_parser->get("smo_method", smo_method);
    m_smo_runner = std::move(wzp::Reflection<SMO>::create_unique(smo_method));
    //set the params from config file
    config_parser->get("C", m_C);
    config_parser->get("toler", m_toler);
    config_parser->get("max_iter", m_max_iter);
}

/**
 * train interface
 */
void SVM::train(wzp::Matrix<Type>& input_matrix, wzp::Matrix<int>& input_label) {
    m_input_matrix = &input_matrix;
    m_input_label = &input_label;
    bool is_silent = (m_silent > 0);
    auto res = m_smo_runner->run(m_config_parser, *m_input_matrix, *m_input_label, m_C, m_toler, m_max_iter, is_silent);
    m_b = res.first;
    m_W = calWs(res.second, *m_input_matrix, *m_input_label);
}

void SVM::train(wzp::Matrix<Type>& input_matrix,
     wzp::Matrix<int>& input_label, wzp::Matrix<Type>& validate_matrix,
     wzp::Matrix<int>& validate_label) {
    m_input_matrix = &input_matrix;
    m_input_label = &input_label;
    m_validate_matrix = &validate_matrix;
    m_validate_label = &validate_label;
}

/**
 * predict interface
 */
wzp::Matrix<int> SVM::predict(wzp::Matrix<Type>& predict_matrix) {
    Matrix<int> res(predict_matrix.rows(), 1);
    for(size_t i = 0; i < predict_matrix.rows(); ++i) {
        auto pred = (predict_matrix.slice(i, 0, i + 1, predict_matrix.cols()) * m_W).at(0, 0) + m_b;
        if(pred > 0) res(i, 0) = 1;
        else res(i, 0) = -1;
    }
    return res;
}

/**
 * IO interface
 */
void SVM::dump_model(const char* filename) {
    //add a b in the last
    m_W.reshape(m_W.rows() + 1, 1);
    m_W(m_W.rows() - 1, 0) = m_b;
    m_W.to_bin_file(filename);
}

void SVM::restore_model(const char* filename) {
    m_W.read_bin_file(filename);
    m_b = m_W(m_W.rows() - 1, 0);
    m_W.reshape(m_W.rows() - 1, 1);
}

} //aiolos