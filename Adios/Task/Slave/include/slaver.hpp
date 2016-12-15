#ifndef SLAVER_HPP
#define SLAVER_HPP


#include "worker.hpp"

#include "config/config_parser.hpp"
#include "function/help_function.hpp"
#include "log/log.hpp"

using wzp::ConfigParser;
using wzp::log;

namespace adios {

class Slaver {
public:
    Slaver(const std::string& config) : m_parser(config), m_worker(nullptr), m_listener(nullptr) {
        log::log_init(wzp::log_level::Info, wzp::log_type::Console);
        if(!parse()) {
            log::fatal("can not parse the config file");
        }
        init();
    }

    void start() {
        m_listener->run();
    }

private:
    bool parse() {
        if(!m_parser.config()) {
            return false;
        }
        if(!m_parser.get("host", m_redis_host)) return false;
        if(!m_parser.get("port", m_redis_port)) return false;
        if(!m_parser.get("list_name", m_list_name)) return false;
        std::string type;
        if(!m_parser.get("return_type", type)) return false;
        if(type == "Void") {
            m_type = ReturnType::Void;
        }
        else if(type == "String") {
            m_type = ReturnType::String;
        }
        else return false;
        return true;
    }

    // init the worker
    inline void init() {
        m_worker = wzp::make_unique<Worker>(m_type, m_redis_host, m_redis_port);
        m_listener = wzp::make_unique<wzp::RedisTaskListen>(m_redis_host, m_redis_port, m_list_name);
        m_listener->set_handler([this](const std::string& input){
            m_worker->run(input);
        });
    }

private:
    ConfigParser m_parser;// the parser

    //the ip of the redis
    std::string m_redis_host;
    int m_redis_port;

    //the list name to listen
    std::string m_list_name;

    //ReturnType : Void for Void, String for String
    ReturnType m_type;

    std::unique_ptr<Worker> m_worker;// the work

    RedisTaskListenPtr m_listener;

};

} // adios

#endif // SLAVER_HPP
