#include "svm/def.hpp"

#include <algorithm>

#include "util/op.hpp"
#include "util/aiolos_math.hpp"


namespace aiolos
{

Type calcEk(const OptStruct& oS, size_t k) {
    auto fXk = Type(((op::multiply(oS.alphas, *oS.label_mat).t()) *
                (*oS.X * ((oS.X)->slice(k, 0, k + 1, oS.n).t()))).at(0, 0)) + oS.b;
    return fXk - Type(oS.label_mat->at(k, 0));
}

std::pair<size_t, Type> selectJ(size_t i, OptStruct& oS, Type Ei) {
    size_t maxK = 0;
    Type maxDeltaE = 0.0, Ej = 0.0;
    oS.e_cache(i, 0) = 1;
    oS.e_cache(i, 1) = Ei;
    //walk the ecache
    auto valid_ecache_list = op::nonzero(oS.e_cache, 0);
    if(valid_ecache_list.size() > 1) {
        for(auto k : valid_ecache_list) {
            if(k == i) continue;
            auto Ek = calcEk(oS, k);
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
        Ej = calcEk(oS, maxK);
    }
    return {maxK, Ej};
}

void updateEk(OptStruct& oS, size_t k) {
    auto Ek = calcEk(oS, k);
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

} //aiolos