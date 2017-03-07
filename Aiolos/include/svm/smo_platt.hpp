#ifndef AIOLOS_SMO_PLATT_HPP_
#define AIOLOS_SMO_PLATT_HPP_

#include "smo.hpp"
#include "def.hpp"

#include "reflection/reflection.hpp"

namespace aiolos
{

class SMOPlatt : public SMO {
public:
    SMOPlatt() = default;

    //run function
    std::pair<Type, wzp::Matrix<Type>> run(wzp::ConfigParser* config_parser, wzp::Matrix<Type>& input_matrix, wzp::Matrix<int>& input_label,
        Type C, Type toler, int max_iter, bool is_silent = false) override;

private:
    //others
    wzp::Matrix<Type>* m_input_matrix;
    wzp::Matrix<int>* m_input_label;
    wzp::ConfigParser* m_config_parser;

    /**
     * SMO Platt Members
     */
    bool m_is_silent = false;
    std::string m_kernal_type {"rbf"};
    Type m_sigma = -1.0;
    bool m_use_kernal = false;

    //Reflection
    static ReflectionRegister(SMO, SMOPlatt) regis_smo_platt;

private:
    /**
     * the inner iteration function, to get the best j
     * @param  i index
     * @return   0 or 1
     */
    int inner_iter(size_t i, OptStruct& oS);

    Type compute_eta(const OptStruct& oS, size_t i, size_t j);

    Type compute_b1(const OptStruct& oS, size_t i, size_t j, Type Ei, Type alpha_i_old, Type alpha_j_old);

    Type compute_b2(const OptStruct& oS, size_t i, size_t j, Type Ej, Type alpha_i_old, Type alpha_j_old);
};

} //aiolos


#endif /*AIOLOS_SMO_PLATT_HPP_*/