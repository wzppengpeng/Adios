#ifndef NODE_SESSION_HPP_
#define NODE_SESSION_HPP_

#include <array>
#include <string>
#include <functional>

#include <boost/asio.hpp>

#include "log/log.hpp"

namespace adios {
using namespace boost::asio;
using boost::asio::ip::tcp;
using wzp::log;

class Session {
public:
    /*the constructor init the socket*/
    Session(io_service& ios) : m_socket(ios) {}

    ~Session() {}

    /*the handle_read function, to get the data from the socket, then call the handle, get response string to write to the */
    void handle_read() {
        async_read(m_socket, buffer(m_buffer), transfer_at_least(1), [this](const boost::system::error_code& ec, size_t size) {
            if(ec){
                handle_error(ec);
                return;
            }
            handle_request(m_buffer.data());
        });
    }

    /*the interface to write data response into socket*/
    void handle_write(const char* data, int len){
        async_write(m_socket, buffer(data, len), [this](const boost::system::error_code& ec, size_t size){
            if(ec){
                handle_error(ec);
                return;
            }
            handle_read();
        });
    }

    //some getter
    tcp::socket& get_socket(){
        return m_socket;
    }

    /*close the socket*/
    void close() {
        boost::system::error_code ec;
        m_socket.shutdown(tcp::socket::shutdown_send, ec);
        m_socket.close(ec);
    }

    //set and get connect id
    void set_connect_id(const int conn_id){
        m_conn_id = conn_id;
    }

    int get_connect_id() const{
        return m_conn_id;
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

private:
    //the callback handle error function
    void handle_error(const boost::system::error_code& ec) {
        close();
        log::error("the error reason", ec.message());
        if(m_callback_error)
            m_callback_error(m_conn_id);
    }

    //the callback handle request
    void handle_request(const char* data) {
        std::string str = "";
        if(m_callback_handle)
            str = std::move(m_callback_handle(std::string(data)));
        //write the response string to socket
        handle_write(str.c_str(), str.size());
    }

private:
    //the socket
    tcp::socket m_socket;

    enum {MAX_IP_PACK_SIZE = 2 * 1024 * 1024};

    std::array<char, MAX_IP_PACK_SIZE> m_buffer;
    int m_conn_id;//the connect id of this handler
    std::function<void(int)> m_callback_error;
    std::function<std::string(std::string)> m_callback_handle;
};

} //adios


#endif /*NODE_SESSION_HPP_*/