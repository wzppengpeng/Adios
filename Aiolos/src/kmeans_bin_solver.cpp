#include "kmeans/kmeans_bin_solver.hpp"

#include "util/op.hpp"

using namespace std;
using namespace wzp;

namespace aiolos
{

ReflectionRegister(KmeansSolver, KmeansBinSolver) regis_kmeans_bin_solver("bin");

std::pair<wzp::Matrix<Type>, wzp::Matrix<Type>> KmeansBinSolver::solve(wzp::Matrix<Type>& data_set,
     size_t k_, const std::function<Type(const Type*, const Type*, size_t)>& dist_meas_fun) {
    m_data_set = &data_set;
    k = k_;
    m_dist_meas_fun = dist_meas_fun;
    m = data_set.rows(); n = data_set.cols();
    // m_centroids.reshape(m, n); use vector<vector>
    m_cluster_assment.reshape(m, 2);
    m_base_solver = wzp::Reflection<KmeansSolver>::create_unique("base");
    process();
    return {m_centroids, m_cluster_assment};
}


void KmeansBinSolver::process() {
    //init
    vector<Type> centroid0(n);
    for(size_t j = 0; j < n; ++j) {
        centroid0[j] = op::col_mean(*m_data_set, j);
    }
    m_cent_list.emplace_back(centroid0);
    //init the cluster assement
    for(size_t i = 0; i < m; ++i) {
        m_cluster_assment(i, 0) = 0;
        auto distance = m_dist_meas_fun(&m_cent_list[0][0], m_data_set->row_at(i), n);
        m_cluster_assment(i, 1) = distance * distance;
    }
    while(m_cent_list.size() < k) {
        Type lowest_sse = 1.0e20;
        size_t best_cent_to_split;
        wzp::Matrix<Type> best_new_cent;
        wzp::Matrix<Type> best_cluster_ass;
        for(size_t i = 0; i < m_cent_list.size(); ++i) {
            auto tmp_indexs = op::split_by_col_value(m_cluster_assment, i);
            auto pts_in_curr_cluster = op::get_new_mat_by_rows(*m_data_set, tmp_indexs);
            auto centroid_curr_res = m_base_solver->solve(pts_in_curr_cluster, 2,
             [this](const Type* a, const Type* b, size_t len) {
                return m_dist_meas_fun(a, b, len);
            });
            //compute the sse
            auto split_sse = op::col_sum(centroid_curr_res.second, 1);
            auto solit_not_split = compute_not_split_sse(i);
            //update the best's
            if(split_sse + solit_not_split < lowest_sse) {
                best_cent_to_split = i;
                best_new_cent = std::move(centroid_curr_res.first);
                best_cluster_ass = std::move(centroid_curr_res.second);
                lowest_sse = split_sse + solit_not_split;
            }
        }
        //update
        update_best_cluster_ass(best_cluster_ass, m_cent_list.size(), best_cent_to_split);
        m_cent_list[best_cent_to_split] = std::move(vector<Type>(best_new_cent.row_at(0),
         best_new_cent.row_at(0) + n));
        m_cent_list.emplace_back(best_new_cent.row_at(1), best_new_cent.row_at(1) + n);
        auto tmp_indexs = op::split_by_col_value(m_cluster_assment, best_cent_to_split);
        for(size_t i = 0; i < tmp_indexs.size(); ++i) {
            m_cluster_assment(tmp_indexs[i], 0) = best_cluster_ass(i, 0);
            m_cluster_assment(tmp_indexs[i], 1) = best_cluster_ass(i, 1);
        }
    }
    m_centroids = std::move(Matrix<Type>(k, n, m_cent_list));
}

Type KmeansBinSolver::compute_not_split_sse(size_t t) {
    Type sum = 0.0;
    for(size_t i = 0; i < m; ++i) {
        if(static_cast<size_t>(m_cluster_assment(i, 0)) != t) {
            sum += m_cluster_assment(i, 1);
        }
    }
    return sum;
}

void KmeansBinSolver::update_best_cluster_ass(wzp::Matrix<Type>& best_cluster_ass,
 size_t len, size_t best_cent_to_split) {
    for(size_t i = 0; i < best_cluster_ass.rows(); ++i) {
        if(static_cast<size_t>(best_cluster_ass(i, 0)) == 1) {
            best_cluster_ass(i, 0) = len;
        }
        else {
            best_cluster_ass(i, 0) = best_cent_to_split;
        }
    }
}

} //aiolos