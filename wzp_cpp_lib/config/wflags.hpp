#ifndef WFLAGS_HPP_
#define WFLAGS_HPP_

#include <string>
#include <iostream>
#include <type_traits>
#include <unordered_map>

#include "common.h"

#include "my_string/string.hpp"

namespace wzp {

using std::string;

namespace detail {   
inline static void log_err(const std::string& content) {
    std::cerr << RED << "[Fatal] " << content << RESET << std::endl;
    exit(-1);
}
// trim the str
inline static bool trim(std::string& str) {
    if (str.empty()) return false;
    str.erase(str.find_last_not_of(" \f\t\n\r\v") + 1);
    str.erase(0, str.find_first_not_of(" -\f\t\n\r\v"));
    return true;
}
} // detail

class WFlags {
// some typedef

template<typename K, typename V>
using Dict = std::unordered_map<K, V>;
using ArgsDict = Dict<std::string, std::pair<std::string, std::string>>;

public:
    // the map getter
    static ArgsDict& Get() {
        static ArgsDict args_map;
        return args_map;
    }
    // the help function to print the args
    static void Help() {
        for(const auto& p : Get()) {
            std::cout << "Key: " << YELLOW << p.first << RESET << " Describs: " << GREEN
                << p.second.second << RESET << std::endl;
        }
    }
    // parse functions
    static void Parse(int argc, char** argv) noexcept {
        if(argc != static_cast<int>(Get().size() + 1)) {
            std::cout << MAGENTA << "[Warning] args size mismatch" << RESET << std::endl;
            Help();
        }
        for(int i = 1; i < argc; ++i) {
            parse(i, argv);
        }
    }
    // the functions to add args
    inline static void add_argument(const string& key, const string& desc) {
        Get().emplace(key, std::make_pair("", desc));
    }
    //the right value type
    inline static void add_argument(string&& key, string&& desc) {
        Get().emplace(key, std::make_pair("", desc));
    }
    // add the default value
    template<typename T>
    inline static void add_argument(const string& key, T&& default_val, const string& desc) {
        if(std::is_same<typename std::decay<T>::type, bool>::value)
            Get().emplace(key, std::make_pair(convert_bool_to_string(default_val), desc));
        else
            Get().emplace(key, std::make_pair(std::to_string(default_val), desc));
    }
    template<typename T>
    inline static void add_argument(string&& key, T&& default_val, string&& desc) {
        if(std::is_same<typename std::decay<T>::type, bool>::value)
            Get().emplace(key, std::make_pair(convert_bool_to_string(default_val), desc));
        else
            Get().emplace(key, std::make_pair(std::to_string(default_val), desc));
    }

private:
    static void parse(int index, char** argv) {
        string cache(argv[index]);
        auto pos = cache.find('=');
        if(pos == string::npos) {
            Help();
            detail::log_err("the Args is Set Not Right, USAGE: arg=xx");
        }
        auto key = std::move(cache.substr(0, pos));
        detail::trim(key);
        auto value = std::move(cache.substr(pos+ 1));
        detail::trim(value);
        auto it = Get().find(key);
        if(it == Get().end()) {
            Help();
            detail::log_err("key not exist " + key);
        }
        it->second.first = std::move(value);
    }

}; //WFlags

template<typename T>
struct WFlagsGetter {
    static T get(const std::string& key) {
        auto it = WFlags::Get().find(key);
        if(it == WFlags::Get().end()) {
            detail::log_err("the key is not exist: " + key);
        }
        else {
            if(it->second.first.empty()) {
                detail::log_err("the key is not given: " + key);
            }
            else return convert_string<T>(it->second.first);
        }
        return T();
    }
};

template<>
struct WFlagsGetter<std::string> {
    static std::string get(const std::string& key) {
        auto it = WFlags::Get().find(key);
        if(it == WFlags::Get().end()) {
            detail::log_err("the key is not exist: " + key);
        }
        else {
            if(it->second.first.empty()) {
                detail::log_err("the key is not given: " + key);
            }
            else return it->second.first;
        }
        return string();
    }
};

template<>
struct WFlagsGetter<bool> {
    static bool get(const std::string& key) {
        auto it = WFlags::Get().find(key);
        if(it == WFlags::Get().end()) {
            detail::log_err("the key is not exist: " + key);
        }
        else {
            if(it->second.first.empty()) {
                detail::log_err("the key is not given: " + key);
            }
            else return wzp::convert_string_to_bool(it->second.first);
        }
        return false;
    }
};

} // wzp

// the macro to help function calls
#define WFLAGS_DEFINE(key, desc) wzp::WFlags::add_argument(#key, desc)
#define WFLAGS_DEFINE_DEFAULT(key, default_val, desc) wzp::WFlags::add_argument(#key, default_val, desc)
#define WFLAGS(Type, key) wzp::WFlagsGetter<Type>::get(#key)
// the detail caller for different types
#define WFLAGS_string(key) WFLAGS(std::string, key)
#define WFLAGS_bool(key) WFLAGS(bool, key)
#define WFLAGS_int(key) WFLAGS(int, key)
#define WFLAGS_float(key) WFLAGS(float, key)
#define WFLAGS_double(key) WFLAGS(double, key)
#define WFLAGS_long_long(key) WFLAGS(long long, key)
#define WFLAGS_char(key) WFLAGS(char, key)

#endif // !WFLAGS_HPP_