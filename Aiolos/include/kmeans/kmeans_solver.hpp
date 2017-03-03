#ifndef AIOLOS_KMEANS_SOLVER_HPP
#define AIOLOS_KMEANS_SOLVER_HPP

#include <functional>

#include "container/matrix.hpp"

#include "common.hpp"

namespace aiolos
{

class KmeansSolver
{
public:
    virtual ~KmeansSolver() {}

    virtual std::pair<wzp::Matrix<Type>, wzp::Matrix<Type>> solve(wzp::Matrix<Type>& data_set,
     size_t k_, const std::function<Type(const Type*, const Type*, size_t)>& dist_meas_fun) = 0;

protected:
    wzp::Matrix<Type>* m_data_set;
    size_t k;
    std::function<Type(const Type*, const Type*, size_t)> m_dist_meas_fun;
    size_t m;
    size_t n;

    //the cluster result
    wzp::Matrix<Type> m_centroids;

};

} //aiolos


#endif /*AIOLOS_KMEANS_SOLVER_HPP*/