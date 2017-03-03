#ifndef AIOLOS_MODEL_TREE_HPP
#define AIOLOS_MODEL_TREE_HPP

#ifndef AIOLOS_CART_TREE_HPP_
#define AIOLOS_CART_TREE_HPP_

#include "regression.hpp"

#include "reflection/reflection.hpp"

#include "cart_model_tree_impl.hpp"

namespace aiolos
{

class ModelTreeRegression : public Regression
{
public:
    /**
     * Constructor
     */
    ModelTreeRegression() = default;

    /**
     * init function
     */
    void init(wzp::ConfigParser* config_parser) override;

    /**
     * train function
     */
    void train(wzp::Matrix<Type>& input_matrix, wzp::Matrix<Type>& input_label) override;

    void train(wzp::Matrix<Type>& input_matrix,
     wzp::Matrix<Type>& input_label, wzp::Matrix<Type>& validate_matrix,
     wzp::Matrix<Type>& validate_label) override;

    /**
     * prediction
     */
    wzp::Matrix<Type> predict(wzp::Matrix<Type>& predict_matrix) override;

    /**
     * save and resotre model
     */
    void dump_model(const char* filename) override;

    void restore_model(const char* filename) override;

private:
    //two data put in train
    wzp::ConfigParser* m_config_parser;

    wzp::Matrix<Type>* m_input_matrix;
    wzp::Matrix<Type>* m_input_label;
    wzp::Matrix<Type>* m_validate_matrix;
    wzp::Matrix<Type>* m_validate_label;

    size_t m;
    size_t n;

    int m_is_silent = 0;

    /**
     * Cart Tree Regression Members
     */
    //op -> tol_s and tol_n
    Type tol_s = 1.0;
    size_t tol_n = 4;

    CartTree<Type> m_root_tree;

    /**
     * Reflection
     */
    static ReflectionRegister(Regression, ModelTreeRegression) regis_model_tree;

private:
    void train_process();

};


} //aiolos


#endif /*AIOLOS_CART_TREE_HPP_*/


#endif /*AIOLOS_MODEL_TREE_HPP*/