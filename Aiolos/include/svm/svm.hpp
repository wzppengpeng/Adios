#ifndef AIOLOS_SVM_HPP
#define AIOLOS_SVM_HPP

#include "classify.hpp"

#include <memory>

#include "reflection/reflection.hpp"
#include "svm/smo.hpp"

namespace aiolos
{

class SVM : public Classify {

public:
    SVM() = default;

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
     * SVM DATAMEMBERS
     */
    std::unique_ptr<SMO> m_smo_runner;
    std::string smo_method = "smo_platt";

    int m_silent = 1;
    //some params
    Type m_C = 0.6;
    Type m_toler = 0.001;
    int m_max_iter = 40;

    //the alphas
    Type m_b = 0.0;
    wzp::Matrix<Type> m_W;

    /**
     * Reflection
     */
    static ReflectionRegister(Classify, SVM) regis_svm;

};


} //aiolos

#endif /*AIOLOS_SVM_HPP*/