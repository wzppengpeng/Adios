#ifndef HANDLE_HPP
#define HANDLE_HPP

#include <string>
#include <iostream>

#include "http_parser.hpp"

namespace adios {


class Handle {
public:
    virtual ~Handle() {}

    virtual std::string get(const adios::HttpParser& parser) {}

    virtual std::string post(const adios::HttpParser& parser) {}
};

} // adios
#endif // HANDLE_HPP
