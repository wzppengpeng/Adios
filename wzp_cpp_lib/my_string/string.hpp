#ifndef MY_STRING_HPP_
#define MY_STRING_HPP_

#include <cstdio>

#include <string>
#include <sstream>
#include <vector>
#include <memory>
#include <type_traits>

/**
 * How To Use :
 * 1. convert_string: convert string to other types, can use the lexical_cast<T>(str), instead
 * 2. convert_to_string: convert other types to string, depend on different types use the most effient functions
 * 3. split_string: split a line string by delemeter into a list of small strings
 * 4. transform: transform a list of strings into a list of other types
 * 5. split_string_transform: combines above two
 * 6. join_string: join a list of data into a line of string
 * 7. trim: trim a string to remove the space
 * 8. format: format the string, to use it just like python: "{} likes {}"
 * 9. start_with: check s if starts with p
 * 10. end_with: check s if ends with p
 * 11. string_format: a c style-format function (%d / %f / %u ...)
 * 12. string_concat: concat different part into a single string (each part can not be string)
 */

namespace wzp {

/*convert string to any type, this is too old, please use lexical_cast<T> instead*/
template<typename T>
inline typename std::enable_if<!std::is_same<T, std::string>::value, T>::type convert_string(const std::string& input) {
    std::istringstream ss(input);
    T output;
    ss >> output;
    return output;
}

template<typename T>
inline typename std::enable_if<std::is_same<T, std::string>::value, std::string>::type convert_string(const std::string& input) {
    return input;
}

// convert string to bool
inline static bool convert_string_to_bool(const std::string& input) {
    std::istringstream is(input);
    bool b;
    is >> std::boolalpha >> b;
    return b;
}

// convert string to bool
template<>
inline bool convert_string<bool>(const std::string& input) {
    return convert_string_to_bool(input);
}


/**
 * This part defines the convert_to_string meta functions
 * convert common types into std::string by different functions
 */
namespace details {

template<typename T>
struct is_interger_or_float {
    const static bool value = std::is_integral<T>::value || std::is_floating_point<T>::value;
};

} //details

template<typename T>
inline typename std::enable_if<details::is_interger_or_float<T>::value, std::string>::type convert_to_string(const T& t) {
    return std::to_string(t);
}

template<typename T>
inline typename std::enable_if<!details::is_interger_or_float<T>::value, std::string>::type convert_to_string(const T& t) {
    return std::string(t);
}

template<>
inline std::string convert_to_string<bool>(const bool& t) {
    return t ? "true" : "false";
}

/* convert to string over */

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

/**
 * transform vector<string> to other type data
 */
template<typename T>
inline static std::vector<T> transform(const std::vector<std::string>& strs) {
    std::vector<T> datas(strs.size());
    for(size_t i = 0; i < strs.size(); ++i) {
        datas[i] = convert_string<T>(strs[i]);
    }
    return std::move(datas);
}

/**
 * the combine type
 */
template<typename T>
inline static std::vector<T> split_string_transform(const std::string& input, char split_delemeter) {
    std::vector<T> result;
    if (input.empty()) return std::move(result);
    std::istringstream ss(input);
    std::string cache;
    while (std::getline(ss, cache, split_delemeter)) {
        result.emplace_back(convert_string<T>(cache));
    }
    return std::move(result);
}

/*join a char into vector<string>, and get the string*/

/**
 * join a char into a vector<T>, get the string
 */
template<typename T>
inline static std::string join_string(const std::vector<T>& datas, char delimiter) {
    if(datas.empty()) return "";
    std::stringstream ss;
    ss << datas[0];
    for (size_t i = 1; i < datas.size(); ++i) {
        ss << delimiter;
        ss << datas[i];
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
    auto end = str.find_first_of('}', begin + 1);
    return std::move(str.substr(0, begin) + wzp::convert_to_string(std::forward<T>(t))
     + str.substr(end + 1));
}

// /*the string format with any params*/
template<typename T, typename... Args>
inline static std::string format(std::string&& str, T&& t, Args&&... args) {
    auto begin = str.find_first_of('{');
    auto end = str.find_first_of('}', begin + 1);
    return std::move(format(std::move(str.substr(0, begin) + wzp::convert_to_string(std::forward<T>(t))
     + str.substr(end + 1)), std::forward<Args>(args)...));
}


/** format the string use c style */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-nonliteral"
template<typename... Args>
inline static std::string string_format(const std::string& format, Args... args) {
    size_t size = snprintf(nullptr, 0, format.c_str(), args... ) + 1; // Extra space for '\0'
    std::unique_ptr<char[]> buffer(new char[size]);
    snprintf(buffer.get(), size, format.c_str(), args...);
    return std::string(buffer.get(), buffer.get() + (size - 1));
}
#pragma GCC diagnostic pop


template<typename... Args>
inline static std::string c_string_format(const char* format, Args... args) {
    size_t size = snprintf(nullptr, 0, format, args... ) + 1; // Extra space for '\0'
    std::unique_ptr<char[]> buffer(new char[size]);
    snprintf(buffer.get(), size, format, args...);
    return std::string(buffer.get(), buffer.get() + (size - 1));
}

namespace concat_details
{

template<typename A>
void string_concat_with_stream(std::ostringstream& ss, A&& a) {
    ss << std::forward<A>(a);
}

template<typename A, typename... Args>
void string_concat_with_stream(std::ostringstream& ss, A&& a, Args&&... args) {
    string_concat_with_stream(ss, std::forward<A>(a));
    string_concat_with_stream(ss, std::forward<Args>(args)...);
}

} //concat_details


/** concat different part string into one string **/
template<typename... Args>
inline static std::string string_concat(Args&&... args) {
    std::ostringstream ss;
    concat_details::string_concat_with_stream(ss, std::forward<Args>(args)...);
    return ss.str();
}


/**
 * the startwith function
 */
inline static bool start_with(const std::string& s, const std::string& p) {
    if(p.size() > s.size()) return false;
    for(size_t i = 0; i < p.size(); ++i) {
        if(p[i] != s[i]) return false;
    }
    return true;
}

/**
 * the end with function
 */
inline static bool end_with(const std::string& s, const std::string& p) {
    if(p.size() > s.size()) return false;
    auto len_s = s.size();
    auto len_j = p.size();
    for(size_t j = 0; j < len_j; ++j) {
        if(p[len_j - 1 - j] != s[len_s - 1 - j]) return false;
    }
    return true;
}

}

#endif