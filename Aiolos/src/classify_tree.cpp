#include "trees/classify_tree.hpp"

#include <unordered_set>

#include "log/log.hpp"
#include "container/array_args.hpp"

#include "util/op.hpp"

using namespace std;
using namespace wzp;

namespace aiolos
{

//the regis
ReflectionRegister(Classify, ClassifyTree) ClassifyTree::regis_classify_tree("classify_tree");

/**
 * [init the module
 * @param config_parser
 */
void ClassifyTree::init(wzp::ConfigParser* config_parser) {
    m_config_parser = config_parser;
}

void ClassifyTree::train(wzp::Matrix<Type>& input_matrix, wzp::Matrix<int>& input_label) {
    m_input_matrix = &input_matrix;
    m_input_label = &input_label;
    //train by create function
    auto axises = op::generate<vector<size_t>>(input_matrix.cols());
    m_tree = create_tree(*m_input_matrix, *m_input_label, axises);
    //check if print
    int silent;
    if(m_config_parser->get<int>("silent", silent) && silent == 1) {
        log::info("print this tree");
        m_tree.print_tree();
    }
}

void ClassifyTree::train(wzp::Matrix<Type>& input_matrix,
     wzp::Matrix<int>& input_label, wzp::Matrix<Type>& validate_matrix,
     wzp::Matrix<int>& validate_label) {

}

wzp::Matrix<int> ClassifyTree::predict(wzp::Matrix<Type>& predict_matrix) {
    Matrix<int> res(predict_matrix.rows(), 1);
    for(size_t i = 0; i < predict_matrix.rows(); ++i) {
        res(i, 0) = classify(predict_matrix, i);
    }
    return std::move(res);
}

void ClassifyTree::dump_model(const char* filename) {

}

void ClassifyTree::restore_model(const char* filename) {

}

/**
 * Private Functions
 */
size_t ClassifyTree::choose_best_feature_to_split(const wzp::Matrix<Type>& data_set,
     const wzp::Matrix<int>& labels) {
    assert(!data_set.empty() && data_set.rows() == labels.rows());
    size_t num_features = data_set.cols();
    Type base_shannon_ent = op::cal_shannon_ent(labels);
    auto axises = op::generate<vector<size_t>>(num_features);
    vector<Type> info_gain_vec(num_features);
    ParallelForeach(begin(axises), end(axises),
     [this, &data_set, &labels, &info_gain_vec, base_shannon_ent](size_t axis) {
        auto new_ent = choose_best_feature_to_split_each(data_set, labels, axis);
        info_gain_vec[axis] = base_shannon_ent - new_ent;
     });
    //find the max index
    return array_args<Type>::arg_max(info_gain_vec);
}

Type ClassifyTree::choose_best_feature_to_split_each(const wzp::Matrix<Type>& data_set,
     const wzp::Matrix<int>& labels, size_t axis) {
    assert(axis < data_set.cols());
    //def a set to store feature values
    unordered_set<Type> unique_fea_list;
    for(size_t i = 0; i < data_set.rows(); ++i) {
        unique_fea_list.emplace(data_set(i, axis));
    }
    Type new_ent = 0.0;
    for(auto val : unique_fea_list) {
        auto sub_data_set = std::move(op::split_matrix_by_col(data_set,
         labels, axis, val));
        Type prob = (Type)sub_data_set.first.rows() / ((Type)data_set.rows());
        new_ent += (prob * op::cal_shannon_ent(sub_data_set.second));
    }
    return new_ent;
}

/**
 * create tree
 */
DecisionTree<int> ClassifyTree::create_tree(wzp::Matrix<Type> data_set,
     wzp::Matrix<int> labels, std::vector<size_t> axises) {
    assert(!data_set.empty() && data_set.rows() == labels.rows());
    //check if labels are all same
    if(op::count_label(labels, labels(0, 0)) == labels.rows()) {
        //all the same
        return DecisionTree<int>(labels(0, 0));
    }
    //walk over all features
    if(data_set.cols() == 0) {
        return DecisionTree<int>(op::majority_cnt(labels));
    }
    //choose best fea to split
    auto best_fea_axis = choose_best_feature_to_split(data_set, labels);
    DecisionTree<int> temp_tree(axises[best_fea_axis], false);
    axises.erase(axises.begin() + best_fea_axis);// delete the split col axis
    //get the unique feature values
    unordered_set<Type> unique_fea_list;
    for(size_t i = 0; i < data_set.rows(); ++i) {
        unique_fea_list.emplace(data_set(i, best_fea_axis));
    }
    for(auto val : unique_fea_list) {
        //add son decison tree
        auto sub_data_set = op::split_matrix_by_col(data_set, labels, best_fea_axis, val);
        temp_tree.append([val](Type v)->bool{return v == val; },
            create_tree(std::move(sub_data_set.first), std::move(sub_data_set.second),
            axises));
    }
    return temp_tree;
}

/**
 * classify function
 */
int ClassifyTree::classify(const wzp::Matrix<Type>& predict_matrix, size_t row_index) {
    auto trees = m_tree.get_trees();
    while(!trees->is_val) {
        size_t next = 0;
        for(size_t i = 0; i < trees->m_tree_modes.size(); ++i) {
            if(trees->axis >= predict_matrix.cols()) {
                log::fatal("The Predict Data Col not match", trees->axis);
            }
            if(trees->m_tree_modes[i].first(predict_matrix(row_index, trees->axis))) {
                next = i;
                break;
            }
        }
        trees = trees->m_tree_modes[next].second;
    }
    return trees->val;
}

} //aiolos