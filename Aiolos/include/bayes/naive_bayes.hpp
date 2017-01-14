#ifndef NAIVE_BAYES_HPP
#define NAIVE_BAYES_HPP

#include <unordered_map>

#include "classify.hpp"

#include "reflection/reflection.hpp"

namespace aiolos
{

class NaiveBayes : public Classify {
public:
    NaiveBayes() = default;

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
     * Naive Bayes Members
     */
    //the p(ci)
    std::unordered_map<int, Type> m_pClass; // the prob for each class

    //the p(w|ci)
    std::unordered_map<int, size_t> m_class_index; // key is class, value is the col index
    vector<std::unordered_map<Type, size_t>> m_fea_index; // key is each col's feature, value is the index in m_pW;
    wzp::Matrix<Type> m_pW; // row is for each class(ie = class numbers), col is each feature for this class prob

    /////////////

    //reflection
    static ReflectionRegister(Classify, NaiveBayes) regis_naive_bayes;

private:
    /**
     * Private Functions
     */
    // train function, to get the above naive members
    void train_naive_bayes();

    size_t compute_class_numbers();

    //preidct one
    int classify(const wzp::Matrix<Type>& predict_matrix, size_t row_index);
};


} //aiolos


#endif /*NAIVE_BAYES_HPP*/