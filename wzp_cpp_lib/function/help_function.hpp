#ifndef RANGE_HPP_
#define RANGE_HPP_

#include <stdexcept>
#include <iostream>
#include <memory>
#include <algorithm>
#include <vector>
#include <string>

#include <initializer_list>


namespace wzp {

/**
 * How to use:
 * 1. range(begin, end, step) : use just like python range function, use for(auto)
 * 2. print(...) : print function in python style
 * 3. print_vector : print the std::vector
 * 4. any_min & any_max : any_min(a, b, c, d) or any_min(int a, double b)
 * 5. make_unique : generate std::uniquePtr
 * 6. str_release(STL container) : release the STL container by swap stategy
 * 7. reverse_walk(Range container) : to reverse walk for(auto)
 * 8. vector_ptr : to get the pointer of vector
 * 9. string_ptr : to get the char* of string
 */

namespace details {

/*
    the range is just an iterator, use c++11 for(auto i : range()) to iter
*/
template<class value_t>
class RangeImpl {
    class Iterator;
public:
    RangeImpl(value_t begin, value_t end, value_t step = 1): m_begin(begin), m_end(end), m_step(step)
    {
        if (step>0&&m_begin >= m_end)
            throw std::logic_error("end must greater than begin.");
        else if (step<0 && m_begin <= m_end)
            throw std::logic_error("end must less than begin.");

        m_step_end = (m_end - m_begin) / m_step;
        if(m_begin + m_step_end * m_step != m_end) ++m_step_end;
    }

    Iterator begin(){
        return Iterator(0, *this);
    }

    Iterator end(){
        return Iterator(m_step_end, *this);
    }

    value_t operator[](int s){
        return m_begin + s * m_step;
    }

    int size(){
        return m_step_end;
    }

private:
    value_t m_begin;
    value_t m_end;
    value_t m_step;
    int m_step_end;

private:
    class Iterator{
    public:
        // using difference_type = ptrdiff_t;
        /*constructor*/
        //the iterator point a value byu contain the reference of a RangeImpl's begin and step now
        Iterator(int start, RangeImpl& range) : m_current_step(start), m_range(range){
            m_current_value = m_range.m_begin + m_current_step * m_range.m_step;
        }

        /*the dereference*/
        value_t operator*(){ return m_current_value; }

        /*iterator self increasement*/
        const Iterator* operator++(){
            m_current_value += m_range.m_step;
            ++m_current_step;
            return this;
        }

        /*iterator judge the same*/
        bool operator==(const Iterator& other){
            return m_current_step == other.m_current_step;
        }

        /*iterator judge not the same*/
        bool operator!=(const Iterator& other){
            return m_current_step != other.m_current_step;
        }

        /*iterator self decreasmnet*/
        const Iterator* operator--(){
            m_current_value -= m_range.m_step;
            --m_current_step;
            return this;
        }

    private:
        value_t m_current_value;
        int m_current_step;
        RangeImpl& m_range;
    };

};

} //details

/*The range funcitons*/
//the begin end and step not the same type
template<typename T, typename V>
auto range(T begin, T end, V stepsize) -> details::RangeImpl<decltype(begin + end + stepsize)>{
    return details::RangeImpl<decltype(begin + end + stepsize)>(begin, end, stepsize);
}

template<typename T>
details::RangeImpl<T> range(T begin, T end){
    return details::RangeImpl<T>(begin, end, 1);
}

template<typename T>
details::RangeImpl<T> range(T end){
    return details::RangeImpl<T>(T(), end, 1);
}

/*print function like python*/
template<typename T>
void print(T&& t){
    std::cout << std::forward<T>(t) << std::endl;
}

//the any print function
template<typename T, typename... Args>
void print(T&& t, Args&&... args){
    std::cout << std::forward<T>(t) << ' ';
    print(std::forward<Args>(args)...);
}

/**
 * print vector
 */
template<typename T>
void print_vector(const std::vector<T>& v) {
    std::cout << '[';
    for(int i = 0; i < v.size() - 1; ++i) {
        std::cout << v[i] << ' ';
    }
    std::cout << v.back() << ']' << std::endl;
}

/**
 * [print_err description]
 * @param t any type which can use <<
 */
template<typename T>
void print_err(T&& t) {
    std::cerr << std::forward<T>(t) << std::endl;
}

template<typename T, typename... Args>
void print_err(T&& t, Args&&... args) {
    std::cerr << std::forward<T>(t) << ' ';
    print_err(std::forward<Args>(args)...);
}

/**
 * The min max function help for any args inputs
 */
namespace details {

template<typename T>
void min_max_help(bool is_min, T& res, T&& t) {
    if(is_min && t < res)
        res = t;
    else if(!is_min && t > res)
        res = t;
}

template<typename T, typename... Args>
void min_max_help(bool is_min, T& res, T&& t, Args&&... args) {
    min_max_help(is_min, res, std::forward<T>(t));
    min_max_help(is_min, res, std::forward<Args>(args)...);
}

} //details


// any types for two min
template<typename T, typename S> inline
auto any_min(T&& left, S&& right) -> typename std::decay<decltype(left < right ? left : right)>::type {
    return left < right ? left : right;
}

// a list to compute max
template<typename T, typename... Args>
typename std::decay<T>::type any_min(T&& first, Args&&... rest) {
    static_assert(sizeof...(Args) > 2, "the any_min function must have more than two input");
    using RT = typename std::decay<T>::type;
    RT res = first;
    details::min_max_help(true, res, std::forward<T>(first), std::forward<Args>(rest)...);
    return res;
}

// any types for two min
template<typename T, typename S> inline
auto any_max(T&& left, S&& right) -> typename std::decay<decltype(left > right ? left : right)>::type {
    return left > right ? left : right;
}

// a list to compute max
template<typename T, typename... Args>
typename std::decay<T>::type any_max(T&& first, Args&&... rest) {
    static_assert(sizeof...(Args) > 2, "the any_max function must have more than two input");
    using RT = typename std::decay<T>::type;
    RT res = first;
    details::min_max_help(false, res, std::forward<T>(first), std::forward<Args>(rest)...);
    return res;
}


//the function of make_unique, just like make_shared
template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

/**
 * the release function, use the swap method
 */
//release the stl container's memory
template<typename T>
inline void stl_release(T& stl_container) {
    using R = typename std::remove_reference<T>::type;
    R().swap(stl_container);
}


/**
 * the reverse walk container
 */
namespace reverse_details
{

template<typename Range>
class ReverseRangeWalker {
public:
    using Iter =  typename Range::reverse_iterator;


    ReverseRangeWalker(Range& r) : m_r(&r) {}

    inline Iter begin() { return m_r->rbegin(); }

    inline Iter end() { return m_r->rend(); }

private:
    Range* m_r;
    const Range* m_r_const;

};

template<typename Range>
class ReverseConstRangeWaler {
public:
    using CIter = typename Range::const_reverse_iterator;


    ReverseConstRangeWaler(const Range& r) : m_r_const(&r) {}

    inline CIter begin() const { return m_r_const->crbegin(); }

    inline CIter end() const { return m_r_const->crend(); }

private:
    const Range* m_r_const;
};

} //details

// teh functions wo reverse walk the container
template<typename Range>
inline reverse_details::ReverseRangeWalker<Range> reverse_walk(Range& r) {
    return reverse_details::ReverseRangeWalker<Range>(r);
}

template<typename Range>
inline reverse_details::ReverseConstRangeWaler<Range> reverse_walk(const Range& r) {
    return reverse_details::ReverseConstRangeWaler<Range>(r);
}

// the function to get the ptr of vector and string
template<typename T>
inline static T* vector_ptr(std::vector<T>& v, size_t idx = 0) {
    if(v.empty())
        return nullptr;
    return &v[idx];
}

template<typename T>
inline static const T* vector_ptr(const std::vector<T>& v, size_t idx = 0) {
    if(v.empty())
        return nullptr;
    return &v[idx];
}

inline static char* string_ptr(std::string& s, size_t idx = 0) {
    if(s.empty())
        return nullptr;
    return &s[idx];
}

inline static const char* string_ptr(const std::string& s, size_t idx = 0) {
    if(s.empty())
        return nullptr;
    return &s[idx];
}

// /////////////////

}//wzp

#endif /*RANGE_HPP_*/