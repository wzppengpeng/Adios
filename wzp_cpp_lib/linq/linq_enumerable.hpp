#ifndef LINQ_ENUMERABLE_HPP
#define LINQ_ENUMERABLE_HPP

#include <vector>
#include <list>
#include <unordered_set>
#include <unordered_map>
#include <stdexcept>

#include "linq/select_iterator.hpp"

namespace  wzp
{
/**
 * Some Traits
 */
template<typename Iterator>
using IteratorType = decltype(**(Iterator*)nullptr);

/**
 * the Class LinqEnumerable is a pair of Iterator of STL Container
 */
template<typename Iterator>
class LinqEnumerable {
//the Iterator point data type
using Element = typename std::remove_cv<typename std::remove_reference<IteratorType<Iterator>>::type>::type;

public:
    /**
     * the constructor
     */
    LinqEnumerable(const Iterator& begin_, const Iterator& end_) : m_begin(begin_), m_end(end_)
    {}

    /**
     * getter functions
     */

    /**
     * begin function to get the m_begin
     * @return begin Iterator
     */
    Iterator begin() const { return m_begin; }

    /**
     * [end function to get the end Iterator]
     * @return end iterator
     */
    Iterator end() const { return m_end; }

    /**
     * empty() function
     */
    bool empty() const {
        return m_begin == m_end;
    }

    /**
     * &************
     * the lazy functions
     * ***********
     */
    /**
     * select function
     */
    template<typename Function>
    auto select(const Function& f)
    ->LinqEnumerable<SelectIterator<Iterator, Function>> {
        return LinqEnumerable<SelectIterator<Iterator, Function>>(
            SelectIterator<Iterator, Function>(m_begin, f),
            SelectIterator<Iterator, Function>(m_end, f)
        );
    }

    /**
     * ************
     * some imeditaly functions
     * ************
     */
    /**
     * to vector functions
     */
    std::vector<Element> to_vector() const {
        std::vector<Element> v;
        for(auto it = m_begin; it != m_end; ++it) {
            v.emplace_back(*it);
        }
        return std::move(v);
    }

    /**
     * the first element function
     */
    Element first() const {
        if(empty())
            throw std::logic_error("the Enumarable is empty");
        auto it = m_begin;
        return *it;
    }

    /**
     * count() function, to get the iterator distance
     */
    int count() const {
        int cnt = 0;
        for(auto it = m_begin; it != m_end; ++it) {
            ++cnt;
        }
        return cnt;
    }

    /**
     * to_list, to_map, to_set functions
     */

private:
    Iterator m_begin;
    Iterator m_end;
};

} //wzp

#endif /*LINQ_ENUMERABLE_HPP*/