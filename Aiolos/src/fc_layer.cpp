#include "nn/fc_layer.hpp"

// #include "util/aiolos_math.hpp"
#include "util/op.hpp"

using namespace std;

using wzp::Matrix;

namespace aiolos
{

ReflectionRegister(Layer, FcLayer) regis_layer_fc_layer("fc");

void FcLayer::init(int num_neural, int pre, Type eta) {
    l = num_neural;
    d = pre;
    m_eta = eta;
    //init the weights and bias
    m_weights.reshape(d, l);
    m_bias.reshape(1, l);
    math::aiolos_randn_matrix(m_weights, 0, 1);
    math::aiolos_randn_matrix(m_bias, 0, 1);
}


void FcLayer::forward(const wzp::Matrix<Type>& pre_data) {
    //save the pre_data
    m_pre_data = &pre_data;
    m_post_data = std::move((pre_data * m_weights));
    op::add_vector(m_post_data, m_bias.row_at(0));
    active_beta();
}

void FcLayer::backward(const wzp::Matrix<Type>& post_diff) {
    //save the post diff
    assert(post_diff.rows() == m_post_data.rows() && post_diff.cols() == m_post_data.cols());
    m_post_diff = &post_diff;
    tmp_diff_from_post = post_diff;
    //active back
    back_active_beta();
    m_pre_diff = std::move(tmp_diff_from_post * (m_weights.t_()));
}

void FcLayer::update() {
    //save eta
    auto delta_w = ((m_pre_data->t_()) * tmp_diff_from_post) * m_eta;
    m_weights += delta_w;
    //the bias
    auto delta_b = (Matrix<Type>(1, m_pre_data->rows()) * tmp_diff_from_post);
    m_bias += delta_b;
}

void FcLayer::active_beta() {
    for(size_t i = 0; i < m_post_data.rows(); ++i) {
        for(size_t j = 0; j < m_post_data.cols(); ++j) {
            m_post_data(i, j) = active_fun(m_post_data(i, j), m_active_fun_type);
        }
    }
}

void FcLayer::back_active_beta() {
    for(size_t i = 0; i < m_post_data.rows(); ++i) {
        for(size_t j = 0; j < m_post_data.cols(); ++j) {
            tmp_diff_from_post(i, j) *= active_fun_back(m_post_data(i, j), m_active_fun_type);
        }
    }
}

Type FcLayer::active_fun(Type input, ActiveFucntion type) {
    if(type == ActiveFucntion::Relu) {
        return input > 0. ? input : 0.;
    }
    else {
        return math::aiolos_sigmoid(input);
    }
}

Type FcLayer::active_fun_back(Type input, ActiveFucntion type) {
    if(type == ActiveFucntion::Relu) {
        return input > 0. ? 1 : 0;
    }
    else {
        return input * (1 - input);
    }
}

} //aiolos