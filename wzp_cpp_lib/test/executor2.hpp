#ifndef __EXE_CUTOR_HPP_
#define __EXE_CUTOR_HPP_

#include <iostream>

#include "executor/executor_factory.hpp"
#include "executor/executor.hpp"

class Executor2 : public wzp::Executor {
public:
    Executor2() {
        std::cout<<"Executor2"<<std::endl;
    }
    
    Executor2(int a) {
        std::cout<<"Executor2 "<<a<<std::endl;
    }
    
    ~Executor2() {}
    
    void run() override {
        std::cout<<"I'm run 2"<<std::endl;
    }
};

REGISTER_EXECUTOR(Executor2, "executor2");

#endif