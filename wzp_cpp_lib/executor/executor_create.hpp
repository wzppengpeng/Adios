#ifndef EXECUTOR_CREATE_HPP_
#define EXECUTOR_CREATE_HPP_

#include <memory>

//need include the execut head file

namespace wzp {

enum class ExecutorType {

};

std::unique_ptr<ExecutorStr> create(ExecutorType type) {
    switch(type) {
        default:
            return std::unique_ptr<ExecutorStr>(nullptr);
    }
}

} // wzp


#endif // EXECUTOR_CREATE_HPP_
