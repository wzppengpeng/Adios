#ifndef MAPPER_HPP
#define MAPPER_HPP

#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>

#include "redis/redis_oper.hpp"

namespace adios {

class Mapper {
public:
    Mapper(const std::string& host, int port, const std::string& list_name, const std::string& task_name) :
    m_redis_host(host),
    m_redis_port(port),
    m_list_name(list_name),
    m_task_name(task_name),
    m_oper(host, port)
    {

    }

    virtual ~Mapper() {}

    virtual void change(const std::string& list_name, const std::string& task_name) {
        m_list_name = list_name;
        m_task_name = task_name;
    }

    //split job from vector<string>
    //return the split nums
    virtual size_t split(const std::vector<std::string>& jobs) {
        for(size_t i = 0; i < jobs.size(); ++i) {
            if(!m_oper.rpush(m_list_name, m_task_name + std::to_string(i + 1) + ":" + jobs[i]))
                throw std::logic_error("can not push into redis list");
        }
        return jobs.size();
    }

    //split job from file
    virtual size_t split(const std::string& filename) {
        std::ifstream infile(filename, std::ios::in);
        if(!infile)
            throw std::logic_error("can not open the split file");
        size_t i = 0;
        std::string line;
        while(infile >> line) {
            if(!m_oper.rpush(m_list_name, m_task_name + std::to_string(++i) + ":" + line))
                throw std::logic_error("can not push into redis list");
        }
        return i;
    }

protected:
    std::string m_redis_host;
    int m_redis_port;
    std::string m_list_name;
    //the task name
    std::string m_task_name;

    wzp::RedisOper m_oper;
};


} // adios

#endif // MAPPER_HPP
