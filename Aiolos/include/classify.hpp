#ifndef AIOLOS_CLASSIFY_HPP
#define AIOLOS_CLASSIFY_HPP

/**
 * the base type of Classify
 * define the base interface to all classify problems
 */
#include "config/config_parser.hpp"
#include "container/matrix.hpp"

#include "common.hpp"


namespace aiolos
{



class Classify {
public:
    virtual ~Classify() {}

    /**
     * the init function
     */
    virtual void init(wzp::ConfigParser* config_parser) = 0;

    /**
     * train function
     */
    //do not have validate matrix
    virtual void train(wzp::Matrix<Type>& input_matrix, wzp::Matrix<int>& input_label) = 0;

    //have validate matrix
    virtual void train(wzp::Matrix<Type>& input_matrix,
     wzp::Matrix<int>& input_label, wzp::Matrix<Type>& validate_matrix,
     wzp::Matrix<int>& validate_label) = 0;

    /**
     * predict function
     */
    virtual wzp::Matrix<int> predict(wzp::Matrix<Type>& predict_matrix) = 0;

    /**
     * save and dump the model
     */
    virtual void dump_model(const char* filename) = 0;


    /**
     * restore the model
     */
    virtual void restore_model(const char* filename) = 0;
};

} //aiolos

#endif /*AIOLOS_CLASSIFY_HPP*/