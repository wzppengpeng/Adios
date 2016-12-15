#ifndef NODE_WORKER_HPP_
#define NODE_WORKER_HPP_

#include <memory>
#include <vector>
#include <chrono>

#include "redis/redis_task_listen.hpp"
#include "worker/work_create.hpp"
#include "thread/thread_pool.hpp"

using work::WorkType;
using wzp::RedisTaskListen;
using wzp::thread_pool;

namespace adios {

class Worker
{
public:
    Worker() :
        task_ptr(nullptr),
        work_ptr(nullptr),
        pool_ptr(nullptr),
        task_ptr_vec()
        {}

    ~Worker() {}

    void run(const std::string& ip, int port, const std::string& list_name,
        WorkType type, bool use_multi_thread = false, int thread_num =  4) {
        if(!use_multi_thread) {
            run_single(ip, port, list_name, type);
        }
        else {
            run_multi_thread(ip, port, list_name, type, thread_num);
        }
    }

private:
    void run_single(const std::string& ip, int port, const std::string& list_name, WorkType type) {
        work_ptr = work::create(type);
        task_ptr = RedisTaskListenPtr(new RedisTaskListen(ip, port, list_name));
        task_ptr->set_handler([this](const std::string& res){work_ptr->run(res);});
        task_ptr->run();
    }

    void run_multi_thread(const std::string& ip, int port,
        const std::string& list_name, WorkType type, int thread_num) {
        work_ptr = work::create(type);//this place may be need more instance
        task_ptr_vec.reserve(thread_num);
        for(int i = 0; i < thread_num; ++i) {
            task_ptr_vec.emplace_back(RedisTaskListenPtr(new RedisTaskListen(ip, port, list_name)));
            task_ptr_vec.back()->set_handler([this](const std::string& res){ work_ptr->run(res); });
        }
        //init the thread pool
        pool_ptr = ThreadPoolPtr(new thread_pool(thread_num));
        for(int i = 0; i < thread_num; ++i) {
            pool_ptr->submit([this, i]{ task_ptr_vec[i]->run(); });
        }
        while(true){
            std::chrono::milliseconds dura(2000);
            std::this_thread::sleep_for(dura);
        }
    }

private:
    //the RedisTaskListenPtr to listen the queue
    RedisTaskListenPtr task_ptr;
    std::unique_ptr<wzp::ExecutorStr> work_ptr;
    //work_ptr should init first

    //the thread pool
    //defult is not use the thread pool
    ThreadPoolPtr pool_ptr;
    std::vector<RedisTaskListenPtr> task_ptr_vec;
};

} // wzp


#endif // NODE_WORKER_HPP_
