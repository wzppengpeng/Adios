#ifndef WZP_ANY_HPP_H_
#define WZP_ANY_HPP_H_

#include <string>
#include <memory>
#include <typeindex>

namespace wzp
{

/**
 * How to use:
 *  wzp::any a(std::vector<int>(10, 5));
    print_vector(a.cast<std::vector<int>>());
    a = 10;
    a = 0.01;
    print(a.cast<double>());
 */

class any {
public:
    // the exception for any cast
    struct BadAnyCast : std::bad_cast {
        BadAnyCast(const std::string& reason) :reason_(reason) {}
        BadAnyCast(std::string&& reason) :reason_(std::move(reason)) {}

        virtual const char* what() const noexcept {
            return reason_.c_str();
        }

        std::string reason_;
    };

public:
    // the any type constructor
    template<typename T> any(T&& val) : ptr_(new Derived<typename std::decay<T>::type>(std::forward<T>(val))),
        tp_index_(std::type_index(typeid(typename std::decay<T>::type))) {}
    //the default
    any(void) : tp_index_(std::type_index(typeid(void))) {}
    // copy constructor
    any(const any& other) : ptr_(other.clone()), tp_index_(other.tp_index_) {}
    // move constructor
    any(any&& other) : ptr_(std::move(other.ptr_)), tp_index_(other.tp_index_) {}

    // judge if is null
    bool is_null() const { return ptr_.get() == nullptr; }
    // judge if is type A
    template<typename U> bool is() const { return tp_index_ == std::type_index(typeid(U)); }
    // cast function, get the detail information
    template<typename T>
    const T& cast() const {
        if(!is<T>()) {
            throw BadAnyCast(std::string("can not cast ") + typeid(T).name() + " to " + tp_index_.name());
        }
        return dynamic_cast<Derived<T>* >(ptr_.get())->value_;
    }
    // the left value
    template<typename T>
    T& cast() {
        if(!is<T>()) {
            throw BadAnyCast(std::string("can not cast ") + typeid(T).name() + " to " + tp_index_.name());
        }
        return dynamic_cast<Derived<T>* >(ptr_.get())->value_;
    }

    // the operator = for copy
    any& operator= (const any& other) {
        if(this != &other) {
            ptr_ = other.clone();
            tp_index_ = other.tp_index_;
        }
        return *this;
    }
    // the operator = for move
    any& operator= (any&& other) {
        if(this != &other) {
            ptr_ = std::move(other.ptr_);
            tp_index_ = other.tp_index_;
        }
        return *this;
    }

private:
    struct Base;
    using BasePtr = std::unique_ptr<Base>;

    struct Base {
        virtual ~Base() {}
        // clone another value, set the function to be const
        virtual BasePtr clone() const = 0;
    }; //Base vritual class

    template<typename T>
    struct Derived : Base {
        // the constructor
        template<typename U>
        Derived(U&& u) : value_(std::forward<U>(u)) {}
        // override the clone function
        virtual BasePtr clone() const {
            return BasePtr(new Derived<T>(value_)); //set the memory dynamic
        }

        T value_;
    };

    // a clone interface for copy constructor
    BasePtr clone() const {
        if(ptr_.get()) {
            return ptr_->clone();
        }
        return nullptr;
    }

    BasePtr ptr_;
    std::type_index tp_index_;

}; //any class

} //wzp

#endif /*WZP_ANY_HPP_H_*/