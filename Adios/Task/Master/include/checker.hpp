#ifndef CHECKER_HPP
#define CHECKER_HPP

#include <stdexcept>

#include "redis/redis_oper.hpp"

namespace adios {

class Checker {
public:
    Checker(const std::string& host, int port)
protected:
    std::string m_redis_host;
    int m_redis_port;
    std::string

};

} // adios


#endif // CHECKER_HPP
