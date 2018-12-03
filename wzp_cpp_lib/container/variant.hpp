#ifndef WZP_CONTAINER_VARIANT_HPP_
#define WZP_CONTAINER_VARIANT_HPP_

#include <typeindex>
#include <tuple>

#include "wzp_cpp_lib/function/type_traits.hpp"

/**
 * the variants class just like union in C
 * The following is normal using
 * typedef variant<int, double, string, std::vector<int>> cv;
    std::vector<int> v(10, 5);
    cv v1 = v; // copy construct of other types
    std::iota(std::begin(v1.get<vector<int>>()), std::end(v1.get<vector<int>>()), 0); // get reference
    v1 = string("wzp"); // move operator
    v1 = 0.01;
    v1 = string("wzp");
    v1 = cv(0.01); // move operator with move construct
 *
 *
 */

namespace wzp {

namespace details {

/**
 * The placement new help class to construct or destroy the object
 */
template<typename... Args>
struct variant_handler;

template<typename T, typename... Args>
struct variant_handler<T, Args...> {
    inline static void destroy(std::type_index id, void* data) {
        if(id == std::type_index(typeid(T))) {
            reinterpret_cast<T*>(data)->~T();
        } else {
            variant_handler<Args...>::destroy(id, data);
        }
    }

    inline static void move(std::type_index id, void* old_v, void* new_v) {
        if(id == std::type_index(typeid(T))) {
            new (new_v) T(std::move(*reinterpret_cast<T*>(old_v)));
        } else {
            variant_handler<Args...>::move(id, old_v, new_v);
        }
    }

    inline static void copy(std::type_index id, void* old_v, void* new_v) {
        if(id == std::type_index(typeid(T))) {
            new (new_v) T(*reinterpret_cast<T*>(old_v));
        } else {
            variant_handler<Args...>::move(id, old_v, new_v);
        }
    }
};

template<> struct variant_handler<> {
    inline static void destroy(std::type_index id, void* data) {}
    inline static void move(std::type_index id, void* old_v, void* new_v) {}
    inline static void copy(std::type_index id, void* old_v, void* new_v) {}
};

} //details

// the class of variant
template<typename... Types>
class variant {
public:
    // some traits declarations
    using handler_t = details::variant_handler<Types...>;
    // two static value of Types
    // the max size and the max align size
    enum {
        data_size = wzp::max_integer<size_t, sizeof(Types)...>::value,
        align_size = wzp::max_alignment<Types...>::value
    };
    using store_type = typename std::aligned_storage<data_size, align_size>::type;

public:
    // the declaration to get the type by index
    template<int I>
    using index_type = typename wzp::index_to_type<I, Types...>::type;

    // the default construct
    variant(void) : type_index_(typeid(void)), index_(-1) {}

    // destroy the object
    ~variant() {
        handler_t::destroy(type_index_, &data_);
    }

    // move construction
    variant(variant<Types...>&& other) : type_index_(other.type_index_), index_(other.index_) {
        handler_t::move(other.type_index_, &other.data_, &data_);
    }

    variant<Types...>& operator= (variant<Types...>&& other) {
        handler_t::destroy(type_index_, &data_);
        type_index_ = other.type_index_;
        index_ = other.index_;
        handler_t::move(type_index_, &other.data_, &data_);
        return *this;
    }

    // copy construction
    variant(const variant<Types...>& other) : type_index_(other.type_index_), index_(other.index_) {
        handler_t::copy(other.type_index_, &other.data_, &data_);
    }

    variant<Types...>& operator= (const variant<Types...>& other) {
        handler_t::destroy(type_index_, &data_);
        type_index_ = other.type_index_;
        index_ = other.index_;
        handler_t::copy(type_index_, const_cast<store_type*>(&other.data_), &data_);
        return *this;
    }

    template<class T,
    class = typename std::enable_if<wzp::contain_of<typename std::remove_reference<T>::type, Types...>::value>::type>
    variant(T&& value) : type_index_(typeid(void)), index_(-1) {
        handler_t::destroy(type_index_, &data_);
        using NowType = typename std::remove_reference<T>::type;
        new (&data_) NowType(std::forward<T>(value)); //placement new
        type_index_ = std::type_index(typeid(T));
        index_ = wzp::index_of_types<NowType, Types...>::value;
    }

    // judge if the data is type T
    template<typename T>
    inline bool is() const { return type_index_ == std::type_index(typeid(T)); }

    // judge if it has been placement
    inline bool empty() const { return type_index_ == std::type_index(typeid(void)); }

    // return the type index of now
    inline std::type_index type() const { return type_index_; }

    // the most important get function
    template<typename T>
    T& get() {
        static_assert(wzp::contain_of<T, Types...>::value, "the variant must contain type");
        if(!is<T>()) {
            throw std::bad_cast();
        }
        return *(T*) (&data_);
    }

private:
    // the data memory
    store_type data_;
    std::type_index type_index_; // the type index of now
    int index_; // the index of type in the Type list
};

} //wzp

#endif /*WZP_CONTAINER_VARIANT_HPP_*/