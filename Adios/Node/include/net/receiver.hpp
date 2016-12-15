#ifndef NODE_RECEIVER_HPP_
#define NODE_RECEIVER_HPP_

#include <unordered_map>
#include <list>
#include <memory>
#include <algorithm>
#include <functional>
#include <string>

#include <boost/asio.hpp>

#include "net/session.hpp"

namespace adios {

using namespace boost::asio;
using wzp::log;

class Receiver {

public:
    /*constructor*/
    Receiver(io_service& ios, short port) : m_ios(ios), m_acceptor(ios, tcp::endpoint(tcp::v4(), port)), m_conn_id_pool(max_connect_num) {
        auto current = 0;
        std::generate_n(begin(m_conn_id_pool), max_connect_num, [&current]{return ++current; });
    }

    ~Receiver() {}

    /*accept a connect as a session*/
    void accept() {
        log::info("Start listening...");
        auto handler = create_handler();
        m_acceptor.async_accept(handler->get_socket(), [this, handler](const boost::system::error_code& ec){
            if(ec){
                wzp::log::error("the error reason:", ec.message());
                handle_accept_error(handler, ec);
            }

            m_handlers.insert({handler->get_connect_id(), handler});
            handler->handle_read();
            accept();
        });
    }

    /*set the callback function of handle request*/
    template<typename Fun>
    void set_callback_handle(Fun f) {
        m_callback_handle = f;
    }

    /*get the number of sessions*/
    size_t get_sessions_num () const {
        return m_handlers.size();
    }

private:
    //the handle of accept error
    void handle_accept_error(std::shared_ptr<Session> event_handler, const boost::system::error_code& ec) {
        wzp::log::error("error reason", ec.message());
        event_handler->close();
        stop_accept();
    }

    //stop the acceptor
    void stop_accept() {
        boost::system::error_code ec;
        m_acceptor.cancel(ec);
        m_acceptor.close(ec);
        m_ios.stop();
    }

    //create a Session
    std::shared_ptr<Session> create_handler() {
        int conn_id = m_conn_id_pool.front();
        m_conn_id_pool.pop_front();
        auto handler = std::make_shared<Session>(m_ios);

        handler->set_connect_id(conn_id);
        handler->set_callback_error([this](int conn_id){
            recycle_conn_id(conn_id);
        });
        if(m_callback_handle) {
            handler->set_callback_handle(m_callback_handle);
        }

        return handler;
    }

    //if error with read or write, calloc this function
    void recycle_conn_id(int conn_id) {
        auto it = m_handlers.find(conn_id);
        if(it != m_handlers.end())
            m_handlers.erase(it);
        m_conn_id_pool.push_back(conn_id);
    }

private:
    io_service& m_ios;

    enum {max_connect_num = 65536};
    tcp::acceptor m_acceptor;
    std::unordered_map<int, std::shared_ptr<Session>> m_handlers;
    std::list<int> m_conn_id_pool;

    //the callback to pass in Session
    std::function<std::string(std::string)> m_callback_handle;

};

} // adios


#endif /*NODE_RECEIVER_HPP_*/