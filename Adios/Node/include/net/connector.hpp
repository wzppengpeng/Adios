#ifndef NODE_CONNECTOR_HPP_
#define NODE_CONNECTOR_HPP_

#include <string>
#include <functional>

#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>

#include "log/log.hpp"

namespace adios {

using namespace boost::asio;
using namespace boost::asio::ip;
using wzp::log;

class Connector {
    public:
    Connector(io_service& ios, const std::string& ip, short port) :
        m_ios(ios),
        m_socket(ios),
        m_server_addr(tcp::endpoint(address::from_string(ip), port)),
        m_is_connected(false)
    {}

    // the connect function
    bool connect() {
        boost::system::error_code ec;
        m_socket.connect(m_server_addr, ec);
        if (ec) {
            handle_error(ec);
            return false;
        }
        m_is_connected = true;
        return m_is_connected;
    }

    /*send char[] data to the socket, get the response then call the callback function*/
    void send(const char* data, size_t length) noexcept {
        if (length > MaxLength) {
            log::error("the input size of data is too large, the max large should be", MaxLength);
            return;
        }
        boost::system::error_code ec;
        write(m_socket, buffer(data, length), ec);

        if (ec) {
            handle_error(ec);
            return;
        }
        //read the response
        char* res = new char[MaxLength];
        size_t reply_length = read(m_socket, buffer(res, MaxLength), transfer_at_least(1), ec);

        if (ec) {
            handle_error(ec);
            return;
        }
        handle_response(res, reply_length);
        delete res;
        res = nullptr;
    }

    /*the getter to know if the connect is ok*/
    bool is_connected() const {
        return m_is_connected;
    }

    /*the public interface to clsoe the connect*/
    void close() {
        boost::system::error_code ec;
        m_socket.shutdown(tcp::socket::shutdown_send, ec);
        m_socket.close(ec);
    }

    /*set the calback handle function*/
    template<typename Fun>
    void set_callback_handle_res(Fun f) {
        m_calback_handle_res = f;
    }

private:
    //need to hadnle the error
    void handle_error(const boost::system::error_code& ec) {
        m_is_connected = false;
        log::error("the connect is shutdown", ec.message());
        close();
    }

    //the funciton to call the calback
    void handle_response(char* res, size_t length) {
        if(m_calback_handle_res)
            m_calback_handle_res(std::string(res, length));
    }

private:
    io_service& m_ios;

    tcp::socket m_socket;
    //the connect endpoint
    tcp::endpoint m_server_addr;
    bool m_is_connected;
    enum { MaxLength = 2 * 1024 * 1024 };
    //here can get a Funciton object by upper level
    std::function<void(std::string)> m_calback_handle_res;
};

} // adios


#endif /*NODE_CONNECTOR_HPP_*/