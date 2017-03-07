#ifndef AIOLOS_SMO_DEF_HPP_
#define AIOLOS_SMO_DEF_HPP_

#include "container/matrix.hpp"

#include "common.hpp"

namespace aiolos
{

/**
 * calculate the kernal transfer, use gaso
 */
wzp::Matrix<Type> kernal_trans(wzp::Matrix<Type>& X,
    const wzp::Matrix<Type>& A,
    const std::string& kernal_type,
    Type sigma); // m* 1

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

    //for kernal
    wzp::Matrix<Type> K;

    //constructor
    OptStruct(wzp::Matrix<Type>& dataMatIn, wzp::Matrix<int>& classLabels, Type C_, Type toler,
        const std::string& kernal_type = "rbf",
        Type sigma = -1.0) :
        X(&dataMatIn),
        label_mat(&classLabels),
        C(C_),
        tol(toler),
        m(dataMatIn.rows()),
        n(dataMatIn.cols()),
        alphas(dataMatIn.rows(), 1),
        e_cache(dataMatIn.rows(), 2)
    {
        if(sigma > 0) {
            K = std::move(wzp::Matrix<Type>(m, m));
            for(size_t i = 0; i < m; ++i) {
                auto trans_vec = kernal_trans(*X, X->slice(i, 0, i + 1, n), kernal_type, sigma);
                for(size_t j = 0; j < m; ++j) {
                    K(j, i) = trans_vec(j, 0);
                }
            }
        }
    }
};

/**
 * calcute the error of kth element
 * @param  oS opt struct
 * @param  k  k index
 * @return    the error
 */
Type calcEk(const OptStruct& oS, size_t k, bool use_kernal = false);

/**
 * choose the max length of k
 */
std::pair<size_t, Type> selectJ(size_t i, OptStruct& oS, Type Ei, bool use_kernal = false);

/**
 * uodate the error cache
 * @param oS Optstruct
 * @param k  k index
 */
void updateEk(OptStruct& oS, size_t k, bool use_kernal = false);

/**
 * calculate the W matrix(n * 1)
 */
wzp::Matrix<Type> calWs(wzp::Matrix<Type>& alphas,
 wzp::Matrix<Type>& dataMatIn, wzp::Matrix<int>& classLabels);



} //aiolos


#endif /*AIOLOS_SMO_DEF_HPP_*/