#ifndef WZP_CONTAINER_OPTIONAL_HPP_
#define WZP_CONTAINER_OPTIONAL_HPP_

#include <stdexcept>

namespace wzp {

/**
 * the optional in C++14, implement by C++11
 * The basic use as follows:
 *  optional<int> op;
    if(op)
        cout<<*op<< "" << endl;
    else
        print("not");
    optional<int> op1 = 1;
    if(op1)
        cout<<*op1<<endl;
 *  other uses:
 *  optional<string> a("ok");
    optional<string> b("ok");
    optional<string> c("aa");

    print(bool(a));
    c = a;
    if (c < a)
        cout << "< " << endl;

    if (a == b)
        cout << " = " << endl;

    map<optional<string>, int> mymap; // based on operator <
    mymap.insert(std::make_pair(a, 1));
    mymap.insert(std::make_pair(c, 2));

    auto it = mymap.find(a);
    cout << it->second << endl;
 */


template<typename T>
class optional {
    using store_type = typename std::aligned_storage<sizeof(T), std::alignment_of<T>::value>::type;

public:
    // constructors
    optional() = default;

    // constructor by left
    optional(const T& val) : has_init_(false) {
        construct(val);
    }

    // construct by right
    optional(T&& val) : has_init_(false) {
        construct(std::move(val));
    }

    ~optional() {
        destroy();
    }

    // copy constructor
    optional(const optional& other) : has_init_(false) {
        assign(other);
    }

    // move constructor
    optional(optional&& other) {
        assign(std::move(other));
    }

    // operator =
    optional& operator= (const optional& other) {
        assign(other);
        return *this;
    }

    optional& operator= (optional&& other) {
        assign(std::move(other));
        return *this;
    }

    // the emplace functions
    template<typename... Args>
    void emplace(Args&&... args) {
        destroy();
        construct(std::forward<Args>(args)...);
    }

    // some getter functions
    inline bool is_init() const { return has_init_; }
    explicit inline operator bool() const { return is_init(); }

    // to get the reference of the object
    inline T& operator*() {
        return *((T*) (&data_));
    }

    const T& operator*() const {
        if (is_init())
            return *((T*) (&data_));
        else
            throw std::runtime_error("this optional has not been initialized");
    }

    bool operator == (const optional<T>& rhs) const {
        return (!bool(*this)) != (!rhs) ? false : (!bool(*this) ? true : (*(*this)) == (*rhs));
    }

    bool operator < (const optional<T>& rhs) const {
        return !rhs ? false : (!bool(*this) ? true : (*(*this) < (*rhs)));
    }

    inline bool operator != (const optional<T>& rhs) {
        return !(*this == (rhs));
    }

private:
    // private functions
    // create by a list of args
    template<typename... Args>
    void construct(Args&&... args) {
        new (&data_) T(std::forward<Args>(args)...);
        has_init_ = true;
    }

    // destroy the type
    void destroy() {
        if(has_init_) {
            reinterpret_cast<T*>(&data_)->~T();
            has_init_ = false;
        }
    }

    // the operator assign functions
    void assign(const optional& other) {
        if(other.is_init()) {
            copy_from_store(other.data_);
            has_init_ = true;
        } else {
            destroy();
        }
    }

    // assign from right
    void assign(optional&& other) {
        if(other.is_init()) {
            move_from_store(std::move(other.data_));
            has_init_ = true;
            other.destroy();
        } else {
            destroy();
        }
    }

    // two help functions for construct
    void move_from_store(store_type&& other) {
        destroy();
        new (&data_) T(std::move(*reinterpret_cast<T*>(&other)));
    }

    void copy_from_store(const store_type& other) {
        destroy();
        new (&data_) T(*reinterpret_cast<const T*>(&other));
    }

private:
    // members
    bool has_init_ = false;
    store_type data_;
};

} //wzp

#endif /*WZP_CONTAINER_OPTIONAL_HPP_*/