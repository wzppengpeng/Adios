#include "svm/smo_platt.hpp"

#include <algorithm>

#include "log/log.hpp"

#include "util/aiolos_math.hpp"

using namespace std;
using namespace wzp;

namespace aiolos
{

ReflectionRegister(SMO, SMOPlatt) regis_smo_platt("smo_platt");

std::pair<Type, wzp::Matrix<Type>> SMOPlatt::run(wzp::ConfigParser* config_parser, wzp::Matrix<Type>& input_matrix, wzp::Matrix<int>& input_label,
        Type C, Type toler, int max_iter, bool is_silent) {
    m_input_matrix = &input_matrix;
    m_input_label = &input_label;
    m_config_parser = config_parser;
    m_config_parser->get("kernal_type", m_kernal_type);
    m_config_parser->get("sigma", m_sigma);
    if(m_sigma > 0) {
        m_use_kernal = true;
        wzp::log::info("Use Kernal, Type", m_kernal_type);
    }
    m_is_silent = is_silent;
    OptStruct oS(input_matrix, input_label, C, toler, m_kernal_type, m_sigma);
    int iter = 0;
    bool entire_set = true;
    int alpha_pairs_changed = 0;
    while((iter < max_iter) && ((alpha_pairs_changed > 0) || (entire_set))) {
        alpha_pairs_changed = 0;
        if(entire_set) {
            for(size_t i = 0; i < oS.m; ++i) {
                alpha_pairs_changed += inner_iter(i, oS);
                if(!m_is_silent) { log::info("fullset, iter",
                 iter, "i :", i, "pais changed ", alpha_pairs_changed); }
            }
            ++iter;
        }
        else {
            for(size_t i = 0; i < oS.m; ++i) {
                if((oS.alphas(i, 0) > 0) && (oS.alphas(i, 0) < oS.C)) {
                    alpha_pairs_changed += inner_iter(i, oS);
                    if(!m_is_silent) {
                        log::info("non-bound, iter:", iter, "i :", i,
                         "pairs changed ", alpha_pairs_changed);
                    }
                }
            }
            ++iter;
        }
        if(entire_set) entire_set = false;
        else if(alpha_pairs_changed == 0) entire_set = true;
        if(!m_is_silent) log::info("iteration number :", iter);
    }
    return {oS.b, oS.alphas};
}

int SMOPlatt::inner_iter(size_t i, OptStruct& oS) {
    auto Ei = calcEk(oS, i, m_use_kernal);
    if((oS.label_mat->at(i, 0) * Ei < -oS.tol && oS.alphas(i, 0) < oS.C) ||
        (oS.label_mat->at(i, 0) * Ei > oS.tol && oS.alphas(i, 0) > 0)) {
        auto j_pair = selectJ(i, oS, Ei, m_use_kernal);
        auto j = j_pair.first;
        auto Ej = j_pair.second;
        auto alpha_i_old = oS.alphas(i, 0);
        auto alpha_j_old = oS.alphas(j, 0);
        Type L, H;
        if(oS.label_mat->at(i, 0) != oS.label_mat->at(j, 0)) {
            L = std::max(0.0, oS.alphas(j, 0) - oS.alphas(i, 0));
            H = std::min(oS.C, oS.C + oS.alphas(j, 0) - oS.alphas(i, 0));
        }
        else {
            L = std::max(0.0, oS.alphas(j, 0) + oS.alphas(i, 0) - oS.C);
            H = std::min(oS.C, oS.alphas(j, 0) + oS.alphas(i, 0));
        }
        if(L == H) {
            if(!m_is_silent) log::info("L == H");
            return 0;
        }
        //compute the eta
        auto eta = compute_eta(oS, i, j);
        if(eta >= 0) {
            if(!m_is_silent) log::info("eta >= 0");
            return 0;
        }
        //update the error cache
        oS.alphas(j, 0) -= oS.label_mat->at(j, 0) * (Ei - Ej) / eta;
        math::aiolos_clip(oS.alphas(j, 0), L, H);
        updateEk(oS, j, m_use_kernal);
        if(std::abs(oS.alphas(j, 0) - alpha_j_old) < 0.00001) {
            if(!m_is_silent) log::info("j is not moving enough");
            return 0;
        }
        oS.alphas(i, 0) += oS.label_mat->at(j, 0) * oS.label_mat->at(i, 0) *
            (alpha_j_old - oS.alphas(j, 0));
        updateEk(oS, i, m_use_kernal);
        //compute b
        Type b1 = compute_b1(oS, i, j, Ei, alpha_i_old, alpha_j_old);
        Type b2 = compute_b2(oS, i, j, Ej, alpha_i_old, alpha_j_old);
        if(oS.alphas(i, 0) > 0 && oS.alphas(i, 0) < oS.C) {
            oS.b = b1;
        }
        else if(oS.alphas(j, 0) > 0 && oS.alphas(j, 0) < oS.C) {
            oS.b = b2;
        }
        else {
            oS.b = b1 / 2.0 + b2 /  2.0;
        }
        return 1;
    }
    else
        return 0;
}

Type SMOPlatt::compute_eta(const OptStruct& oS, size_t i, size_t j) {
    if(m_use_kernal) {
        return (2.0 * oS.K(i, j) - oS.K(i, i) - oS.K(j, j));
    }
    return (2.0 * (oS.X->slice(i, 0, i+1, oS.n)
     * (oS.X->slice(j, 0, j+1, oS.n).t())).at(0, 0)) -
        (oS.X->slice(i, 0, i+1, oS.n) * (oS.X->slice(i, 0, i+1, oS.n).t())).at(0, 0) -
        (oS.X->slice(j, 0, j+1, oS.n) * (oS.X->slice(j, 0, j+1, oS.n).t())).at(0, 0);
}

Type SMOPlatt::compute_b1(const OptStruct& oS, size_t i, size_t j, Type Ei, Type alpha_i_old, Type alpha_j_old) {
    if(m_use_kernal) {
        return oS.b - Ei - oS.label_mat->at(i, 0) * (oS.alphas(i, 0) - alpha_i_old) * oS.K(i, i) -
            oS.label_mat->at(j, 0) * (oS.alphas(j, 0) - alpha_j_old) * oS.K(i, j);
    }
    return oS.b - Ei - oS.label_mat->at(i, 0) * (oS.alphas(i, 0) - alpha_i_old) *
            (oS.X->slice(i, 0, i+1, oS.n) * (oS.X->slice(i, 0, i+1, oS.n)).t()).at(0, 0) -
            oS.label_mat->at(j, 0) * (oS.alphas(j, 0) - alpha_j_old) *
            (oS.X->slice(i, 0, i+1, oS.n) * (oS.X->slice(j, 0, j+1, oS.n)).t()).at(0, 0);
}

Type SMOPlatt::compute_b2(const OptStruct& oS, size_t i, size_t j, Type Ej, Type alpha_i_old, Type alpha_j_old) {
    if(m_use_kernal) {
        return oS.b - Ej - oS.label_mat->at(i, 0) * (oS.alphas(i, 0) - alpha_i_old) * oS.K(i, j) -
            oS.label_mat->at(j, 0) * (oS.alphas(j, 0) - alpha_j_old) * oS.K(j, j);
    }
    return oS.b - Ej - oS.label_mat->at(i, 0) * (oS.alphas(i, 0) - alpha_i_old) *
            (oS.X->slice(i, 0, i+1, oS.n) * (oS.X->slice(j, 0, j+1, oS.n)).t()).at(0, 0) -
            oS.label_mat->at(j, 0) * (oS.alphas(j, 0) - alpha_j_old) *
            (oS.X->slice(j, 0, j+1, oS.n) * (oS.X->slice(j, 0, j+1, oS.n)).t()).at(0, 0);
}

} //aiolos