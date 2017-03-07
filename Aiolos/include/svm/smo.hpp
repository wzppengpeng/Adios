#ifndef AIOLOS_SMO_HPP
#define AIOLOS_SMO_HPP

#include "container/matrix.hpp"
#include "config/config_parser.hpp"

#include "common.hpp"


namespace aiolos
{

class SMO {
public:
    //Destructor
    virtual ~SMO() {}

    /**
     * smo resolve function
     */
    virtual std::pair<Type, wzp::Matrix<Type>> run(wzp::ConfigParser* config_parser, wzp::Matrix<Type>& input_matrix, wzp::Matrix<int>& input_label,
        Type C, Type toler, int max_iter, bool is_silent = false) = 0;

};

} //aiolos

#endif /*AIOLOS_SMO_HPP*/