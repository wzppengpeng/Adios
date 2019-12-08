#ifndef WZP_CPP_LIB_THREAD_THREAD_LOCAL_HPP_
#define WZP_CPP_LIB_THREAD_THREAD_LOCAL_HPP_

/**
 * provide a singleton of thread local getter
 */

#include <utility>

namespace wzp
{

// singleton for each thread
// set the args type in the class template
// so Get(args...) will get the only one instance
// T is the main type, Args is the args types
template<typename T, typename... Args>
class ThreadLocalStore {

public:
    static T* Get(Args... args) {
        static thread_local T t(std::forward<Args>(args)...);
        return &t;
    }

private:
    ThreadLocalStore() {}
    ~ThreadLocalStore() {}

};

} //wzp

#endif /*WZP_CPP_LIB_THREAD_THREAD_LOCAL_HPP_*/