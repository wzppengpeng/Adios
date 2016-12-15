#include "session.hpp"

using namespace std;

namespace adios {

Session::Session(io_service& ios) : m_socket(ios) {}

Session::~Session() {}

void Session::handle_read() {
    async_read(m_socket, buffer(m_buffer), transfer_at_least(1),
     [this](const boost::system::error_code& ec, size_t size) {
        if(ec){
            handle_error(ec);
            return;
        }
        handle_request(m_buffer.data());
    });
}

/*the interface to write data response into socket*/
void Session::handle_write(const char* data, int len){
    async_write(m_socket, buffer(data, len),
     [this](const boost::system::error_code& ec, size_t size){
        if(ec){
            handle_error(ec);
            return;
        }
        close();
    });
}

/*close the socket*/
void Session::close() {
    boost::system::error_code ec;
    m_socket.shutdown(tcp::socket::shutdown_send, ec);
    m_socket.close(ec);
    wzp::log::info("socket is closing, conn_id is", m_conn_id);
    if(m_callback_error)
        m_callback_error(m_conn_id);
}

//the callback handle error function
void Session::handle_error(const boost::system::error_code& ec) {
    close();
    log::error("the error reason", ec.message());
}

//the callback handle request
void Session::handle_request(const char* data) {
    std::string str = "";
    if(m_callback_handle)
        str = std::move(m_callback_handle(std::string(data)));
    //write the response string to socket
    handle_write(str.c_str(), str.size());
}

} // adios