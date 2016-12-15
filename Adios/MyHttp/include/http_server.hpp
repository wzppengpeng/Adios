#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include <memory>
#include <unordered_map>

#include "receiver.hpp"
#include "function/help_function.hpp"

namespace adios {

class HttpServer
{
public:
    HttpServer(short port);
    ~HttpServer();

    //registry
    inline void registry(std::unordered_map<std::string, std::string>&& factory) {
        m_factory = factory;
    }

    inline void registry(const std::unordered_map<std::string, std::string>& factory) {
        m_factory = factory;
    }

    void run();

private:
    void init();

    std::string response(const std::string& req);

private:
    io_service m_ios;

    short m_port;

    std::unique_ptr<Receiver> m_receiver;

    //need the http request parser and http response generator
    std::unordered_map<std::string, std::string> m_factory;
};

} // adios


#endif // HTTP_SERVER_HPP
