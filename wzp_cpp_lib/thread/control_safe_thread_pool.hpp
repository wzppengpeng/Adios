#ifndef WZP_CPP_LIB_CONTROL_SAFE_THREAD_POOL_HPP_
#define WZP_CPP_LIB_CONTROL_SAFE_THREAD_POOL_HPP_

#include <thread>
#include <functional>
#include <vector>

#include "wzp_cpp_lib/container/control_safe_queue.h"

namespace wzp {

    /*manage the threads(vector<thread>)*/
    class join_threads {
    private:
        std::vector<std::thread> &threads;
    public:
        explicit join_threads(std::vector<std::thread> &threads_)
            : threads(threads_) {}
        ~join_threads() {
            for(auto &th : threads) {
                if(th.joinable()) {
                    th.join();
                }
            }
        }
    };

    // thread pool with safe close funtion
    class ControlSafeThreadPool {

    private:
        using Task = std::function<void()>;
        control_safe_queue<Task> task_queue;
        std::vector<std::thread> threads;
        join_threads join_threads_;

    private:
        // the work function for each thread
        void work_thread() {
            for( ; ; ) {
                Task task;
                if(task_queue.wait_and_pop(task)) {
                    task();
                } else {
                    break;
                }
            }
        }

    public:
        ControlSafeThreadPool(const int pool_num = -1)
            : task_queue(),
              threads(),
              join_threads_(threads) {
            int real_pool_num;
            if(pool_num <= 0) {
                real_pool_num = std::thread::hardware_concurrency();
            } else {
                real_pool_num = pool_num;
            }
            for(int i = 0; i < real_pool_num; ++i) {
                threads.emplace_back(&ControlSafeThreadPool::work_thread, this);
            }
        }

        ~ControlSafeThreadPool() {
            task_queue.close();
        }

        // submit single task
        template<typename F>
        void Submit(F &&f) {
            task_queue.push(Task(std::forward<F>(f)));
        }

        // the manual close function
        void Close() {
            task_queue.close();
        }

        // the join function to block the main thread
        void Join() {
            for(auto& th : threads) {
                if(th.joinable()) {
                    th.join();
                }
            }
        }

    };

} //wzp

#endif /*WZP_CPP_LIB_CONTROL_SAFE_THREAD_POOL_HPP_*/