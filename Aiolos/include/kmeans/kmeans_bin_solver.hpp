#ifndef AIOLOS_KMENAS_BIN_SOLVER_HPP
#define AIOLOS_KMENAS_BIN_SOLVER_HPP

#include "kmeans_solver.hpp"

#include "reflection/reflection.hpp"

namespace aiolos
{

class KmeansBinSolver : public KmeansSolver
{
public:
    KmeansBinSolver() = default;

    std::pair<wzp::Matrix<Type>, wzp::Matrix<Type>> solve(wzp::Matrix<Type>& data_set,
     size_t k_, const std::function<Type(const Type*, const Type*, size_t)>& dist_meas_fun) override;

private:
    static ReflectionRegister(KmeansSolver, KmeansBinSolver) regis_kmeans_bin_solver;

    //Bin Members
    std::unique_ptr<KmeansSolver> m_base_solver;

    std::vector<std::vector<Type>> m_cent_list;
    wzp::Matrix<Type> m_cluster_assment;

private:
    //
    void process();

    Type compute_not_split_sse(size_t i);

    void update_best_cluster_ass(wzp::Matrix<Type>& best_cluster_ass, size_t len, size_t best_cent_to_split);

};

} //aiolos

#endif /*AIOLOS_KMENAS_BIN_SOLVER_HPP*/