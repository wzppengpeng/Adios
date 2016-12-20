#ifndef MY_STRING_HPP_
#define MY_STRING_HPP_

#include <string>
#include <sstream>
#include <vector>

namespace wzp {

/*convert string to any type, this is too old, please use lexical_cast<string> instead*/
template<typename T>
inline static T convert_string(const std::string& input) {
    std::istringstream ss(input);
    T output;
    ss >> output;
    return output;
}

template<typename T>
inline static std::string convert_to_string(T&& t) {
    std::stringstream ss;
    ss<<t;
    std::string res;
    ss>>res;
    return res;
}


/*the specific one to split strings into vector<string>*/
inline static std::vector<std::string> split_string(const std::string& input, char split_delemeter) {
    std::vector<std::string> result;
    if (input.empty()) return std::move(result);
    std::istringstream ss(input);
    std::string cache;
    while (std::getline(ss, cache, split_delemeter)) {
        result.emplace_back(cache);
    }
    return std::move(result);
}

/*join a char into vector<string>, and get the string*/
inline static std::string join_string(const std::vector<std::string>& strs, char delimiter) {
    if (strs.size() <= 0) {
        return std::string("");
    }
    std::stringstream ss;
    ss << strs[0];
    for (size_t i = 1; i < strs.size(); ++i) {
        ss << delimiter;
        ss << strs[i];
    }
    return std::move(ss.str());
}

/*trim funciton*/
inline static bool trim(std::string& str) {
    if (str.empty()) return false;
    str.erase(str.find_last_not_of(" \f\t\n\r\v") + 1);
    str.erase(0, str.find_first_not_of(" \f\t\n\r\v"));
    return true;
}

/*the string format*/
template<typename T>
inline static std::string format(std::string&& str, T&& t) {
    auto begin = str.find_first_of('{');
    auto end = str.find_first_of('}');
    return std::move(str.substr(0, begin) + wzp::convert_to_string(std::forward<T>(t))
     + str.substr(end + 1));
}

// /*the string format with any params*/
template<typename T, typename... Args>
inline static std::string format(std::string&& str, T&& t, Args&&... args) {
    auto begin = str.find_first_of('{');
    auto end = str.find_first_of('}');
    return std::move(format(std::move(str.substr(0, begin) + wzp::convert_to_string(std::forward<T>(t))
     + str.substr(end + 1)), std::forward<Args>(args)...));
}

}

#endif