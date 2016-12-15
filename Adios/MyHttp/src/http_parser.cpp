#include <iostream>
#include "http_parser.hpp"
#include "my_string/string.hpp"

using namespace std;
using namespace wzp;


namespace adios {

HttpParser::~HttpParser() {}

//the parse function
void HttpParser::parse(const std::string& req) {
    auto first_line = std::move(get_first_line(req));
    parse_url(first_line);
    if(first_line.substr(0, 4) == "POST") {
        m_type = RequestType::POST;
        parse_post(req);
    }
    else if(first_line.substr(0, 3) != "GET") {
        m_type = RequestType::WRONG;
    }
    else {
        parse_get(first_line);
    }
}

std::string HttpParser::get_first_line(const std::string& req) {
    auto end = req.find_first_of('\n');
    return req.substr(0, end);
}

void HttpParser::parse_url(const std::string& first_line) {
    auto begin = first_line.find_first_of('/');
    auto end = first_line.find_first_of(" ?", begin);
    m_url = first_line.substr(begin, end - begin);
}

void HttpParser::parse_get(const std::string& first_line) {
    auto begin = first_line.find('?');
    if(begin == std::string::npos) return;
    auto end = first_line.find_first_of(' ', begin);
    auto data_list = std::move(first_line.substr(begin+1, end - begin - 1));
    parse_params(data_list);
}

void HttpParser::parse_post(const std::string& req) {
    auto begin = req.find_last_of("\r\n");
    auto data_list = std::move(req.substr(begin + 1));
    parse_params(data_list);
}

void HttpParser::parse_params(const std::string& data_list) {
    auto str_vec = std::move(split_string(data_list, '&'));
    for(auto& str : str_vec) {
        auto v = std::move(split_string(str, '='));
        m_param_map[v[0]] = v[1];
    }
}

std::string HttpResponse::HttpResponse::headers() {
    string res;
    res.append("HTTP/1.1 200 OK\r\n");
    res.append(server_info);
    res.append("Content-Type: text/html\r\n");
    res.append("\r\n");
    return res;
}

std::string HttpResponse::not_found() {
    string res;
    res.append("HTTP/1.1 404 NOT FOUND\r\n");
    res.append(server_info);
    res.append("Content-Type: text/html\r\n");
    res.append("\r\n");
    res.append("<HTML><TITLE>Not Found</TITLE>\r\n");
    res.append("<BODY><P>The server could not fulfill\r\n");
    res.append("your request because the resource specified\r\n");
    res.append("is unavailable or nonexistent.\r\n");
    res.append("</BODY></HTML>\r\n");
    return res;
}

std::string HttpResponse::unimplemented() {
    string res;
    res.append("HTTP/1.0 501 Method Not Implemented\r\n");
    res.append(server_info);
    res.append("Content-Type: text/html\r\n");
    res.append("\r\n");
    res.append("<HTML><HEAD><TITLE>Method Not Implemented\r\n");
    res.append("</TITLE></HEAD>\r\n");
    res.append("<BODY><P>HTTP request method not supported.\r\n");
    res.append("</BODY></HTML>\r\n");
    return res;
}

std::string HttpResponse::server_info = "Server: adioshttpd/0.1.0\r\n";

} // adios
