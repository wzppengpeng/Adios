#ifndef AIOLOS_ADABOOST_HPP
#define AIOLOS_ADABOOST_HPP

#include "classify.hpp"

#include <tuple>

#include "reflection/reflection.hpp"

#include "trees/tree_struct.hpp"


namespace aiolos
{

class AdaBoost : public Classify {
public:
    AdaBoost() = default;

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

    size_t m;
    size_t n;

    /**
     * AdaBoost Members
     */
    //the num steps, segments numbers
    int m_num_steps = 10;
    int m_is_silent = 1;

    std::vector<pair<Type, Stump<Type>>> m_weak_class_arrays; // {alpha, stump(dim, val, ineq)}

    /**
     * Relfections
     */
    static ReflectionRegister(Classify, AdaBoost) regis_adaboost;

private:
    /**
     * use a single stump to classify
     */
    wzp::Matrix<Type> stump_classify(const wzp::Matrix<Type>& data_matrix,
     const Stump<Type>& stump);

    /**
     * build the stump by data array and class labels
     */
    std::tuple<Stump<Type>, Type, wzp::Matrix<Type>> build_stump(wzp::Matrix<Type>& D);

    /**
     * train process
     */
    void adaboost_train_process(int num_iter);

};


} //aiolos


#endif /*AIOLOS_ADABOOST_HPP*/