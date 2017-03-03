#ifndef AIOLOS_KMEANS_BASE_SOLVER_HPP
#define AIOLOS_KMEANS_BASE_SOLVER_HPP

#include <vector>

#include "kmeans_solver.hpp"

#include "reflection/reflection.hpp"


namespace aiolos
{

class KmeansBaseSolver : public KmeansSolver
{
public:
    KmeansBaseSolver() = default;

    std::pair<wzp::Matrix<Type>, wzp::Matrix<Type>> solve(wzp::Matrix<Type>& data_set,
     size_t k_, const std::function<Type(const Type*, const Type*, size_t)>& dist_meas_fun) override;

private:

    //reflection
    static ReflectionRegister(KmeansSolver, KmeansBaseSolver) regis_kmeans_base_solver;

private:
    /**
     * rand the centroids
     */
    void rand_cent();

    /**
     * the process function
     */
    void process(wzp::Matrix<Type>& cluster_assement);

    void compute_col_mean(const vector<size_t>& cent_indexs, size_t cent);

};

} //aiolos

#endif /*AIOLOS_KMEANS_BASE_SOLVER_HPP*/