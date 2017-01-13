#ifndef AIOLOS_CLASSIFY_TREE_HPP
#define AIOLOS_CLASSIFY_TREE_HPP

#include "classify.hpp"

#include <unordered_map>

#include "reflection/reflection.hpp"

#include "trees/tree_struct.hpp"

namespace aiolos
{

class ClassifyTree : public Classify {

public:
    /**
     * constructor
     */
    ClassifyTree() = default;

    /**
     * init function
     */
    void init(wzp::ConfigParser* config_parser) override;

    /**
     * void train function
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
    /*menbers*/
    wzp::ConfigParser* m_config_parser;

    wzp::Matrix<Type>* m_input_matrix;
    wzp::Matrix<int>* m_input_label;
    wzp::Matrix<Type>* m_validate_matrix;
    wzp::Matrix<int>* m_validate_label;

    /**
     * Personal menbers
     */
    //Decison Tree
    DecisionTree<int> m_tree;

    //a map to store std::shared_ptr<TreeNode> -> axis value
    //only use mode = train

    /**
     * reflection
     */
    static ReflectionRegister(Classify, ClassifyTree) regis_classify_tree;

private:
    /**
     * Private Functions
     */
    size_t choose_best_feature_to_split(const wzp::Matrix<Type>& data_set,
     const wzp::Matrix<int>& labels);
    Type choose_best_feature_to_split_each(const wzp::Matrix<Type>& data_set,
     const wzp::Matrix<int>& labels, size_t axis);

    //create decison tree
    DecisionTree<int> create_tree(wzp::Matrix<Type> data_set,
     wzp::Matrix<int> labels, std::vector<size_t> axises);

    //classify function
    int classify(const wzp::Matrix<Type>& predict_matrix, size_t row_index);

};


} //aiolos


#endif /*AIOLOS_CLASSIFY_TREE_HPP*/