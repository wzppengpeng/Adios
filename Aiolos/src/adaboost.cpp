#include "boost/adaboost.hpp"

#include "log/log.hpp"

#include "util/op.hpp"
#include "util/aiolos_math.hpp"

#include "util/serialize.hpp"

using namespace std;
using wzp::Matrix;

namespace aiolos
{

//regis
ReflectionRegister(Classify, AdaBoost) regis_adaboost("adaboost");

void AdaBoost::init(wzp::ConfigParser* config_parser) {
    m_config_parser = config_parser;
    config_parser->get("silent", m_is_silent);
}

void AdaBoost::train(wzp::Matrix<Type>& input_matrix, wzp::Matrix<int>& input_label) {
    m_input_matrix = &input_matrix;
    m_input_label = &input_label;
    m = m_input_matrix->rows(); n = m_input_matrix->cols();
    int num_iter = 40;
    m_config_parser->get("num_iter", num_iter);
    adaboost_train_process(num_iter);
}

void AdaBoost::train(wzp::Matrix<Type>& input_matrix,
     wzp::Matrix<int>& input_label, wzp::Matrix<Type>& validate_matrix,
     wzp::Matrix<int>& validate_label) {
    m_input_matrix = &input_matrix;
    m_input_label = &input_label;
    m_validate_matrix = &validate_matrix;
    m_validate_label = &validate_label;
}

wzp::Matrix<int> AdaBoost::predict(wzp::Matrix<Type>& predict_matrix) {
    Matrix<int> res(predict_matrix.rows(), 1);
    //TODO
    Matrix<Type> agg_class_est(predict_matrix.rows(), 1);
    for(auto& weak : m_weak_class_arrays) {
        auto class_est = stump_classify(predict_matrix, weak.second);
        agg_class_est += (class_est * weak.first);
    }
    //sign(agg)
    for(size_t i = 0; i < predict_matrix.rows(); ++i) {
        res(i, 0) = agg_class_est(i, 0) > 0.0 ? 1 : -1;
    }
    return res;
}

void AdaBoost::dump_model(const char* filename) {
    //dump the m_weak_class_arrays to disk
    std::string buffer;
    //dump length of vector
    size_t len = m_weak_class_arrays.size();
    wzp::serialize(&buffer, len);
    for(auto& weak : m_weak_class_arrays) {
        wzp::serialize(&buffer, weak.first);
        wzp::serialize(&buffer, weak.second);
    }
    std::ofstream ofile(filename, std::ios::binary);
    ofile.write(buffer.c_str(), sizeof(char) * buffer.size());
    ofile.close();
}

void AdaBoost::restore_model(const char* filename) {
    //restore the len
    ifstream ifile;
    ifile.open(filename);
    std::string buffer((std::istreambuf_iterator<char>(ifile)),
        std::istreambuf_iterator<char>());
    if(buffer.empty()) {
        wzp::log::fatal("Model File Illegal", filename);
    }
    ifile.close();
    size_t len;
    wzp::deserialize(buffer, len);
    buffer = std::move(buffer.substr(sizeof(size_t)));
    m_weak_class_arrays.reserve(len);
    Type alpha;
    Stump<Type> tmp_stump;
    for(size_t i = 0; i < len; ++i) {
        wzp::deserialize(buffer, alpha);
        buffer = std::move(buffer.substr(sizeof(Type)));
        wzp::deserialize(buffer, tmp_stump);
        buffer = std::move(buffer.substr(sizeof(Stump<Type>)));
        m_weak_class_arrays.emplace_back(alpha, tmp_stump);
    }
}

/**
 * Private Functions
 */
wzp::Matrix<Type> AdaBoost::stump_classify(const wzp::Matrix<Type>& data_matrix,
     const Stump<Type>& stump) {
    wzp::Matrix<Type> ret_array(data_matrix.rows(), 1, 1);
    //lt
    if(stump.ineq == 0) {
        ret_array.filter([&](size_t i, size_t j) {
            return data_matrix(i, stump.dim) <= stump.thresh;
        }, -1.0);
    }
    else {
        ret_array.filter([&](size_t i, size_t j) {
            return data_matrix(i, stump.dim) > stump.thresh;
        }, -1.0);
    }
    return std::move(ret_array);
}

std::tuple<Stump<Type>, Type, wzp::Matrix<Type>> AdaBoost::build_stump(wzp::Matrix<Type>& D) {
    //the best stump
    Stump<Type> best_stump; Stump<Type> tmp_stump;
    wzp::Matrix<Type> best_class_est(m, 1);
    Type min_error = 1000.0;
    for(size_t i = 0; i < n; ++i) {
        //iter all features
        auto min_max_indexs = op::arg_min_max(*m_input_matrix, i);
        auto range_min = m_input_matrix->at(min_max_indexs.first, i);
        auto step_size = (m_input_matrix->at(min_max_indexs.second, i) -
            range_min) / (Type)m_num_steps;
        for(int j = -1; j < m_num_steps + 1; ++j) {
            //iter all step vals
            for(short k = 0; k < 2; ++k) {
                //iter the two compare method
                tmp_stump.thresh = range_min + static_cast<Type>(j) * step_size;
                tmp_stump.dim = i; tmp_stump.ineq = k;
                auto predict_vals = stump_classify(*m_input_matrix, tmp_stump);
                wzp::Matrix<Type> err_array(m, 1, 1.0);
                err_array.filter([&predict_vals, this](size_t r, size_t c) {
                    return (int)predict_vals(r, 0) == m_input_label->at(r, 0);
                }, 0.0);
                //compute the weight error rate
                auto weight_error = op::vector_product(D, err_array);
                if(weight_error < min_error) {
                    min_error = weight_error;
                    best_class_est = std::move(predict_vals);
                    best_stump = std::move(tmp_stump);
                }
            }
        }
    }
    return {best_stump, min_error, best_class_est};
}

void AdaBoost::adaboost_train_process(int num_iter) {
    m_weak_class_arrays.reserve(num_iter);
    Matrix<Type> D(m, 1, 1.0/m);
    Matrix<Type> agg_class_est(m, 1, 0);
    for(int i = 0; i < num_iter; ++i) {
        auto ans = build_stump(D);
        //tie these three
        Stump<Type> best_stump = std::move(std::get<0>(ans));
        Type error = std::get<1>(ans);
        wzp::Matrix<Type> class_est = std::move(std::get<2>(ans));
        //compute alpha
        Type alpha = 0.5 * log((1.0 - error) / std::max(error, 1e-16f));
        m_weak_class_arrays.emplace_back(alpha, best_stump);
        auto expon = op::multiply(*m_input_label, class_est);
        expon *= (-1.0 * alpha);
        D = op::multiply(D, math::aiolos_matrix_exp(expon));
        D *= (1.0 / op::col_sum(D));
        agg_class_est += (class_est * alpha);
        //compute wrong ones
        int pred;
        int wrong_sum = 0;
        for(size_t r = 0; r < m; ++r) {
            pred = agg_class_est(r, 0) > 0.0 ? 1 : -1;
            if(pred != m_input_label->at(r, 0)) ++wrong_sum;
        }
        auto error_rate = (Type)wrong_sum / (Type)m;
        if(m_is_silent == 0) {
            wzp::log::info("iter is", i, "error_rate is", error_rate);
        }
        if(error_rate == 0.0) break;
    }
}

} //aiolos