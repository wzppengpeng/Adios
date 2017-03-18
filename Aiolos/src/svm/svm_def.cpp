#include "svm/def.hpp"

#include <algorithm>

#include "util/op.hpp"
#include "util/aiolos_math.hpp"
#include "util/matrix_args.hpp"

#include "log/log.hpp"


namespace aiolos
{

Type calcEk(const OptStruct& oS, size_t k, bool use_kernal) {
    if(use_kernal) {
        auto fXk = Type(((op::multiply(oS.alphas, *oS.label_mat).t()) *
                (oS.K.slice(0, k, oS.m, k+1))).at(0, 0)) + oS.b;
        return fXk - Type(oS.label_mat->at(k, 0));
    }
    auto fXk = Type(((op::multiply(oS.alphas, *oS.label_mat).t()) *
                (*oS.X * ((oS.X)->slice(k, 0, k + 1, oS.n).t()))).at(0, 0)) + oS.b;
    return fXk - Type(oS.label_mat->at(k, 0));
}

std::pair<size_t, Type> selectJ(size_t i, OptStruct& oS, Type Ei, bool use_kernal) {
    size_t maxK = 0;
    Type maxDeltaE = 0.0, Ej = 0.0;
    oS.e_cache(i, 0) = 1;
    oS.e_cache(i, 1) = Ei;
    //walk the ecache
    auto valid_ecache_list = op::nonzero(oS.e_cache, 0);
    if(valid_ecache_list.size() > 1) {
        for(auto k : valid_ecache_list) {
            if(k == i) continue;
            auto Ek = calcEk(oS, k, use_kernal);
            auto deltaE = std::abs(Ei - Ek);
            if(deltaE > maxDeltaE) {
                maxK = k;
                maxDeltaE = deltaE;
                Ej = Ek;
            }
        }
    }
    else {
        maxK = math::aiolos_select_rand<size_t, size_t>(i, 0, oS.m - 1);
        Ej = calcEk(oS, maxK, use_kernal);
    }
    return {maxK, Ej};
}

void updateEk(OptStruct& oS, size_t k, bool use_kernal) {
    auto Ek = calcEk(oS, k, use_kernal);
    oS.e_cache(k, 0) = 1;
    oS.e_cache(k, 1) = Ek;
}

wzp::Matrix<Type> calWs(wzp::Matrix<Type>& alphas,
 wzp::Matrix<Type>& dataMatIn, wzp::Matrix<int>& classLabels) {
    wzp::Matrix<Type> W(dataMatIn.cols(), 1);
    for(size_t i = 0; i < dataMatIn.rows(); ++i) {
        if(alphas(i, 0) != 0) {
            Type val = (alphas(i, 0) * classLabels(i, 0));
            auto other = dataMatIn.slice(i, 0, i+1, dataMatIn.cols()).t() * val;
            W += other;
        }
    }
    return W;
}

wzp::Matrix<Type> kernal_trans(wzp::Matrix<Type>& X,
    const wzp::Matrix<Type>& A,
    const std::string& kernal_type,
    Type sigma) {
    wzp::Matrix<Type> K(X.rows(), 1);
    if(kernal_type == "lin") {
        wzp::Matrix<Type> tmp(A);
        K = std::move(X * (tmp.t()));
    }
    else if(kernal_type == "rbf") {
        for(size_t i = 0; i < X.rows(); ++i) {
            K(i, 0) = MatrixArgs::compute_squre_distance(X.row_at(i), A.row_at(0), X.cols());
        }
        K = std::move(math::aiolos_matrix_exp(K * (1.0 / (-1 * sigma * sigma))));
    }
    else {
        wzp::log::fatal("The Kernal Type Is Not Ok", kernal_type);
    }
    return std::move(K);
}

} //aiolos