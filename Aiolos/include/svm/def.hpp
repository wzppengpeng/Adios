#ifndef AIOLOS_SMO_DEF_HPP_
#define AIOLOS_SMO_DEF_HPP_

#include "container/matrix.hpp"

#include "common.hpp"

namespace aiolos
{

/**
 * For Platt SMO Support
 */
struct OptStruct
{
    wzp::Matrix<Type>* X; //dataMatIn
    wzp::Matrix<int>* label_mat;
    Type C;
    Type tol;
    size_t m;
    size_t n;
    wzp::Matrix<Type> alphas;
    Type b = 0;
    wzp::Matrix<Type> e_cache;

    //constructor
    OptStruct(wzp::Matrix<Type>& dataMatIn, wzp::Matrix<int>& classLabels, Type C_, Type toler) :
        X(&dataMatIn),
        label_mat(&classLabels),
        C(C_),
        tol(toler),
        m(dataMatIn.rows()),
        n(dataMatIn.cols()),
        alphas(dataMatIn.rows(), 1),
        e_cache(dataMatIn.rows(), 2)
    {}
};

/**
 * calcute the error of kth element
 * @param  oS opt struct
 * @param  k  k index
 * @return    the error
 */
Type calcEk(const OptStruct& oS, size_t k);

/**
 * choose the max length of k
 */
std::pair<size_t, Type> selectJ(size_t i, OptStruct& oS, Type Ei);

/**
 * uodate the error cache
 * @param oS Optstruct
 * @param k  k index
 */
void updateEk(OptStruct& oS, size_t k);

/**
 * calculate the W matrix(n * 1)
 */
wzp::Matrix<Type> calWs(wzp::Matrix<Type>& alphas,
 wzp::Matrix<Type>& dataMatIn, wzp::Matrix<int>& classLabels);

} //aiolos


#endif /*AIOLOS_SMO_DEF_HPP_*/