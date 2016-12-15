#ifndef SLAVE_WORKER_HPP
#define SLAVE_WORKER_HPP

#include <stdexcept>
//the handle interface
#include "handle.hpp"

#include "redis/redis_task_listen.hpp"
#include "log/log.hpp"

using wzp::log;

namespace adios {

//the return type enum, only two
enum class ReturnType {
    Void,
    String
};

class Worker {
public:
    Worker(ReturnType type, const std::string& host, int port) :
        m_type(type),
        m_handler(nullptr),
        m_oper(host, port)
    {
        init();
    }

    void run(const std::string& input) {
        try{
            parse(input);
        }
        catch(std::logic_error& ex) {
            log::error("exception happened", ex.what());
            return;
        }
        log::info("receive a task", "task id is", m_taskid);
        if(!m_oper.hash_set(m_taskid, "state", "DOING"))
            throw std::logic_error("can not write state");
        if(m_type == ReturnType::Void) {
            m_handler->run_void(m_task);
        }
        else {
            auto res = m_handler->run_string(m_task);
            write(res);
        }
        log::info("the task", m_taskid, "is done...");
        if(!m_oper.hash_set(m_taskid, "state", "FINISHED"))
            throw std::logic_error("can not write state");
    }

private:
    //init the m_handle
    inline void init() {
        m_handler = Handler::create_unique();
    }

    //the parse function, parse the input string get from redis
    //the content is taskid:xxxxxxx
    inline void parse(const std::string& input) {
        auto pos = input.find_first_of(':');
        if(pos == std::string::npos)
            throw std::logic_error("the input is not ok");
        m_taskid = std::move(input.substr(0, pos));
        m_task = std::move(input.substr(pos + 1));
    }

    inline void write(const std::string& res) {
        if(!m_oper.hash_set(m_taskid, "res", res))
            throw std::logic_error("can not write res");
    }

private:
    ReturnType m_type = ReturnType::Void;// record the return type
    std::unique_ptr<Handler> m_handler;// the handler, has run function

    std::string m_taskid;// taskid of input

    std::string m_task;// task of the input

    wzp::RedisOper m_oper;
};

} // adios


#endif // SLAVE_WORKER_HPP
