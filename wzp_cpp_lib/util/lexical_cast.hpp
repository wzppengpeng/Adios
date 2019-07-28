#ifndef LEXICAL_CAST_HPP_
#define LEXICAL_CAST_HPP_

#include <type_traits>
#include <string>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include <stdexcept>
#include <cctype>
#include <cstring>

namespace wzp {

namespace detail
{

    template <typename To, typename From>
    struct Converter
    {
    };

    //to numeric
    template <typename From>
    struct Converter<int, From>
    {
        static int convert(const From& from)
        {
            return std::atoi(from);
        }
    };

    template <typename From>
    struct Converter<long, From>
    {
        static long convert(const From& from)
        {
            return std::atol(from);
        }
    };

    template <typename From>
    struct Converter<long long, From>
    {
        static long long convert(const From& from)
        {
            return std::atoll(from);
        }
    };

    template <typename From>
    struct Converter<double, From>
    {
        static double convert(const From& from)
        {
            return std::atof(from);
        }
    };

    template <typename From>
    struct Converter<float, From>
    {
        static float convert(const From& from)
        {
            return (float)std::atof(from);
        }
    };


    //for string
    template<>
    struct Converter<int, std::string>
    {
        inline static int convert(const std::string& from) {
            return std::stoi(from);
        }
    };

    template<>
    struct Converter<long, std::string>
    {
        inline static long convert(const std::string& from) {
            return std::stol(from);
        }
    };

    template<>
    struct Converter<long long, std::string>
    {
        inline static long long convert(const std::string& from) {
            return std::stoll(from);
        }
    };

    template<>
    struct Converter<double, std::string>
    {
        inline static double convert(const std::string& from) {
            return std::stod(from);
        }
    };

    template<>
    struct Converter<long double, std::string>
    {
        inline static long double convert(const std::string& from) {
            return std::stold(from);
        }
    };

    template<>
    struct Converter<float, std::string>
    {
        inline static float convert(const std::string& from) {
            return std::stof(from);
        }
    };

    template<>
    struct Converter<uint32_t, std::string>
    {
        inline static uint32_t convert(const std::string& from) {
            return std::stoul(from);
        }
    };

    template<>
    struct Converter<uint64_t, std::string>
    {
        inline static uint64_t convert(const std::string& from) {
            return std::stoull(from);
        }
    };

    template<>
    struct Converter<size_t, std::string>
    {
        inline static size_t convert(const std::string& from) {
            return std::stoul(from);
        }
    };

    //to bool
    template <typename From>
    struct Converter<bool, From>
    {
        static typename std::enable_if<std::is_integral<From>::value, bool>::type convert(From from)
        {
            return !!from;
        }
    };

    inline static bool checkbool(const char* from, const size_t len, const char* s)
    {
        for (size_t i = 0; i < len; i++)
        {
            if (from[i] != s[i])
            {
                return false;
            }
        }

        return true;
    }

    static bool bool_convert(const char* from)
    {
        const unsigned int len = strlen(from);
        if (len != 4 && len != 5)
            throw std::invalid_argument("argument is invalid");

        bool r = true;
        if (len == 4)
        {
            r = checkbool(from, len, "true");

            if (r)
                return true;
        }
        else
        {
            r = checkbool(from, len, "false");

            if (r)
                return false;
        }

        throw std::invalid_argument("argument is invalid");
    }

    template <>
    struct Converter<bool, std::string>
    {
        inline static bool convert(const std::string& from)
        {
            return bool_convert(from.c_str());
        }
    };

    template <>
    struct Converter<bool, const char*>
    {
        inline static bool convert(const char* from)
        {
            return bool_convert(from);
        }
    };

    template <>
    struct Converter<bool, char*>
    {
        inline static bool convert(char* from)
        {
            return bool_convert(from);
        }
    };

    template <unsigned N>
    struct Converter<bool, const char[N]>
    {
        inline static bool convert(const char(&from)[N])
        {
            return bool_convert(from);
        }
    };

    template <unsigned N>
    struct Converter<bool, char[N]>
    {
        inline static bool convert(const char(&from)[N])
        {
            return bool_convert(from);
        }
    };

    //to std::string
    template <typename From>
    struct Converter<std::string, From>
    {
        inline static std::string convert(const From& from)
        {
            return std::to_string(from);
        }
    };
} // detail

template <typename To, typename From>
typename std::enable_if<!std::is_same<To, From>::value, To>::type lexical_cast(const From& from)
{
    return detail::Converter<To, From>::convert(from);
}

template <typename To, typename From>
typename std::enable_if<std::is_same<To, From>::value, To>::type lexical_cast(const From& from)
{
    return from;
}
}//wzp

#endif /*LEXICAL_CAST_HPP_*/
