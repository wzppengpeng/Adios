#ifndef AIOLOS_NN_LAYER_HPP
#define AIOLOS_NN_LAYER_HPP

#include "container/matrix.hpp"

// #include "reflection/reflection.hpp"

#include "common.hpp"


namespace aiolos
{

/**
 * The Active Function Type
 */
enum class ActiveFucntion
{
    Sigmoid,
    Relu
};


class Layer
{

public:

    /**
     * Destructor
     */
    virtual ~Layer() {}


    /**
     * Layer set up
     */
    virtual void init(int num_neural, int pre, Type eta) = 0;

    /**
     * forward , input the pre layer's data
     */
    virtual void forward(const wzp::Matrix<Type>& pre_data) = 0;

    /**
     * backward, input the post layer's diff
     */
    virtual void backward(const wzp::Matrix<Type>& post_diff) = 0;

    /**
     * update the weights and bias, when backward
     */
    virtual void update() = 0;

    /**
     * get data functions
     */
    virtual const wzp::Matrix<Type>& get_layer_data_output() const { return m_post_data; }

    virtual const wzp::Matrix<Type>& get_layer_diff_output() const { return m_pre_diff; }

    /**
     * get weights and bias
     */
    virtual wzp::Matrix<Type>& get_weights() { return m_weights; }
    virtual wzp::Matrix<Type>& get_bias() { return m_bias; }

protected:
    const wzp::Matrix<Type>* m_pre_data;
    const wzp::Matrix<Type>* m_post_diff;

    /**
     * forward need to compute the output of this layer
     */
    wzp::Matrix<Type> m_post_data;

    /**
     * backward need to compute the output diff of this layer
     */
    wzp::Matrix<Type> m_pre_diff;

    //params
    wzp::Matrix<Type> m_weights;
    wzp::Matrix<Type> m_bias;

    //Active Function Type
    ActiveFucntion m_active_fun_type = ActiveFucntion::Sigmoid;

    //the nums of nerual
    int l;
    int d;

    //the learning rate
    Type m_eta = 0.001;

};


} //aiolos

#endif /*AIOLOS_NN_LAYER_HPP*/
