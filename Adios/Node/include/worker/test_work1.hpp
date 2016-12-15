#ifndef TEST_WORK1_HPP_
#define TEST_WORK1_HPP_

#include "executor/executor_str.hpp"
#include "function/help_function.hpp"

class TestWork1 : public wzp::ExecutorStr
{
public:
    TestWork1(){}
    ~TestWork1(){}

    void run(const std::string& str) {
        wzp::print("wzp", str);
    }
};


#endif // TEST_WORK1_HPP_
