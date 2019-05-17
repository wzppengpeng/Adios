#ifndef WZP_CPP_LIB_THREAD_SLEEP_HPP_
#define WZP_CPP_LIB_THREAD_SLEEP_HPP_

#include <thread>
#include <chrono>

namespace wzp
{

// some typedefs of std time duration
using MilliSeconds = std::chrono::milliseconds;
using NanoSeconds = std::chrono::nanoseconds;
using MicroSeconds = std::chrono::microseconds;
using Seconds = std::chrono::seconds;
using Minutes = std::chrono::minutes;
using Hours = std::chrono::hours;

// the sleep function
template<typename D>
inline void ThreadSleepFor(const D& d) {
    std::this_thread::sleep_for(d);
}

} //wzp

#endif /*WZP_CPP_LIB_THREAD_SLEEP_HPP_*/