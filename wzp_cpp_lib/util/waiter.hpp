#ifndef UTILS_WAITER_HPP_
#define UTILS_WAITER_HPP_

#include <mutex>
#include <condition_variable>

namespace wzp
{

/**
 * different threads can use the same waiter
 * same waiter control the thread to block by condition variable
 * user can set the wait num, the wait thread will final notify when the num into zero
 */
class Waiter {

public:
    explicit Waiter(int num_wait = 1) : m_num_wait(num_wait) {}

    // the wait interface to set the wait thread to wait until the num wait -- to zero
    void Wait() {
        std::unique_lock<std::mutex> lock(m_mut);
        while (m_num_wait > 0) m_cv.wait(lock);
    }

    // the notify interface to minus the num wait one
    // and call the notify interface to wake up the wait threads
    void Notify() {
        std::unique_lock<std::mutex> lock(m_mut);
        --m_num_wait;
        m_cv.notify_all();
    }

    // reset the num wait value
    void Reset(int num_wait = 1) {
        std::unique_lock<std::mutex> lock(m_mut);
        m_num_wait = num_wait;
    }


private:
    mutable std::mutex m_mut;
    std::condition_variable m_cv;
    int m_num_wait;

};

} //wzp

#endif /*UTILS_WAITER_HPP_*/