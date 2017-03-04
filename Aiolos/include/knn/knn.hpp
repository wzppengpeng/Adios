#ifndef AIOLOS_KNN_HPP
#define AIOLOS_KNN_HPP
#include <utility>
#include <vector>

#include "classify.hpp"

#include "reflection/reflection.hpp"

namespace aiolos
{

class KNN : public Classify {
public:
    /**
     * Constructor
     */
    KNN() = default;

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
    wzp::Matrix<Type> m_in;
    wzp::Matrix<int>* m_input_label;
    wzp::Matrix<int> m_ll;
    wzp::Matrix<Type>* m_validate_matrix;
    wzp::Matrix<int>* m_validate_label;

    int k;// the k nn

    //reflection
    static ReflectionRegister(Classify, KNN) regis_knn;

private:
    void predict_one(const wzp::Matrix<Type>& predict_matrix, int row, int index,
     std::vector<pair<Type, int>>& distances);

    int compute_max_cnt_index(std::vector<pair<Type, int>>& distances);

};

}

#endif /*AIOLOS_KNN_HPP*/