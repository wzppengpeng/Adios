#ifndef AIOLOS_INPUT_LAYER_HPP
#define AIOLOS_INPUT_LAYER_HPP

#include "layer.hpp"

#include "reflection/reflection.hpp"

namespace aiolos
{

class InputLayer : public Layer
{

public:
    InputLayer() = default;

    void init(int num_neural, int pre, Type eta) override;

    void forward(const wzp::Matrix<Type>& pre_data) override;

    void backward(const wzp::Matrix<Type>& post_diff) override;

    void update() override;

    const wzp::Matrix<Type>& get_layer_data_output() const override;

private:

    //reflection
    static ReflectionRegister(Layer, InputLayer) regis_layer_input_layer;

};


} //aiolos


#endif /*AIOLOS_INPUT_LAYER_HPP*/