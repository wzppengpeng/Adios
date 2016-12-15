#ifndef TEST_HANDLE_HPP
#define TEST_HANDLE_HPP

#include <iostream>
#include "handle.hpp"

using std::cout;
using std::endl;

namespace adios {

class TestHandle : public Handle {
public:
    std::string get(const adios::HttpParser& parser) override {
        auto name = parser.Get<std::string>("name");
        cout<<"name is: "<<name<<endl;
        auto id = parser.Get<int>("id");
        cout<<"id is: "<<id<<endl;
        return "get";
    }

    std::string post(const adios::HttpParser& parser) override {
        auto name = parser.Get<std::string>("name");
        cout<<"name is: "<<name<<endl;
        auto id = parser.Get<int>("id");
        cout<<"id is: "<<id<<endl;
        return "post";
    }
};


} // adios

#endif // TEST_HANDLE_HPP
