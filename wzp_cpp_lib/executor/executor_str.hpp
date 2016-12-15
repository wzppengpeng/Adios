#ifndef EXECUTOR_STR_HPP_
#define EXECUTOR_STR_HPP_

#include <string>

namespace wzp {

class ExecutorStr
{
public:
    virtual ~ExecutorStr(){}

    virtual void run(const std::string& s) {};

};

} // wzp


#endif // EXECUTOR_STR_HPP_
