#include "kmeans/kmeans_base_solver.hpp"

#include "util/aiolos_math.hpp"
#include "util/op.hpp"

using namespace std;
using namespace wzp;

namespace aiolos
{

ReflectionRegister(KmeansSolver, KmeansBaseSolver) regis_kmeans_base_solver("base");

std::pair<wzp::Matrix<Type>, wzp::Matrix<Type>> KmeansBaseSolver::solve(wzp::Matrix<Type>& data_set,
     size_t k_, const std::function<Type(const Type*, const Type*, size_t)>& dist_meas_fun) {
    m_data_set = &data_set;
    k = k_;
    m_dist_meas_fun = dist_meas_fun;
    m = data_set.rows(); n = data_set.cols();
    m_centroids.reshape(k, data_set.cols());
    Matrix<Type> cluster_assement(m ,2);
    process(cluster_assement);
    return {m_centroids, cluster_assement};
}


void KmeansBaseSolver::rand_cent() {
    for(size_t j = 0; j < n; ++j) {
        auto arg_min_max_indexs = op::arg_min_max(*m_data_set, j);
        auto min_val = m_data_set->at(arg_min_max_indexs.first, j);
        auto range = m_data_set->at(arg_min_max_indexs.second, j) - min_val;
        for(size_t i = 0; i < k; ++i) {
            m_centroids(i, j) = min_val + range * (math::aiolos_random_real(0, 1));
        }
    }
}

void KmeansBaseSolver::process(wzp::Matrix<Type>& cluster_assement) {
    rand_cent();
    bool is_cluster_changed = true;
    //use multi thread
    // auto row_indexs = op::generate<vector<size_t>>(m);
    while(is_cluster_changed) {
        is_cluster_changed = false;
        wzp::ParallelRange(m,
         [&cluster_assement, this, &is_cluster_changed](size_t i) {
            Type min_dist = 1.0e20; size_t min_index;
            for(size_t j = 0; j < k; ++j) {
                auto dist_ji = m_dist_meas_fun(m_centroids.row_at(j),
                    m_data_set->row_at(i), n);
                if(dist_ji < min_dist) {
                    min_dist = dist_ji;
                    min_index = j;
                }
            }
            if(static_cast<size_t>(cluster_assement(i, 0)) != min_index) is_cluster_changed = true;
            cluster_assement(i, 0) = min_index;
            cluster_assement(i, 1) = min_dist * min_dist;
        });
        //change the centroids location
        for(size_t cent = 0; cent < k; ++cent) {
            //generate the right indexs
            auto cent_indexs = op::split_by_col_value(cluster_assement, cent);
            compute_col_mean(cent_indexs, cent);
        }
    }
}

void KmeansBaseSolver::compute_col_mean(const vector<size_t>& cent_indexs, size_t cent) {
    vector<Type> tmp(n, 0.0);
    for(size_t i = 0; i < cent_indexs.size(); ++i) {
        for(size_t j = 0; j < n; ++j) {
            tmp[j] += m_data_set->at(cent_indexs[i], j);
        }
    }
    for(size_t j = 0; j < n; ++j) {
        m_centroids(cent, j) = tmp[j] / static_cast<Type>(cent_indexs.size());
    }
}

} //aiolos