#ifndef ADD_HANDLER_H
#define ADD_HANDLER_H

#include "handle.hpp"

using adios::Handler;

class AddHandler : public Handler {
public:
    std::string run_string(const std::string& task) override;
};


#endif // ADD_HANDLER_H
