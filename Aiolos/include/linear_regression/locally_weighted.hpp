#ifndef AIOLOS_LOCALLY_WEIGHTED_LINEAR_REGRESSION_HPP
#define AIOLOS_LOCALLY_WEIGHTED_LINEAR_REGRESSION_HPP

#include "regression.hpp"

#include "reflection/reflection.hpp"

namespace aiolos
{

class LocallyWeighted : public Regression {
public:
    /**
     * Constructor
     */
    LocallyWeighted() = default;

    /**
     * init function
     */
    void init(wzp::ConfigParser* config_parser) override;

    /**
     * train function
     */
    void train(wzp::Matrix<Type>& input_matrix, wzp::Matrix<Type>& input_label) override;

    void train(wzp::Matrix<Type>& input_matrix,
     wzp::Matrix<Type>& input_label, wzp::Matrix<Type>& validate_matrix,
     wzp::Matrix<Type>& validate_label) override;

    /**
     * prediction
     */
    wzp::Matrix<Type> predict(wzp::Matrix<Type>& predict_matrix) override;

    /**
     * save and resotre model
     */
    void dump_model(const char* filename) override;

    void restore_model(const char* filename) override;

private:
    //two data put in train
    wzp::ConfigParser* m_config_parser;

    wzp::Matrix<Type>* m_input_matrix;
    wzp::Matrix<Type>* m_input_label;
    wzp::Matrix<Type>* m_validate_matrix;
    wzp::Matrix<Type>* m_validate_label;

    size_t m;
    size_t n;

    int m_is_silent = 0;

    /**
     * Locally Weighted Linear Regression Members
     */
    //kernal size
    Type k = 1.0;

    wzp::Matrix<Type> m_X;
    wzp::Matrix<Type> m_Y;

    /**
     * Reflection
     */
    static ReflectionRegister(Regression, LocallyWeighted) regis_locally_weighted;

private:
    /**
     * the LWLR function
     */
    Type lwlr(wzp::Matrix<Type>& test_point); //1 * n, and use the k

};


} //aiolos


#endif /*AIOLOS_LOCALLY_WEIGHTED_LINEAR_REGRESSION_HPP*/