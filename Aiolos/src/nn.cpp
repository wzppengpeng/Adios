#include "nn/nn.hpp"

#include "my_string/string.hpp"
#include "log/log.hpp"

#include "util/matrix_args.hpp"
#include "util/op.hpp"


using namespace std;
using wzp::Matrix;

namespace aiolos
{

ReflectionRegister(Classify, NN) regis_nn("nn");

void NN::init(wzp::ConfigParser* config_parser) {
    m_config_parser = config_parser;
    //set params
    m_config_parser->get("eta", m_eta);
    m_config_parser->get("num_iter", m_num_iter);
    m_config_parser->get("batch_size", m_batch_size);
    if(!m_config_parser->get("net_framework", m_net_framework)) {
        wzp::log::fatal("Miss The Defination Of Net");
    }
    layers_setup();
}

void NN::train(wzp::Matrix<Type>& input_matrix, wzp::Matrix<int>& input_label) {
    m_input_matrix = &input_matrix;
    m_input_label = &input_label;
    //init the m_weights
    //train loop
    mean();
    wzp::log::info("Begin To Train");
    train_loop();
}

void NN::train(wzp::Matrix<Type>& input_matrix,
     wzp::Matrix<int>& input_label, wzp::Matrix<Type>& validate_matrix,
     wzp::Matrix<int>& validate_label) {
    m_input_matrix = &input_matrix;
    m_input_label = &input_label;
    m_validate_matrix = &validate_matrix;
    m_validate_label = &validate_label;
}

wzp::Matrix<int> NN::predict(wzp::Matrix<Type>& predict_matrix) {
    Matrix<int> res(predict_matrix.rows(), 1);
    auto minus_mean_data = m_mean_data * -1;
    op::add_vector(predict_matrix, minus_mean_data.row_at(0));
    m_nets[0]->forward(predict_matrix);
    for(size_t i = 1; i < m_nets.size(); ++i) {
        m_nets[i]->forward(m_nets[i-1]->get_layer_data_output());
    }
    for(size_t i = 0; i < predict_matrix.rows(); ++i) {
        res(i, 0) = m_nets.back()->get_layer_data_output().at(i, 0) > 0.5 ? 1 : 0;
    }
    return res;
}

void NN::dump_model(const char* filename) {
    //save the matrix or weights
}

void NN::restore_model(const char* filename) {
}

void NN::train_loop() {
    for(int i = 0; i < m_num_iter; ++i) {
        train_process(i);
    }
}

void NN::layers_setup() {
    auto net_def = wzp::split_string(m_net_framework, ':');
    m_nets.reserve(net_def.size() + 1);
    //first is the input layer
    m_nets.emplace_back(wzp::Reflection<Layer>::create_shared("input"));
    for(size_t i = 1; i < net_def.size() - 1; ++i) {
        m_nets.emplace_back(wzp::Reflection<Layer>::create_shared("fc"));
    }
    m_nets.emplace_back(wzp::Reflection<Layer>::create_shared("output"));
    //init
    m_nets[0]->init(std::stoi(net_def[0]), 0, m_eta);
    for(size_t i = 1; i < net_def.size(); ++i) {
        m_nets[i]->init(std::stoi(net_def[i]), std::stoi(net_def[i - 1]), m_eta);
    }
}


void NN::train_process(int iter) {
    for(size_t i = 0; i < m_input_matrix->rows(); i += m_batch_size) {
        //get data
        if(i + m_batch_size > m_input_matrix->rows()) break;
        auto X = m_input_matrix->slice(i, 0, i + m_batch_size, m_input_matrix->cols()); //m * d
        auto Y = std::move(op::convert<Type>(m_input_label->slice(i,
         0, i + m_batch_size, m_input_label->cols()))); //m * 1
        //forward
        m_nets[0]->forward(X);
        for(size_t i = 1; i < m_nets.size(); ++i) {
            m_nets[i]->forward(m_nets[i-1]->get_layer_data_output());
        }
        if(iter % 5 == 4 && i % 50 == 0) {
            wzp::log::info("Iter", iter+1,
             "Batch", i+1, "Loss", compute_loss(m_nets.back()->get_layer_data_output(), Y));
        }
        //back ward and learning
        m_nets.back()->backward(Y);
        m_nets.back()->update();
        for(int i = m_nets.size() - 2; i >= 0; --i) {
            m_nets[i]->backward(m_nets[i+1]->get_layer_diff_output());
            m_nets[i]->update();
        }
    }
}

Type NN::compute_loss(const wzp::Matrix<Type>& Y, const wzp::Matrix<Type>& Y_) {
    assert(Y.rows() == Y_.rows() && Y.cols() == Y_.cols());
    Type sum = 0.;
    for(size_t i = 0; i < Y.rows(); ++i) {
        sum += (MatrixArgs::compute_squre_distance(Y.row_at(i), Y_.row_at(i), Y.cols())) / 2.;
    }
    return sum;
}

void NN::mean() {
    m_mean_data.reshape(1, m_input_matrix->cols());
    for(size_t j = 0; j < m_input_matrix->cols(); ++j) {
        m_mean_data(0, j) = op::col_mean(*m_input_matrix, j);
    }
    auto minus_mean_data = m_mean_data * -1;
    op::add_vector(*m_input_matrix, minus_mean_data.row_at(0));
}

} //aiolos