#ifndef WZPCPPLIB_CONFIG_WFLAGS_HPP_
#define WZPCPPLIB_CONFIG_WFLAGS_HPP_

/**
 * This file provides a similar flag method as gflags
 * only save the int/double/bool/string four types
 * used in each cpp files
 * main file to define
 * other cpp file can use declare to get the global ref of value
 */

#include <string>
#include <unordered_map>
#include <algorithm>
#include <iostream>

#include "wzp_cpp_lib/common.h"
#include "wzp_cpp_lib/uncopyable.hpp"

namespace wzp
{

namespace wflags
{

// define the wflags command
template<typename T>
struct WFlags {
    T value;
    std::string description;
    bool is_init = false;

    WFlags() = default;

    WFlags(const T& val, const std::string& desc, bool init)
        : value(val), description(desc), is_init(init) {}

};

// define different container to save the flags
template<typename T>
class WFlagsRegister : Uncopyable {

public:
    // register flags into map
    void RegisterFlag(const std::string& name, const std::string& description) {
        flags_[name] = WFlags<T>(T(), description, false);
    }

    // register flags with default value
    void RegisterFlag(const std::string& name, const T& default_val, const std::string& description) {
        flags_[name] = WFlags<T>(default_val, description, true);
    }

    // set the flags if in the default list
    bool SetFlagIfFound(const std::string& key, const T& val) {
        if (flags_.find(key) != flags_.end()) {
            flags_[key].value = val;
            flags_[key].is_init = true;
            return true;
        }
        return false;
    }

    // get the value in the flags
    T& GetValue(const std::string& name) {
        return flags_[name].value;
    }

    // get the description
    const std::string& GetDescription(const std::string& name) {
        return flags_[name].description;
    }

    // check for each value
    bool CheckInit() {
        for (const auto & p : flags_) {
            if (!p.second.is_init) {
                std::cerr << RED << "Miss Flag: --" << p.first << RESET << std::endl;
                return false;
            }
        }
        return true;
    }

    // print the help info
    void Help() {
        for(const auto& p : flags_) {
            std::cout << "Key: " << YELLOW << "--" << p.first << RESET << "   Describes: " << GREEN
                << p.second.description << RESET << std::endl;
        }
    }

    // the getter interface to get the instance
    static WFlagsRegister* GetInstance() {
        static WFlagsRegister instance;
        return &instance;
    }

private:
    // the map container to save flags from command
    std::unordered_map<std::string, WFlags<T>> flags_;

private:
    // the constructor
    WFlagsRegister() {}

}; //WFlagsRegister


// the register helper, use the constructor to register a flag into the map
template<typename T>
struct WFlagsRegisterHelper {
    WFlagsRegisterHelper(const std::string& name, const T& val, const std::string& description) {
        WFlagsRegister<T>::GetInstance()->RegisterFlag(name, val, description);
    }

    WFlagsRegisterHelper(const std::string& name, const std::string& description) {
        WFlagsRegister<T>::GetInstance()->RegisterFlag(name, description);
    }
};

// the set help function to set cmd flag
template<typename T>
inline bool SetCMDFlag(const std::string& name, const T& val) {
    return WFlagsRegister<T>::GetInstance()->SetFlagIfFound(name, val);
}

// parse the command flags
inline static void ParseCMDFlags(int* argc, char** argv) {
    if (argc == nullptr || argv == nullptr) return;

    int unused = 0;
    size_t pos;
    int intval;
    bool boolval;
    double dval;
    std::string line, key, value;

    for (int i = 0; i < *argc; ++i) {
        line = argv[i];
        if (line.find("--") != std::string::npos) {

            pos = line.find("=");
            if (pos == std::string::npos) {
                std::cerr << RED << "Set CMD Flags as --ars=xxx" << RESET << std::endl;
                throw std::runtime_error("Set CMD Flags as --ars=xxx");
            }

            key = line.substr(2, pos - 2);
            value = line.substr(pos + 1);

            if (WFlagsRegister<std::string>::GetInstance()->SetFlagIfFound(key, value)) {
              continue;
            }

            intval = atoi(value.c_str());
            if (WFlagsRegister<int>::GetInstance()->SetFlagIfFound(key, intval)) {
              continue;
            }

            dval = strtod(value.c_str(), nullptr);
            if (WFlagsRegister<double>::GetInstance()->SetFlagIfFound(key, dval)) {
              continue;
            }

            std::transform(value.begin(), value.end(), value.begin(), ::tolower);
            boolval = (value == "true");
            if (WFlagsRegister<bool>::GetInstance()->SetFlagIfFound(key, boolval)) {
              continue;
            }
        }

        std::swap(argv[unused++], argv[i]);
    }

    *argc = unused;

    // finally check if has been init
    if (!WFlagsRegister<std::string>::GetInstance()->CheckInit()
        || !WFlagsRegister<int>::GetInstance()->CheckInit()
        || !WFlagsRegister<double>::GetInstance()->CheckInit()
        || !WFlagsRegister<bool>::GetInstance()->CheckInit()) {
        WFlagsRegister<std::string>::GetInstance()->Help();
        WFlagsRegister<int>::GetInstance()->Help();
        WFlagsRegister<double>::GetInstance()->Help();
        WFlagsRegister<bool>::GetInstance()->Help();
        throw std::runtime_error("Miss Flags");
    }
}

} //wflags

} // wzp


/**
 * ***************************
 * The Macro Part to define flags
 * ***************************
 */

// declare the ref in one cpp file
#define WFLAGS_DECLARE(type, name) \
    static const type& WFLAGS_##name = \
    wzp::wflags::WFlagsRegister<type>::GetInstance()->GetValue(#name);


// define the flags without default value
#define WFLAGS_DEFINE(type, name, description) \
    namespace wzp \
    { \
        namespace wflags \
        { \
            WFlagsRegisterHelper<type> internal_wflags_helper_##name(#name, description); \
        } \
    } \
    WFLAGS_DECLARE(type, name)

// define the flags with default value
#define WFLAGS_DEFINE_DEFAULT(type, name, default_val, description) \
    namespace wzp \
    { \
        namespace wflags \
         { \
             WFlagsRegisterHelper<type> internal_wflags_helper_##name(#name, default_val, description); \
         } \
    } \
    WFLAGS_DECLARE(type, name)


// the alias of fixed types (int)
#define WFLAGS_DEFINE_int(name, description) \
    WFLAGS_DEFINE(int, name, description)

#define WFLAGS_DEFINE_DEFAULT_int(name, default_val, description) \
    WFLAGS_DEFINE_DEFAULT(int, name, default_val, description)

#define WFLAGS_DECLARE_int(name) \
    WFLAGS_DECLARE(int, name)

// the alias of fixed types (string)
#define WFLAGS_DEFINE_string(name, description) \
    WFLAGS_DEFINE(std::string, name, description)

#define WFLAGS_DEFINE_DEFAULT_string(name, default_val, description) \
    WFLAGS_DEFINE_DEFAULT(std::string, name, default_val, description)

#define WFLAGS_DECLARE_string(name) \
    WFLAGS_DECLARE(std::string, name)

// the alias of fixed types (double)
#define WFLAGS_DEFINE_double(name, description) \
    WFLAGS_DEFINE(double, name, description)

#define WFLAGS_DEFINE_DEFAULT_double(name, default_val, description) \
    WFLAGS_DEFINE_DEFAULT(double, name, default_val, description)

#define WFLAGS_DECLARE_double(name) \
    WFLAGS_DECLARE(double, name)

// the alias of fixed types (bool)
#define WFLAGS_DEFINE_bool(name, description) \
    WFLAGS_DEFINE(bool, name, description)

#define WFLAGS_DEFINE_DEFAULT_bool(name, default_val, description) \
    WFLAGS_DEFINE_DEFAULT(bool, name, default_val, description)

#define WFLAGS_DECLARE_bool(name) \
    WFLAGS_DECLARE(bool, name)

#endif /*WZPCPPLIB_CONFIG_WFLAGS_HPP_*/
