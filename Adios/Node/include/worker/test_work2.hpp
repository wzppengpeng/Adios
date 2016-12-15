#ifndef TEST_WORK2_HPP_
#define TEST_WORK2_HPP_

#include "executor/executor_str.hpp"
#include "function/help_function.hpp"

class TestWork2 : public wzp::ExecutorStr
{
public:
    TestWork2(){}
    ~TestWork2(){}

    void run(const std::string& str) {
        wzp::print("I'm not wzp", str);
    }
};


#endif // TEST_WORK2_HPP_
