#ifndef WZP_CPP_LIB_THREAD_LOCK_HPP_
#define WZP_CPP_LIB_THREAD_LOCK_HPP_

/**
 * 实现自旋锁等并发场景下的高科技手段
 */

#include <atomic>

#include "wzp_cpp_lib/uncopyable.hpp"

namespace wzp
{

/**
 * A simple class of implement of spinlock
 */
class SpinLock : Uncopyable {

public:

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wbraced-scalar-init"
    SpinLock() : lock_(ATOMIC_FLAG_INIT) {}
#pragma clang diagnostic pop

    ~SpinLock() = default;

    /*!
    * \brief Acquire lock.
    */
    inline void lock() noexcept(true) {
        while (lock_.test_and_set(std::memory_order_acquire)) {
        }
    }
    /*!
    * \brief Release lock.
    */
    inline void unlock() noexcept(true) {
        lock_.clear(std::memory_order_release);
    }

private:
    // the lock flag
    std::atomic_flag lock_;

};

} // wzp

#endif /*WZP_CPP_LIB_THREAD_LOCK_HPP_*/