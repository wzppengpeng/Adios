#ifndef RECEIVER_HPP
#define RECEIVER_HPP

#include <unordered_map>
#include <list>
#include <memory>
#include <algorithm>
#include <functional>
#include <string>

#include <boost/asio.hpp>

#include "session.hpp"

using namespace boost::asio;
using wzp::log;
namespace adios {


class Receiver {
public:
    Receiver(io_service& ios, short port);
    ~Receiver();

    void accept();

    /*set the callback function of handle request*/
    template<typename Fun>
    void set_callback_handle(Fun f) {
        m_callback_handle = f;
    }

    /*get the number of sessions*/
    inline size_t get_sessions_num () const {
        return m_handlers.size();
    }

private:
    //the handle of accept error
    void handle_accept_error(std::shared_ptr<Session> event_handler, const boost::system::error_code& ec);

    //stop the acceptor
    void stop_accept();

    //create a Session
    std::shared_ptr<Session> create_handler();

    //if error with read or write, calloc this function
    void recycle_conn_id(int conn_id);

private:
    io_service& m_ios;

    enum {max_connect_num = 65536};
    tcp::acceptor m_acceptor;

    std::unordered_map<int, std::shared_ptr<Session>> m_handlers;

    std::list<int> m_conn_id_pool;

    //the callback to pass in Session
    std::function<std::string(const std::string&)> m_callback_handle;

};

} // adios
#endif // RECEIVER_HPP
