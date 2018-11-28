#ifndef MY_TIMER_HPP_
#define MY_TIMER_HPP_
#include <chrono>

/**
 * How to Use:
 * Timer t;
 * auto time_used = t.elapsed(); // ms, to print or other using
 * t.reset();
 */

namespace wzp{

class Timer{
public:
    Timer() : m_begin(std::chrono::high_resolution_clock::now()) {}
    //reset
    void reset(){
        m_begin = std::chrono::high_resolution_clock::now();
    }

    //ms
    int64_t elapsed() const{
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - m_begin).count();
    }

    //us
    int64_t elapsed_micro() const{
        return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - m_begin).count();
    }

    //ns
    int64_t elapsed_nano() const{
        return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_begin).count();
    }

    //s
    int64_t elapsed_seconds() const{
        return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - m_begin).count();
    }

    //minutes
    int64_t elapsed_minutes() const{
        return std::chrono::duration_cast<std::chrono::minutes>(std::chrono::high_resolution_clock::now() - m_begin).count();
    }

    //hour
    int64_t elapsed_hours() const{
        return std::chrono::duration_cast<std::chrono::hours>(std::chrono::high_resolution_clock::now() - m_begin).count();
    }
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_begin;//the clock to get the time begin
};

}//wzp

#endif /*MY_TIMER_HPP_*/