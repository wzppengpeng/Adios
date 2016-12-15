#ifndef HANDLE_FACTORY_HPP
#define HANDLE_FACTORY_HPP

#include <string>
#include <memory>

#include "test_handle.hpp"

namespace adios {

class HandleFactory {
public:
    static std::shared_ptr<Handle> create(const std::string& key) {
        if(key == "test") {
            return std::make_shared<TestHandle>();
        }
        else {
            return std::shared_ptr<Handle>(nullptr);
        }
    }
};

} // adios

#endif // HANDLE_FACTORY_HPP
