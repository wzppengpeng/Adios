#include "http_server.hpp"
#include "handle_factory.hpp"

using namespace std;

namespace adios {

HttpServer::HttpServer(short port) : m_ios(),
                                     m_port(port),
                                     m_receiver(nullptr)
{
    init();
}

HttpServer::~HttpServer() {}

void HttpServer::run() {
    m_receiver->accept();
    m_ios.run();
}

void HttpServer::init() {
    wzp::log::log_init(wzp::log_level::Info, wzp::log_type::Console);
    m_receiver = wzp::make_unique<Receiver>(m_ios, m_port);
    m_receiver->set_callback_handle([this](const std::string& req){ return response(req); });
}

std::string HttpServer::response(const std::string& req) {
    HttpParser parser;
    parser.parse(req);
    auto handle_ptr = HandleFactory::create(m_factory[parser.get_url()]);
    std::string ret;
    std::string res;
    if(handle_ptr.get() != nullptr) {
        if(parser.get_request_type() == RequestType::GET) {
            ret = handle_ptr->get(parser);
            res = HttpResponse::headers();
            res.append(ret);
        }
        else if(parser.get_request_type() == RequestType::POST){
            ret = handle_ptr->post(parser);
            res = HttpResponse::headers();
            res.append(ret);
        }
        else {
            res = HttpResponse::unimplemented();
        }
    }
    else {
        res = HttpResponse::not_found();
    }
    return res;
}

} // adios