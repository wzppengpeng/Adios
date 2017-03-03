#ifndef AIOLOS_KMEANS_HPP
#define AIOLOS_KMEANS_HPP

#include "cluster.hpp"

#include "reflection/reflection.hpp"
#include "kmeans_solver.hpp"

namespace aiolos
{

class Kmeans : public Cluster
{
public:
    Kmeans() = default;

    /**
     * the init function
     */
    void init(wzp::ConfigParser* config_parser) override;

    /**
     * Cluster function run
     */
    wzp::Matrix<Type> run(wzp::Matrix<Type>& data_set) override;

private:
    wzp::ConfigParser* m_config_parser;
    wzp::Matrix<Type>* m_data_set;
    size_t m = 0;
    size_t n = 0;

    /**
     * Kmeans Memebers
     */
    size_t k = 3; // the number of cluster center

    std::unique_ptr<KmeansSolver> m_solver;

    /**
     * Reflection
     */
    static ReflectionRegister(Cluster, Kmeans) regis_kmeans;

};

} //aiolos

#endif /*AIOLOS_KMEANS_HPP*/