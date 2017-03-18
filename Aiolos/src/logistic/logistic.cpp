#include "logistic/logistic.hpp"

#include "log/log.hpp"

#include "util/op.hpp"
#include "util/optimize.hpp"
#include "util/aiolos_math.hpp"
#include "util/matrix_args.hpp"

using namespace std;
using namespace wzp;

namespace aiolos
{

ReflectionRegister(Classify, Logistic) regis_logistic("logistic");

void Logistic::init(wzp::ConfigParser* config_parser) {
    m_config_parser = config_parser;
    //set params
    m_config_parser->get<Type>("alpha", m_alpha);
    m_config_parser->get<size_t>("num_iter", m_num_iter);
    m_config_parser->get<Type>("stop_error", m_stop_error);
    m_config_parser->get<int>("silent", m_silent);
    m_config_parser->get<size_t>("snapshot", m_snapshot);
    m_config_parser->get<Type>("eta", m_eta);
}

void Logistic::train(wzp::Matrix<Type>& input_matrix, wzp::Matrix<int>& input_label) {
    m_input_matrix = &input_matrix;
    m_input_label = &input_label;
    //init the m_weights
    m_weights = std::move(wzp::Matrix<Type>(1, input_matrix.cols() + 1, 0.1));
    //train loop
    train_loop();
}

void Logistic::train(wzp::Matrix<Type>& input_matrix,
     wzp::Matrix<int>& input_label, wzp::Matrix<Type>& validate_matrix,
     wzp::Matrix<int>& validate_label) {
    m_input_matrix = &input_matrix;
    m_input_label = &input_label;
    m_validate_matrix = &validate_matrix;
    m_validate_label = &validate_label;
    //init the m_weights
    m_weights = std::move(wzp::Matrix<Type>(1, input_matrix.cols() + 1, 0.1));
    train_loop(true);
}

wzp::Matrix<int> Logistic::predict(wzp::Matrix<Type>& predict_matrix) {
    assert(predict_matrix.cols() == m_weights.cols() - 1);
    Matrix<int> res(predict_matrix.rows(), 1);
    for(size_t i = 0; i < res.rows(); ++i) {
        res(i, 0) = classify(predict_matrix, i);
    }
    return res;
}

void Logistic::dump_model(const char* filename) {
    //save the matrix or weights
    m_weights.to_bin_file(filename);
}

void Logistic::restore_model(const char* filename) {
    m_weights.read_bin_file(filename);
}

Type Logistic::train_one_iter(size_t iter) {
    //use stoc grad ascent method
    auto indexs = op::generate<vector<size_t>>(m_input_matrix->rows());
    Type error_mean = 0.0;
    for(size_t i : wzp::range(m_input_matrix->rows())) {
        Type alpha = m_eta  / (1.0 + iter + i) + m_alpha;
        size_t rand_index = math::aiolos_random_int<size_t>(0, indexs.size() - 1);
        size_t index = indexs[rand_index];
        Type sum_res = MatrixArgs::vector_product(m_input_matrix->row_at(index),
         m_weights.row_at(0), m_input_matrix->cols());
        sum_res += m_weights(0, m_input_matrix->cols());
        sum_res = math::aiolos_sigmoid(sum_res);
        auto error = optimize::relative_loss<Type>(sum_res, (Type)m_input_label->at(index, 0));
        //update
        update(alpha, error, index);
        indexs.erase(indexs.begin() + rand_index);
        error_mean += abs(error);
    }
    return error_mean;
}

void Logistic::update(Type alpha, Type error, size_t rand_index) {
    //update the weights
    for(size_t j = 0; j < m_weights.cols() - 1; ++j) {
        m_weights(0, j) = m_weights(0, j) + alpha * error * m_input_matrix->at(rand_index, j);
    }
    m_weights(0, m_weights.cols() - 1) =
     m_weights(0, m_weights.cols() - 1) + alpha * error;
}

void Logistic::train_loop(bool has_validate) {
    for(size_t i = 0; i < m_num_iter; ++i) {
        auto error = train_one_iter(i);
        if(m_silent == 0 && i % m_snapshot == 0) {
            if(has_validate) {
                //predict validate matrix
                auto wrong_rate = eval::eval<Type>([this]{ return predict(*m_validate_matrix); },
                 *m_validate_label, LossTypes::Error01);
                log::info("Train Loop", i + 1, "Now The Error On TrainSet is", error,
                    "Error Rate On Validate Dataset is", wrong_rate);
            }
            else {
                log::info("Train Loop", i + 1, "Now The Error is", error);
            }
        }
        if(error < m_stop_error) break;
    }
}

int Logistic::classify(const wzp::Matrix<Type>& predict_matrix, size_t row_index) {
    Type sum_res = MatrixArgs::vector_product(predict_matrix.row_at(row_index),
     m_weights.row_at(0), predict_matrix.cols());
    sum_res += m_weights(0, m_weights(0, m_weights.cols() - 1));
    Type prob = math::aiolos_sigmoid(sum_res);
    return prob > 0.5 ? 1 : 0;
}

} //aiolos