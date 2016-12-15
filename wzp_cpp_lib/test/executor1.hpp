#ifndef EXE_CUTOR_HPP_
#define EXE_CUTOR_HPP_

#include <iostream>

#include "executor/executor_factory.hpp"
#include "executor/executor.hpp"

class Executor1 : public wzp::Executor {
public:
    Executor1() {
        std::cout<<"Executor1"<<std::endl;
    }
    
    Executor1(int a) {
        std::cout<<"Executor1 "<<a<<std::endl;
    }
    
    ~Executor1() {}
    
    void run() override {
        std::cout<<"I'm run"<<std::endl;
    }
};

REGISTER_EXECUTOR(Executor1, "executor1");

#endif