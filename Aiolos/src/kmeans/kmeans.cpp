#include "kmeans/kmeans.hpp"

#include "util/matrix_args.hpp"

using namespace std;
using wzp::Matrix;


namespace aiolos
{

//reflection
ReflectionRegister(Cluster, Kmeans) regis_kmeans("kmeans");

void Kmeans::init(wzp::ConfigParser* config_parser) {
    m_config_parser = config_parser;
    config_parser->get("k", k);
    config_parser->get("method", task);
    m_solver = wzp::Reflection<KmeansSolver>::create_unique(task);
}

wzp::Matrix<Type> Kmeans::run(wzp::Matrix<Type>& data_set) {
    m_data_set = &data_set;
    m = data_set.rows(); n = data_set.cols();
    auto res = m_solver->solve(data_set, k,
        [](const Type* a, const Type* b, size_t len) {
            return MatrixArgs::compute_distance(a, b, len);
        });
    return res.first;
}

} //aiolos