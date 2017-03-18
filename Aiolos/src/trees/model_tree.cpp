#include "trees/model_tree.hpp"


using namespace std;
using wzp::Matrix;

namespace aiolos
{

//reflection
ReflectionRegister(Regression, ModelTreeRegression) regis_model_tree("model_tree");

void ModelTreeRegression::init(wzp::ConfigParser* config_parser) {
    m_config_parser = config_parser;
    config_parser->get("silent", m_is_silent);
    config_parser->get("tol_s", tol_s);
    config_parser->get("tol_n", tol_n);
}

void ModelTreeRegression::train(wzp::Matrix<Type>& input_matrix, wzp::Matrix<Type>& input_label) {
    m_input_matrix = &input_matrix;
    m_input_label = &input_label;
    m = input_matrix.rows(); n = input_matrix.cols();
    train_process();
}

void ModelTreeRegression::train(wzp::Matrix<Type>& input_matrix,
     wzp::Matrix<Type>& input_label, wzp::Matrix<Type>& validate_matrix,
     wzp::Matrix<Type>& validate_label) {
    m_input_matrix = &input_matrix;
    m_input_label = &input_label;
    m_validate_matrix = &validate_matrix;
    m_validate_label = &validate_label;
    m = input_matrix.rows(); n = input_matrix.cols();
}

wzp::Matrix<Type> ModelTreeRegression::predict(wzp::Matrix<Type>& predict_matrix) {
    Matrix<Type> res(predict_matrix.rows(), 1);
    for(size_t i = 0; i < predict_matrix.rows(); ++i) {
        res(i, 0) = cart::tree_forecast(m_root_tree.get_trees(),
         predict_matrix.row_at(i), predict_matrix.cols(),
          [](const shared_ptr<CartTreeNode<Type>>& ptr, const Type* vals, size_t len) {
            return cart::model_tree_eval(ptr, vals, len);
          });
    }
    return res;
}

void ModelTreeRegression::dump_model(const char* filename) {
    //save the m_W
}

void ModelTreeRegression::restore_model(const char* filename) {
}

void ModelTreeRegression::train_process() {
    m_root_tree = cart::create_model_tree(*m_input_matrix, *m_input_label,
     [](const Matrix<Type>& mat, const Matrix<Type>& labels) {
        return cart::model_leaf(mat, labels);
     }, [](const Matrix<Type>&mat, const Matrix<Type>& labels) {
        return cart::model_err(mat, labels);
     }, tol_s, tol_n);
}



} //aiolos