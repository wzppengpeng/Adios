#ifndef HTTP_PARSER_HPP
#define HTTP_PARSER_HPP

#include <string>
#include <unordered_map>

#include "my_string/string.hpp"

namespace adios {

enum class RequestType {
    GET,
    POST,
    WRONG
};

class HttpParser
{
public:
    HttpParser() = default;
    ~HttpParser();

    void parse(const std::string& req);

    //some getters
    inline RequestType get_request_type() const {
        return m_type;
    }

    //get the url location
    inline std::string get_url() const {
        return m_url;
    }

    template<typename T>
    inline T Get(const std::string& key) const {
        auto it = m_param_map.find(key);
        if(it == m_param_map.end()) return T();
        return wzp::convert_string<T>(it->second);
    }

private:
    inline std::string get_first_line(const std::string& req);

    inline void parse_url(const std::string& first_line);

    void parse_get(const std::string& first_line);

    void parse_post(const std::string& req);

    void parse_params(const std::string& data_list);

private:
    RequestType m_type = RequestType::GET;

    std::string m_url = "/";//default is the root location

    std::unordered_map<std::string, std::string> m_param_map; // the params map
};

class HttpResponse {
public:
    static std::string headers();
    static std::string not_found();
    static std::string unimplemented();
private:
    static std::string server_info;
};

} // adios
#endif // HTTP_PARSER_HPP
