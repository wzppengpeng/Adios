#include "svm/smo_simple.hpp"

#include <algorithm>

#include "log/log.hpp"
#include "util/op.hpp"
#include "util/aiolos_math.hpp"


using namespace std;
using namespace wzp;

namespace aiolos
{

ReflectionRegister(SMO, SMOSimple) regis_smo_simple("smo_simple");

//the main interface
std::pair<Type, wzp::Matrix<Type>> SMOSimple::run(wzp::Matrix<Type>& input_matrix, wzp::Matrix<int>& input_label,
        Type C, Type toler, int max_iter, bool is_silent) {
    //prepare for code
    m_input_matrix = &input_matrix;
    m_input_label = &input_label;
    m = input_matrix.rows();
    n = input_matrix.cols();
    m_is_silent = is_silent;
    //the return ans pair
    Type b = 0.0;
    wzp::Matrix<Type> alphas(m, 1);
    //the iter number record
    int iter = 0;
    while(iter < max_iter) {
        process(b, alphas, iter, C, toler);
    }
    return {b, alphas};
}

void SMOSimple::process(Type& b, wzp::Matrix<Type>& alphas, int& iter, Type C, Type toler) {
    int alpha_pairs_changed = 0;
    for(size_t i = 0; i < m; ++i) {
        //compute the fxi
        auto fXi = Type(((op::multiply(alphas, *m_input_label).t()) *
         (*m_input_matrix * m_input_matrix->slice(i, 0, i + 1, n).t())).at(0, 0)) + b;
        auto Ei = fXi - Type(m_input_label->at(i, 0));
        //in toler then do below
        if(((m_input_label->at(i, 0) * Ei < -toler) && (alphas(i, 0) < C)) ||
            ((m_input_label->at(i, 0) * Ei > toler) && (alphas(i, 0) > 0))) {
            auto j = math::aiolos_select_rand<size_t, size_t>(i, 0, m-1);
            //compyte another error
            auto fXj = Type(((op::multiply(alphas, *m_input_label).t()) *
                (*m_input_matrix * m_input_matrix->slice(j, 0, j + 1, n).t())).at(0, 0)) + b;
            auto Ej = fXj - Type(m_input_label->at(j, 0));
            auto alpha_i_old = alphas(i, 0);
            auto alpha_j_old = alphas(j, 0);
            //ensure the alpha is in 0 and C
            Type L, H;
            if(m_input_label->at(i, 0) != m_input_label->at(j, 0)) {
                L = std::max(0.0f, alphas(j, 0) - alphas(i, 0));
                H = std::min(C, C + alphas(j, 0) - alphas(i, 0));
            }
            else {
                L = std::max(0.0f, alphas(j, 0) + alphas(i, 0) - C);
                H = std::min(C, alphas(j, 0) + alphas(i, 0));
            }
            if(L == H) {
                if(!m_is_silent) log::info("L == H;");
                continue;
            }
            //set the modify eta
            auto eta = compute_eta(i, j);
            if(eta >= 0) {
                if(!m_is_silent) log::info("eta >= 0;");
                continue;
            }
            //modify alphas[i] and alphas[j]
            alphas(j, 0) -= m_input_label->at(j, 0) * (Ei - Ej) / eta;
            alphas(j, 0) = math::aiolos_clip(alphas(j, 0), L, H);
            if(std::abs(alphas(j, 0) - alpha_j_old) < 0.00001) {
                if(!m_is_silent) log::info("j is not moving enough;");
                continue;
            }
            alphas(i, 0) += m_input_label->at(j, 0) * m_input_label->at(i, 0) *
             (alpha_j_old - alphas(j, 0));
            //compute b
            Type b1 = b - Ei - m_input_label->at(i, 0) * (alphas(i, 0) - alpha_i_old) *
                (m_input_matrix->slice(i, 0, i+1, n) * (m_input_matrix->slice(i, 0, i+1, n)).t()).at(0, 0) -
                m_input_label->at(j, 0) * (alphas(j, 0) - alpha_j_old) *
                (m_input_matrix->slice(i, 0, i+1, n) * (m_input_matrix->slice(j, 0, j+1, n)).t()).at(0, 0);
            Type b2 = b - Ej - m_input_label->at(i, 0) * (alphas(i, 0) - alpha_i_old) *
                (m_input_matrix->slice(i, 0, i+1, n) * (m_input_matrix->slice(j, 0, j+1, n)).t()).at(0, 0) -
                m_input_label->at(j, 0) * (alphas(j, 0) - alpha_j_old) *
                (m_input_matrix->slice(j, 0, j+1, n) * (m_input_matrix->slice(j, 0, j+1, n)).t()).at(0, 0);
            if((alphas(i, 0) > 0) && (alphas(i, 0) < C)) {
                b = b1;
            }
            else if((alphas(j, 0) > 0) && (alphas(j, 0) < C)) {
                b = b2;
            }
            else {
                b = (b1) / 2.0 + (b2) / 2.0;
            }
            ++alpha_pairs_changed;
            //print changed pairs
            if(!m_is_silent) log::info("iter:", iter, "i:", i, "pairs changed:", alpha_pairs_changed);
        }
    }
    if(alpha_pairs_changed == 0) ++iter;
    else iter = 0;
    if(!m_is_silent) log::info("iteration number:", iter);
}

Type SMOSimple::compute_eta(size_t i, size_t j) {
    return (2.0 * (m_input_matrix->slice(i, 0, i+1, n)
     * (m_input_matrix->slice(j, 0, j+1, n).t())).at(0, 0)) -
        (m_input_matrix->slice(i, 0, i+1, n) * (m_input_matrix->slice(i, 0, i+1, n).t())).at(0, 0) -
        (m_input_matrix->slice(j, 0, j+1, n) * (m_input_matrix->slice(j, 0, j+1, n).t())).at(0, 0);
}

}