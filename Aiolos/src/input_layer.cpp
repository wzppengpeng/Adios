#include "nn/input_layer.hpp"

#include "log/log.hpp"


using namespace std;

using wzp::Matrix;

namespace aiolos
{

ReflectionRegister(Layer, InputLayer) regis_layer_input_layer("input");

void InputLayer::init(int num_neural, int pre, Type eta) {
    l = num_neural;
    d = pre;
    assert(d == 0);
    //init the matrix
    //weights should be d * l
    //this layer has no weights or bias
}

void InputLayer::forward(const wzp::Matrix<Type>& pre_data) {
    m_pre_data = &pre_data;
}

void InputLayer::backward(const wzp::Matrix<Type>& post_diff) {
    //null as it need not to calculate the diff
}

void InputLayer::update() {
    //null as it has no paramters to update
}

const wzp::Matrix<Type>& InputLayer::get_layer_data_output() const {
    return *m_pre_data;
}

} //aiolos