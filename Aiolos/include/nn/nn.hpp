#ifndef AIOLOS_NN_HPP_
#define AIOLOS_NN_HPP_

#include "classify.hpp"

#include "reflection/reflection.hpp"

#include "layer.hpp"

namespace aiolos
{

class NN : public Classify
{

public:
    /**
     * Constructor
     */
    NN() = default;

    /**
     * init function
     */
    void init(wzp::ConfigParser* config_parser) override;

    /**
     * train function
     */
    void train(wzp::Matrix<Type>& input_matrix, wzp::Matrix<int>& input_label) override;

    void train(wzp::Matrix<Type>& input_matrix,
     wzp::Matrix<int>& input_label, wzp::Matrix<Type>& validate_matrix,
     wzp::Matrix<int>& validate_label) override;

    /**
     * prediction
     */
    wzp::Matrix<int> predict(wzp::Matrix<Type>& predict_matrix) override;

    /**
     * save and resotre model
     */
    void dump_model(const char* filename) override;

    void restore_model(const char* filename) override;

private:
    //two data put in train
    wzp::ConfigParser* m_config_parser;

    wzp::Matrix<Type>* m_input_matrix;
    wzp::Matrix<int>* m_input_label;
    wzp::Matrix<Type>* m_validate_matrix;
    wzp::Matrix<int>* m_validate_label;

    /**
     * Private Members for nn
    */
    // wzp::Matrix<Type> m_labels;

    //params for trainning
    int m_num_iter = 10;
    Type m_eta = 0.001;
    int m_batch_size = 10;
    std::string m_net_framework;

    wzp::Matrix<Type> m_mean_data;

    //the nets
    std::vector<std::shared_ptr<Layer>> m_nets;


    static ReflectionRegister(Classify, NN) regis_nn;

private:

    void layers_setup();

    void train_loop();

    void train_process(int iter);

    Type compute_loss(const wzp::Matrix<Type>& Y, const wzp::Matrix<Type>& Y_);

    void mean();

};

} //aiolos

#endif /*AIOLOS_NN_HPP_*/