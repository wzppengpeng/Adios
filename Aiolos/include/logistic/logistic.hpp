#ifndef AIOLOS_LOGISTIC_HPP
#define AIOLOS_LOGISTIC_HPP

#include "classify.hpp"

#include "reflection/reflection.hpp"

namespace aiolos
{

class Logistic : public Classify {
public:
    /**
     * Constructor
     */
    Logistic() = default;

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
     * private members for logistic
     */
    //core model! weights
    wzp::Matrix<Type> m_weights; //first is 1
    //params for train
    Type m_alpha = 0.01; // the learning rate
    Type m_eta = 0.1; //the base
    size_t m_num_iter = 100; // the iter num;
    Type m_stop_error = 1e-7; // the stop error

    int m_silent = 0;
    size_t m_snapshot = 10; // the interval to print result

    //reflection
    //
    static ReflectionRegister(Classify, Logistic) regis_logistic;

private:
    //train function, return the loss
    Type train_one_iter(size_t iter);

    void update(Type alpha, Type error, size_t rand_index);

    void train_loop(bool has_validate = false);

    //classify function for single one
    int classify(const wzp::Matrix<Type>& predict_matrix, size_t row_index);

};


} //aiolos


#endif /*AIOLOS_LOGISTIC_HPP*/