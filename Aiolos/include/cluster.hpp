#ifndef AIOLOS_CLUSTER_HPP_
#define AIOLOS_CLUSTER_HPP_

/**
 * the base interface of Cluster
 */

#include "config/config_parser.hpp"
#include "container/matrix.hpp"

#include "common.hpp"

namespace aiolos
{

class Cluster
{
public:
    //destructor
    virtual ~Cluster() {}

    /**
     * the init function
     */
    virtual void init(wzp::ConfigParser* config_parser) = 0;

    /**
     * Cluster function run
     */
    virtual wzp::Matrix<Type> run(wzp::Matrix<Type>& data_set) = 0;
};

} //aiolos


#endif /*AIOLOS_CLUSTER_HPP_*/