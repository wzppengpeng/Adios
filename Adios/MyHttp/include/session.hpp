#ifndef SESSION_HPP
#define SESSION_HPP

#include <array>
#include <string>
#include <functional>

#include <boost/asio.hpp>

#include "log/log.hpp"

using namespace boost::asio;
using boost::asio::ip::tcp;
using wzp::log;

namespace adios {

class Session {
public:
    Session(io_service& ios);
    ~Session();

    void handle_read();

    void handle_write(const char* data, int len);

    //getters
    inline tcp::socket& get_socket() {
        return m_socket;
    }

    inline int get_connect_id() const{
        return m_conn_id;
    }

    //sertter
    inline void set_connect_id(const int conn_id){
        m_conn_id = conn_id;
    }

    /*set the callback of handle and error function*/
    template<typename Fun>
    void set_callback_error(Fun f) {
        m_callback_error = f;
    }

    template<typename Fun>
    void set_callback_handle(Fun f) {
        m_callback_handle = f;
    }

    //close socket
    void close();

private:
    void handle_error(const boost::system::error_code& ec);

    //the callback handle request
    void handle_request(const char* data);

private:
    //the socket
    tcp::socket m_socket;

    enum {MAX_IP_PACK_SIZE = 8 * 1024 * 1024};

    std::array<char, MAX_IP_PACK_SIZE> m_buffer;

    int m_conn_id;//the connect id of this handler

    std::function<void(int)> m_callback_error;
    std::function<std::string(const std::string&)> m_callback_handle;

};


} // adios

#endif // SESSION_HPP
