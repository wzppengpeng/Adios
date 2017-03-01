#ifndef AIOLOS_LINEAR_REGRESSION_HPP
#define AIOLOS_LINEAR_REGRESSION_HPP

#include "regression.hpp"

#include "reflection/reflection.hpp"

namespace aiolos
{

class StandLinearRegression : public Regression {
public:
    /**
     * Constructor
     */
    StandLinearRegression() = default;

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

    /**
     * StandLinearRegression Members
     */
    //the weights
    wzp::Matrix<Type> m_ws;

    /**
     * Reflection
     */
    static ReflectionRegister(Regression, StandLinearRegression) regis_stand_linear_regression;

private:
    /**
     * the train process
     */
    void train_process();

};

} //aiolos

#endif /*AIOLOS_LINEAR_REGRESSION_HPP*/