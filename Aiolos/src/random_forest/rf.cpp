#include "random_forest/rf.hpp"

#include <unordered_set>

using namespace std;
using wzp::Matrix;

#include "trees/cart_model_tree_impl.hpp"

#include "log/log.hpp"
#include "container/array_args.hpp"


namespace aiolos
{

//regis
ReflectionRegister(Classify, RandomForest) regis_random_forest("random_forest");

void RandomForest::init(wzp::ConfigParser* config_parser) {
    m_config_parser = config_parser;
    m_config_parser->get("tree_nums", m_tree_numbers);
    m_config_parser->get("tol_s", tol_s);
    m_config_parser->get("tol_n", tol_n);
    wzp::log::info("The Tree Numbers is", m_tree_numbers);
    m_cart_tree.resize(m_tree_numbers);
}

void RandomForest::train(wzp::Matrix<Type>& input_matrix, wzp::Matrix<int>& input_label) {
    m_input_matrix = &input_matrix;
    m_input_label = &input_label;
    train_process();
}

void RandomForest::train(wzp::Matrix<Type>& input_matrix,
     wzp::Matrix<int>& input_label, wzp::Matrix<Type>& validate_matrix,
     wzp::Matrix<int>& validate_label) {
    m_input_matrix = &input_matrix;
    m_input_label = &input_label;
    m_validate_matrix = &validate_matrix;
    m_validate_label = &validate_label;
}

wzp::Matrix<int> RandomForest::predict(wzp::Matrix<Type>& predict_matrix) {
    Matrix<int> res(predict_matrix.rows(), 1);
    for(size_t i = 0; i < predict_matrix.rows(); ++i) {
        vector<int> predict_proposals(m_tree_numbers);
        for(size_t k = 0; k < m_tree_numbers; ++k) {
            predict_proposals[k] = cart::tree_forecast(m_cart_tree[k].get_trees(),
         predict_matrix.row_at(i), predict_matrix.cols(),
          [](const shared_ptr<CartTreeNode<Type>>& ptr, const Type* vals, size_t len) {
            return cart::cart_tree_eval(ptr, vals, len);
          });
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

void RandomForest::train_process() {
    //convert the input label
    m_input_label_convert = std::move(op::convert<Type>(*m_input_label));
    wzp::ParallelRange(m_tree_numbers,
        [this](size_t k) {
        auto datasets = op::booststrap(*m_input_matrix, m_input_label_convert);
        m_cart_tree[k] = cart::create_tree(datasets.first, datasets.second,
            [](const Matrix<Type>& mat, const Matrix<Type>& labels){
                return cart::gini_leaf(mat, labels);
            },
            [](const Matrix<Type>& mat, const Matrix<Type>& labels){
                return cart::gini_err(mat, labels);
            }, tol_s, tol_n);
    });
}

} //aiolos