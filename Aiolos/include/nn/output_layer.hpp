#ifndef AIOLOS_NN_OUTPUT_LAYER_HPP
#define AIOLOS_NN_OUTPUT_LAYER_HPP

#include "layer.hpp"

#include "reflection/reflection.hpp"

namespace aiolos
{

class OutputLayer : public Layer
{

public:
    OutputLayer() = default;

    void init(int num_neural, int pre, Type eta) override;

    void forward(const wzp::Matrix<Type>& pre_data) override;

    void backward(const wzp::Matrix<Type>& post_diff) override;

    void update() override;


private:
    static ReflectionRegister(Layer, OutputLayer) regis_layer_output_layer;

    wzp::Matrix<Type> tmp_diff_from_post;

private:
    void active_beta();

    void back_active_beta();

    Type active_fun(Type input, ActiveFucntion type);

    Type active_fun_back(Type input, ActiveFucntion type);

};

} //aiolos


#endif /*AIOLOS_NN_OUTPUT_LAYER_HPP*/