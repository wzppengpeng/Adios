#ifndef SLAVE_HANDLE_HPP
#define SLAVE_HANDLE_HPP

#include <string>
#include <memory>

namespace adios {

class Handler
{
public:
    virtual ~Handler() {}

    /*abstract function of run*/
    //the void run function, set the config, if do not need return value
    virtual void run_void(const std::string& task) {}

    //the string run function, run then generate a response string, set the config need return value
    virtual std::string run_string(const std::string& task) { return ""; }

    static std::unique_ptr<Handler> create_unique();// here need to add the handle config!!!

    static std::shared_ptr<Handler> create_shared();
};


} // adios


#endif // SLAVE_HANDLE_HPP
