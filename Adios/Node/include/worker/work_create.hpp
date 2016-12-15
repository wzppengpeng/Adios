#ifndef WORK_CREATE_HPP
#define WORK_CREATE_HPP

#include <memory>

//this is for test, the actual use only need to modify this file

#include "worker/test_work1.hpp"
#include "worker/test_work2.hpp"

namespace work {

enum class WorkType {
    One,
    Two
};

std::unique_ptr<wzp::ExecutorStr> create(WorkType type) {
    switch(type) {
        case WorkType::One :
        {
            return std::unique_ptr<wzp::ExecutorStr>(new TestWork1());
            break;
        }
        case WorkType::Two :
        {
            return std::unique_ptr<wzp::ExecutorStr>(new TestWork2());
            break;
        }
        default:
        {
            return std::unique_ptr<wzp::ExecutorStr>(nullptr);
            break;
        }
    }
}
}
#endif // WORK_CREATE_HPP
