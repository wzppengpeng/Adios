#ifndef APPLY_TUPLE_HPP_
#define APPLY_TUPLE_HPP_


#include <tuple>


namespace wzp
{

namespace detail
{

template<int I, typename T, typename... Args>
struct FindIndex
{
    static int call(const std::tuple<Args...>& t, T&& val) {
        return (std::get<I - 1>(t) == val) ? (I - 1) :
            FindIndex<I - 1, T, Args...>::call(t, std::forward<T>(val));
    }
};

// the special one
template<typename T, typename... Args>
struct FindIndex<0, T, Args...>
{
    static int call(const std::tuple<Args...>& t, T&& val) {
        return (std::get<0>(t) == val) ? 0 : -1;
    }
};

} //detail

// the wrapper
// the wraper of function to call
template<typename T, typename... Args>
int find_index(const std::tuple<Args...>& t, T&& val) {
    return detail::FindIndex<sizeof...(Args), T, Args...>::call(t, std::forward<T>(val));
}

} //wzp


#endif /*APPLY_TUPLE_HPP_*/