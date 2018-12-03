#ifndef WZP_TYPE_TRAIITS_HPP_
#define WZP_TYPE_TRAIITS_HPP_

#include <type_traits>

namespace wzp {

// get the meta max integer of a list of args
template<typename T, T arg, T... rest>
struct max_integer;

template<typename T, T arg>
struct max_integer<T, arg> : std::integral_constant<T, arg> {};

template<typename T, T arg1, T arg2, T... rest>
struct max_integer<T, arg1, arg2, rest...>
    : std::integral_constant<T, arg1 >= arg2 ? max_integer<T, arg1, rest...>::value :
    max_integer<T, arg2, rest...>::value> {};

// get the meta min integer of a list of args
template<typename T, T arg, T... rest>
struct min_integer;

template<typename T, T arg>
struct min_integer<T, arg> : std::integral_constant<T, arg> {};

template<typename T, T arg1, T arg2, T... rest>
struct min_integer<T, arg1, arg2, rest...>
    : std::integral_constant<T, arg1 <= arg2 ? min_integer<T, arg1, rest...>::value :
    min_integer<T, arg2, rest...>::value> {};

template<typename T, typename... Args>
struct max_alignment
    : std::integral_constant<int, (std::alignment_of<T>::value > max_alignment<Args...>::value)
    ? std::alignment_of<T>::value : max_alignment<Args...>::value> {};

template<typename T>
struct max_alignment<T> : std::integral_constant<int, std::alignment_of<T>::value> {};

// check if the rest contains the first type
template<typename T, typename... List>
struct contain_of : std::true_type {};

template<typename T, typename H, typename... Rest>
struct contain_of<T, H, Rest...>
    : std::conditional<std::is_same<T, H>::value, std::true_type, contain_of<T, Rest...>>::type {};

template<typename T>
struct contain_of<T> : std::false_type {};

// get types index by Type
namespace details {

// get the left size, only when type(T) == one of type(Types)
template<typename T, typename... Types>
struct get_left_size;

template<typename T, typename First, typename... Types>
struct get_left_size<T, First, Types...> : get_left_size<T, Types...> {};

template<typename T, typename... Types>
struct get_left_size<T, T, Types...> : std::integral_constant<int, sizeof...(Types)> {};

template<typename T>
struct get_left_size<T> : std::integral_constant<int, -1> {};

} //details

// get the index of types in Type list
template<typename T, typename... List>
struct index_of_types : std::integral_constant<int, sizeof...(List) - details::get_left_size<T, List...>::value - 1> {};

// get the type by index of a Type list
template<int I, typename... List>
struct index_to_type;

template<int I, typename First, typename... List>
struct index_to_type<I, First, List...> : index_to_type<I - 1, List...> {};

template<typename First, typename... List>
struct index_to_type<0, First, List...> { typedef First type; };

} //wzp

#endif /*WZP_TYPE_TRAIITS_HPP_*/