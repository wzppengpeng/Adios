/*
this file need to genereate a server, so it should be a
 which listen to a port, then when get a message of begin, then it begin to run work
 async and can get other messages.
 this is for future use, now most important is that let it run the job and don't block
 */

#ifndef WORKER_NODE_HPP_
#define WORKER_NODE_HPP_

#include <memory>
#include <thread>
#include <atomic>

#include "log/log.hpp"
#include "my_string/string.hpp"
#include "net/net_engine.hpp"
#include "worker/worker.hpp"

using work::WorkType;
using wzp::log;

namespace adios {

class WorkerNode
{
public:
    WorkerNode(short port) :
        m_worker_ptr(nullptr),
        m_port(port),
        m_net_engine(port),
        m_is_start(false)
    {
        init();
    }

    ~WorkerNode(){}

    //some import sets function
    //set redis_ip
    void set_redis_ip(std::string&& ip) {
        m_redis_ip = ip;
    }
    void set_redis_ip(const std::string& ip) {
        m_redis_ip = ip;
    }

    //set the port of redis
    void set_redis_port(int port) {
        m_redis_port = port;
    }

    //important!! set the redis_list_name, where the task from
    void set_redis_list_name(std::string&& name) {
        m_redis_list_name = name;
    }
    void set_redis_list_name(const std::string& name) {
        m_redis_list_name = name;
    }


    //the server run function
    void run() {
        m_net_engine.start([this](std::string upload){
            return response(upload);
        });
        m_net_engine.run();
    }

private:
    inline void init() {
        //init the log
        log::log_init(wzp::log_level::Info, wzp::log_type::Console);
    }

    /*put all operation here*/

    //the comand parser
    //need use protobuf future
    //this is for temp test
    //TODO:
    inline bool parse(const std::string& input) {
        return input == "start_work" ? true : false;
    }

    //start the worker
    void start(WorkType type, bool use_multi_thread = false, int thread_num = 4) {
        if(m_redis_list_name.empty()) {
            log::error("the redis list name is not set");
            return;
        }
        if(m_worker_ptr.get() != nullptr)
            return;
        m_worker_ptr = std::unique_ptr<Worker>(new Worker());
        m_worker_ptr->run(m_redis_ip, m_redis_port, m_redis_list_name, type, use_multi_thread, thread_num);
    }

    //the response function
    std::string response(std::string request) {
        //TODO:
        log::info("the input is:", request);
        auto req_vec = wzp::split_string(request, ',');
        if(parse(req_vec.front())) {
            //TODO:
            WorkType type;
            type = req_vec[1] == "1" ? (WorkType::One) : (WorkType::Two);
            bool use_multi_thread = false;
            if(req_vec.size() >= 3) {
                use_multi_thread = true;
            }
            if(m_is_start == true)
                return "the job has been started";
            std::thread t([this, type, use_multi_thread]{
                start(type, use_multi_thread);
            });
            t.detach();
            m_is_start = true;
        }
        //TODO:
        return "receive ok";
    }

private:
    //two most import module
    //the worker
    std::unique_ptr<Worker> m_worker_ptr;
    //the net_engine
    //the port to listen
    short m_port;
    NetEngine m_net_engine;

    //check the job is start
    std::atomic<bool> m_is_start;
    //redis settings
    std::string m_redis_ip = "127.0.0.1";
    int m_redis_port = 6379;
    std::string m_redis_list_name {};
};

} // adios


#endif // WORKER_NODE_HPP_
