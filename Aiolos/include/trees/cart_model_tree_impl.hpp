#ifndef AIOLOS_CART_MODEL_TREE_HPP
#define AIOLOS_CART_MODEL_TREE_HPP

#include <unordered_set>

#include "tree_struct.hpp"

#include "util/op.hpp"

namespace aiolos
{

namespace cart
{

/**
 * split Matrix by axis and its value
 */
template<typename Mat, typename Label, typename T>
pair<pair<Mat, Label>, pair<Mat, Label>> bin_split_data_set(const Mat& m,
    const Label& labels, size_t axis, T value) {
    vector<vector<T>> mat_less; vector<vector<T>> label_less;
    vector<vector<T>> mat_great; vector<vector<T>> label_great;
    for(size_t i = 0; i < m.rows(); ++i) {
        vector<T> tmp_col_data(m.cols()); vector<T> tmp_col_label(1);
        for(size_t j = 0; j < m.cols(); ++j) {
            tmp_col_data[j] = m(i, j);
        }
        tmp_col_label[0] = labels(i, 0);
        if(tmp_col_data[axis] > value) {
            mat_less.emplace_back(tmp_col_data);
            label_less.emplace_back(tmp_col_label);
        }
        else {
            mat_great.emplace_back(tmp_col_data);
            label_great.emplace_back(tmp_col_label);
        }
    }
    Mat l(0, 0), g(0, 0); Label l_l(0, 0), l_g(0, 0);
    if(!mat_less.empty()) {
        l = Mat(mat_less.size(), m.cols(), mat_less);
        l_l = Label(label_less.size(), 1, label_less);
    }
    if(!mat_great.empty()) {
        g = Mat(mat_great.size(), m.cols(), mat_great);
        l_g = Label(label_great.size(), 1, label_great);
    }
    return {{l, l_l}, {g, l_g}};
}

template<typename Mat, typename Label>
/**
 * compute the regression by labels
 * @param  labels
 * @return
 */
inline Type reg_leaf(const Mat& mat, const Label& labels) {
    auto col_sums = op::col_sum(labels);
    return static_cast<Type>(col_sums) / labels.rows();
}

/**
 * the  (val - mean) error sum
 */
template<typename Mat, typename Label>
Type reg_err(const Mat& mat, const Label& labels) {
    auto mean_val = reg_leaf(mat, labels);
    Type error_sum = 0.0;
    for(size_t i = 0; i < labels.rows(); ++i) {
        auto minus = labels(i, 0) - mean_val;
        error_sum += (minus * minus);
    }
    return error_sum;
}


/**
 * Choose Function
 */
template<typename Mat, typename Label, typename LeafType, typename ErrorType>
pair<int, Type> choose_best_split(const Mat& mat, const Label& labels,
 const LeafType& leaf_type, const ErrorType& err_type, Type tol_s, size_t tol_n) {
    //judge labels's value
    bool all_equal = true;
    for(size_t i = 1; i < labels.rows(); ++i) {
        if(labels(i, 0) != labels(i - 1, 0)) {
            all_equal = false;
            break;
        }
    }
    if(all_equal) {
        return {-1, leaf_type(mat, labels)};
    }
    size_t m = mat.rows(), n = mat.cols();
    auto S = err_type(mat, labels);
    Type bestS = 1.0e20; int best_index = 0; Type best_value = 0.0;
    std::unordered_set<Type> col_values;
    for(size_t j = 0; j < n; ++j) {
        //get the unorder_set of this col values
        col_values.clear();
        for(size_t i = 0; i < m; ++i) {
            col_values.emplace(mat(i, j));
        }
        for(auto& split_val : col_values) {
            auto split_mats = bin_split_data_set(mat, labels, j, split_val);
            if(split_mats.first.first.rows() < tol_n || split_mats.second.first.rows() < tol_n) continue;
            auto newS = err_type(split_mats.first.first, split_mats.first.second)
             + err_type(split_mats.second.first, split_mats.second.second);
            if(newS < bestS) {
                best_index = j;
                best_value = split_val;
                bestS = newS;
            }
        }
    }
    if((S - bestS) < tol_s) {
        return {-1, leaf_type(mat, labels)};
    }
    auto best_split_mats = bin_split_data_set(mat, labels, best_index, best_value);
    if(best_split_mats.first.first.rows() < tol_n || best_split_mats.second.first.rows() < tol_n) {
        return {-1, leaf_type(mat, labels)};
    }
    return {best_index, best_value};
}

/**
 * creat a tree
 */
template<typename Mat, typename Label, typename LeafType, typename ErrorType>
CartTree<Type> create_tree(const Mat& mat, const Label& labels,
 const LeafType& leaf_type, const ErrorType& err_type, Type tol_s, size_t tol_n) {
    auto choosed_splits = choose_best_split(mat, labels, leaf_type, err_type, tol_s, tol_n);
    if(choosed_splits.first == -1) {
        return CartTree<Type>(choosed_splits.second);
    }
    CartTree<Type> ret_tree(static_cast<size_t>(choosed_splits.first), choosed_splits.second);
    auto best_split_mats = bin_split_data_set(mat, labels, choosed_splits.first, choosed_splits.second);
    ret_tree.append_left(create_tree(best_split_mats.first.first, best_split_mats.first.second, leaf_type, err_type, tol_s, tol_n));
    ret_tree.append_right(create_tree(best_split_mats.second.first, best_split_mats.second.second, leaf_type, err_type, tol_s, tol_n));
    return ret_tree;
}

//judge a tree node is leaf or not
template<typename TNodePtr>
inline bool is_leaf(const TNodePtr& n) {
    return n->is_leaf;
}

/**
 * Post Prune Tree for Cart Tree
 */
//compute the mean of a tree
template<typename TNodePtr>
Type get_mean_of_cart_tree(const TNodePtr& ptr) {
    Type left_val = is_leaf(ptr->left) ? ptr->left->val : get_mean_of_cart_tree(ptr->left);
    Type right_val = is_leaf(ptr->right) ? ptr->right->val : get_mean_of_cart_tree(ptr->right);
    return (left_val) / 2.0 + right_val / 2.0;
}

template<typename Label>
Type validate_regression_error(const Label& labels, Type val) {
    Type sum = 0.0;
    for(size_t i = 0; i < labels.rows(); ++i) {
        auto minus = labels(i, 0) - val;
        sum += (minus * minus);
    }
    return sum;
}

template<typename TNodePtr, typename Mat, typename Label>
TNodePtr prune(TNodePtr ptr, const Mat& mat, const Label& labels) {
    if(mat.rows() == 0) {
        ptr->is_leaf = true;
        ptr->val = get_mean_of_cart_tree(ptr);
        (ptr->left).reset(); (ptr->right).reset();
        return ptr;
    }
    if(is_leaf(ptr)) return ptr;
    auto split_mats = bin_split_data_set(mat, labels, ptr->axis, ptr->val);
    if(!is_leaf(ptr->left)) ptr->left = prune(ptr->left, split_mats.first.first, split_mats.first.second);
    if(!is_leaf(ptr->right)) ptr->right = prune(ptr->right, split_mats.second.first, split_mats.second.second);
    //merge
    if(is_leaf(ptr->left) && is_leaf(ptr->right)) {
        // split_mats = std::move(bin_split_data_set(mat, labels, ptr->axis, ptr->val)); // question
        auto error_no_merge = validate_regression_error(split_mats.first.second, ptr->left->val) +
            validate_regression_error(split_mats.second.second, ptr->right->val);
        auto tree_mean = get_mean_of_cart_tree(ptr);
        auto error_merge = validate_regression_error(labels, tree_mean);
        if(error_merge < error_no_merge) {
            ptr->is_leaf = true;
            ptr->val = tree_mean;
            (ptr->left).reset(); (ptr->right).reset();
            return ptr;
        }
    }
    return ptr;
}

/**
 * Fucntions of Tree ForeCast
 */

//cart tree eval
template<typename TNodePtr>
Type cart_tree_eval(const TNodePtr& ptr, const Type* vals, size_t len) {
    return ptr->val;
}

template<typename TNodePtr, typename ModelEval>
Type tree_forecast(const TNodePtr& ptr, const Type* vals,
 size_t len, const ModelEval& eval_fun) {
    if(is_leaf(ptr)) {
        return eval_fun(ptr, vals, len);
    }
    if(vals[ptr->axis] > ptr->val) return tree_forecast(ptr->left, vals, len, eval_fun);
    else return tree_forecast(ptr->right, vals, len, eval_fun);
}

} //cart

} //aiolos


#endif /*AIOLOS_CART_MODEL_TREE_HPP*/