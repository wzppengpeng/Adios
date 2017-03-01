#include "linear_regression/linear_regression.hpp"


using namespace std;
using namespace wzp;


namespace aiolos
{

//reflection
ReflectionRegister(Regression, StandLinearRegression) regis_stand_linear_regression("stand_linear_regression");

void StandLinearRegression::init(wzp::ConfigParser* config_parser) {
    m_config_parser = config_parser;
}

void StandLinearRegression::train(wzp::Matrix<Type>& input_matrix, wzp::Matrix<Type>& input_label) {
    m_input_matrix = &input_matrix;
    m_input_label = &input_label;
    train_process();
}

void StandLinearRegression::train(wzp::Matrix<Type>& input_matrix,
     wzp::Matrix<Type>& input_label, wzp::Matrix<Type>& validate_matrix,
     wzp::Matrix<Type>& validate_label) {
    m_input_matrix = &input_matrix;
    m_input_label = &input_label;
    m_validate_matrix = &validate_matrix;
    m_validate_label = &validate_label;
}

wzp::Matrix<Type> StandLinearRegression::predict(wzp::Matrix<Type>& predict_matrix) {
    auto res = predict_matrix * (m_ws);
    return res;
}

void StandLinearRegression::dump_model(const char* filename) {
    //save the m_W
    m_ws.to_bin_file(filename);
}

void StandLinearRegression::restore_model(const char* filename) {
    m_ws.read_bin_file(filename);
}

void StandLinearRegression::train_process() {
    wzp::Matrix<Type> X(*m_input_matrix); //n * m
    auto XTX = (X.t()) * (*m_input_matrix);
    auto ws = (XTX.inv()) * (X * (*m_input_label));
    m_ws = std::move(ws);
}

} //aiolos