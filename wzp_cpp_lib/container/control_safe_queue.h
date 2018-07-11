#ifndef CONTROL_SAFE_QUEUE_H_
#define CONTROL_SAFE_QUEUE_H_

#include <mutex>
#include <condition_variable>
#include <queue>

namespace wzp {

    template<typename T>
    class control_safe_queue {

    private:
        mutable std::mutex m_mut;
        std::queue<T> m_data_queue;
        std::condition_variable m_cv;
        bool m_stop;

    public:
        control_safe_queue() : m_stop(false) {}
        ~control_safe_queue() {}

        void push(T new_value) {
            if(m_stop) return;
            std::lock_guard<std::mutex> lck(m_mut);
            m_data_queue.push(std::move(new_value));
            m_cv.notify_one();
        }

        // close the queue
        void close() {
            {
                std::lock_guard<std::mutex> lck(m_mut);
                m_stop = true;
            }
            m_cv.notify_all();
        }

        bool wait_and_pop(T &value) {
            std::unique_lock<std::mutex> lck(m_mut);
            m_cv.wait(lck, [this] {
                return this->m_stop || !this->m_data_queue.empty();
            });
            if(this->m_stop && this->m_data_queue.empty())
                return false;
            if(!m_data_queue.empty()) {
                value = std::move(m_data_queue.front());
                m_data_queue.pop();
            }
            return true;
        }

        bool empty() const {
            std::lock_guard<std::mutex> lck(m_mut);
            return m_data_queue.empty();
        }

    };

} //wzp

#endif /*CONTROL_SAFE_QUEUE_H_*/