
#include <unordered_map>

#include "knn/knn.hpp"
#include "log/log.hpp"

#include "util/matrix_args.hpp"
#include "util/op.hpp"



using namespace std;
using namespace wzp;

namespace aiolos
{

//the regis
ReflectionRegister(Classify, KNN) KNN::regis_knn("knn");

void KNN::init(wzp::ConfigParser* config_parser) {
    m_config_parser = config_parser;
    if(!m_config_parser->get<int>("k", k)) {
        k = 3;
        log::error("Miss Arg K of KNN");
    }
}

void KNN::train(wzp::Matrix<Type>& input_matrix, wzp::Matrix<int>& input_label) {
    m_in = input_matrix;
    m_ll = input_label;
    m_input_matrix = &m_in;
    m_input_label = &m_ll;
    if(k > m_input_matrix->rows()) {
        k = m_input_matrix->rows();
        log::error("the K is set to be too large");
    }
    //auto norm
    op::auto_norm(*m_input_matrix);
}

void KNN::train(wzp::Matrix<Type>& input_matrix,
     wzp::Matrix<int>& input_label, wzp::Matrix<Type>& validate_matrix,
     wzp::Matrix<int>& validate_label) {
    //null
}

Matrix<int> KNN::predict(wzp::Matrix<Type>& predict_matrix) {
    //use a pair
    Matrix<int> res(predict_matrix.rows(), 1);
    op::auto_norm(predict_matrix);
    for(size_t row = 0; row < predict_matrix.rows(); ++row) {
        auto indexs = op::generate<vector<int>>(m_input_matrix->rows());
        //result
        vector<pair<Type, int>> distances(m_input_matrix->rows());
        //judge use multi thread
        if(predict_matrix.cols() < 100) {
            std::for_each(begin(indexs), end(indexs), [this, &predict_matrix, row, &distances](int index){
                predict_one(predict_matrix, row, index, distances);
            });
        }
        else {
            ParallelForeach(begin(indexs), end(indexs), [this, &predict_matrix, row, &distances](int index){
                predict_one(predict_matrix, row, index, distances);
            });
        }
        res(row, 0) = compute_max_cnt_index(distances);
    }
    return res;
}

void KNN::dump_model(const char* filename) {
    log::fatal("KNN do not have model to dump, please set mode to both");
}

void KNN::restore_model(const char* filename) {
    log::fatal("KNN do not have model to restore, please set mode to both");
}

void KNN::predict_one(const wzp::Matrix<Type>& predict_matrix, int row, int index,
     std::vector<pair<Type, int>>& distances) {
    auto a = predict_matrix.row_at(row);
    auto b = m_input_matrix->row_at(index);
    distances[index].first = MatrixArgs::compute_distance(a, b, m_input_matrix->cols());
    distances[index].second = m_input_label->at(index, 0);
}

int KNN::compute_max_cnt_index(std::vector<pair<Type, int>>& distances) {
    //nth_element
    nth_element(begin(distances), begin(distances) + k,
     end(distances), [](const pair<Type, int>& left, const pair<Type, int>& right) {
        return left.first < right.first;
    });
    unordered_map<int, int> cnt;
    for(int i = 0; i < k; ++i) {
        auto it = cnt.find(distances[i].second);
        if(it != cnt.end()) {
            ++it->second;
        }
        else {
            cnt.emplace(distances[i].second, 1);
        }
    }
    int max_key = cnt.begin()->first;
    for(auto& p : cnt) {
        if(p.second > cnt[max_key]) {
            max_key = p.first;
        }
    }
    return max_key;
}

}