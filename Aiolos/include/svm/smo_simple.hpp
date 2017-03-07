#ifndef AIOLOS_SMO_SIMPLE_HPP_
#define AIOLOS_SMO_SIMPLE_HPP_

#include "svm/smo.hpp"

#include "reflection/reflection.hpp"

namespace aiolos
{

class SMOSimple : public SMO {
public:
    SMOSimple() = default;

    //run function
    std::pair<Type, wzp::Matrix<Type>> run(wzp::ConfigParser* config_parser, wzp::Matrix<Type>& input_matrix, wzp::Matrix<int>& input_label,
        Type C, Type toler, int max_iter, bool is_silent = false) override;

private:
    //others
    wzp::Matrix<Type>* m_input_matrix;
    wzp::Matrix<int>* m_input_label;

    /**
     * SMOSIMPLE MEMBERS
     */
    bool m_is_silent = false;

    size_t m;
    size_t n; // the rows and cols

    //Reflection
    static ReflectionRegister(SMO, SMOSimple) regis_smo_simple;

private:
    /**
     * the process in max_iter
     * @param b      the return b
     * @param alphas the return alphas
     * @param C      the max one
     * @param toler  the error rate
     */
    void process(Type& b, wzp::Matrix<Type>& alphas, int& iter, Type C, Type toler);

    /**
     * [compute_eta description]
     * @param  i the i row index
     * @param  j the j select random row index
     * @return   the eta value
     */
    Type compute_eta(size_t i, size_t j);

};

} //aiolos

#endif /*AIOLOS_SMO_SIMPLE_HPP_*/