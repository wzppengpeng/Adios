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
    std::pair<Type, wzp::Matrix<Type>> run(wzp::Matrix<Type>& input_matrix, wzp::Matrix<int>& input_label,
        Type C, Type toler, int max_iter, bool is_silent = false) override;

private:
    //others
    wzp::Matrix<Type>* m_input_matrix;
    wzp::Matrix<int>* m_input_label;

    /**
     * SMO Platt Members
     */
    bool m_is_silent = false;

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
};

} //aiolos


#endif /*AIOLOS_SMO_PLATT_HPP_*/