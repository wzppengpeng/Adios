#ifndef NODE_NET_ENGINE_HPP_
#define NODE_NET_ENGINE_HPP_


#include <thread>
#include <future>
#include <string>
#include <memory>

#include <boost/asio.hpp>

#include "log/log.hpp"

#include "net/receiver.hpp"
#include "net/connector.hpp"

namespace adios {

using namespace boost::asio;

class NetEngine {
public:
    NetEngine(short port) : m_ios(), m_port(port), m_receiver(nullptr) {}

    /*init function, must set callback of handle request*/
    template<typename Fun>
    void start(Fun f) {
        //let the ios run
        io_service::work work(m_ios);


        m_receiver = std::unique_ptr<Receiver>(new Receiver(m_ios, m_port));
        m_receiver->set_callback_handle(f);
        m_receiver->accept();

        //let the job begin
        std::thread t([this]{m_ios.run();});
        t.detach();
    }

    void run() {
        m_ios.run();
    }

    /*get connect numbers*/
    size_t get_sessions_num() const {
        return m_receiver->get_sessions_num();
    }

    /*the asyn interface to send message to target*/
    template<typename Fun>
    void async_delevery(const std::string& ip, short port, const std::string& data, Fun f) {
        std::async(std::launch::async, [this, &ip, port, &data, f](){
            delivery(ip, port, data, f);
        });
    }

private:

    //the connect function, give the target, and send message and a Function to handle the response from users
    template<typename Fun>
    void delivery(const std::string& ip, short port, const std::string& data, Fun f) {
        Connector conn(m_ios, ip, port);

        conn.set_callback_handle_res(f);
        if(!conn.connect())
            return;

        conn.send(data.c_str(), data.size());
        conn.close();
    }
private:
    io_service m_ios;

    short m_port;

    std::unique_ptr<Receiver> m_receiver;
    std::function<std::string(std::string)> m_callback_handle;
};

} // adios

#endif /*NODE_NET_ENGINE_HPP_*/