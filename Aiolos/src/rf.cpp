#include "random_forest/rf.hpp"

#include <unordered_set>

using namespace std;
using wzp::Matrix;

#include "util/op.hpp"

#include "log/log.hpp"
#include "container/array_args.hpp"


namespace aiolos
{

//regis
ReflectionRegister(Classify, RandomForest) regis_random_forest("random_forest");

void RandomForest::init(wzp::ConfigParser* config_parser) {
    m_config_parser = config_parser;
    m_config_parser->get("tree_nums", m_tree_numbers);
    m_config_parser->get("choose_nums", m_choose_nums);
    wzp::log::info("The Tree Numbers is", m_tree_numbers);
    m_tree.resize(m_tree_numbers);
}

void RandomForest::train(wzp::Matrix<Type>& input_matrix, wzp::Matrix<int>& input_label) {
    m_input_matrix = &input_matrix;
    m_input_label = &input_label;
    //train by create function
    wzp::ParallelRange(m_tree_numbers,
        [this](size_t k) {
        auto datasets = op::booststrap(*m_input_matrix, *m_input_label);
        auto axises = op::generate<vector<size_t>>(datasets.first.cols());
        m_tree[k] = create_tree(datasets.first, datasets.second, axises);
    });
}

void RandomForest::train(wzp::Matrix<Type>& input_matrix,
     wzp::Matrix<int>& input_label, wzp::Matrix<Type>& validate_matrix,
     wzp::Matrix<int>& validate_label) {
    m_input_matrix = &input_matrix;
    m_input_label = &input_label;
    m_validate_matrix = &validate_matrix;
    m_validate_label = &validate_label;
    //
}

wzp::Matrix<int> RandomForest::predict(wzp::Matrix<Type>& predict_matrix) {
    Matrix<int> res(predict_matrix.rows(), 1);
    for(size_t i = 0; i < predict_matrix.rows(); ++i) {
        vector<int> predict_proposals(m_tree_numbers);
        for(size_t k = 0; k < m_tree_numbers; ++k) {
            predict_proposals[k] = classify(predict_matrix, i, k);
        }
        res(i, 0) = majority(predict_proposals);
    }
    return std::move(res);
}

void RandomForest::dump_model(const char* filename) {
    wzp::log::fatal("Simple Tree Need Not To Dump Model, Please Use Both Mode");
}

void RandomForest::restore_model(const char* filename) {
    wzp::log::fatal("Simple Tree Need Not To Dump Model, Please Use Both Mode");
}

size_t RandomForest::choose_best_feature_to_split(const wzp::Matrix<Type>& data_set,
     const wzp::Matrix<int>& labels) {
    assert(!data_set.empty() && data_set.rows() == labels.rows());
    size_t num_features = data_set.cols();//choose log2(d)
    Type base_shannon_ent = op::cal_shannon_ent(labels);
    // auto axises = op::generate<vector<size_t>>(num_features);
    auto choose_num = m_choose_nums < 0 ? math::aiolos_log(data_set.cols(), 2) : m_choose_nums;
    size_t length =  choose_num > 1.0 ? static_cast<size_t>(choose_num) : 1;
    if(length > num_features) length = num_features;
    auto axises = op::random_choose(data_set.cols(), length);
    vector<Type> info_gain_vec(num_features, 0.0);
    std::for_each(begin(axises), end(axises),
     [this, &data_set, &labels, &info_gain_vec, base_shannon_ent](size_t axis) {
        auto new_ent = choose_best_feature_to_split_each(data_set, labels, axis);
        info_gain_vec[axis] = base_shannon_ent - new_ent;
     });
    //find the max index
    return wzp::array_args<Type>::arg_max(info_gain_vec);
}

Type RandomForest::choose_best_feature_to_split_each(const wzp::Matrix<Type>& data_set,
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
DecisionTree<int> RandomForest::create_tree(wzp::Matrix<Type> data_set,
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
int RandomForest::classify(const wzp::Matrix<Type>& predict_matrix, size_t row_index, size_t tree_index) {
    auto trees = m_tree[tree_index].get_trees();
    while(!trees->is_val) {
        size_t next = 0;
        for(size_t i = 0; i < trees->m_tree_modes.size(); ++i) {
            if(trees->axis >= predict_matrix.cols()) {
                wzp::log::fatal("The Predict Data Col not match", trees->axis);
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


int RandomForest::majority(const vector<int>& propasal) {
    unordered_map<int, int> counts;
    int max_l = propasal[0];
    for(auto l : propasal) {
        ++counts[l];
        if(counts[max_l] < counts[l]) {
            max_l = l;
        }
    }
    return max_l;
}

} //aiolos