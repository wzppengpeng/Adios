#ifndef LINQ_ENUMERABLE_HPP
#define LINQ_ENUMERABLE_HPP

#include <vector>
#include <list>
#include <unordered_set>
#include <unordered_map>
#include <stdexcept>

#include "linq/select_iterator.hpp"
#include "linq/where_iterator.hpp"
#include "linq/take_iterator.hpp"

namespace  wzp
{

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
    LinqEnumerable(Iterator begin_, Iterator end_) : m_begin(begin_), m_end(end_)
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
     * where function
     */
    template<typename Function>
    auto where(const Function& f)
    ->LinqEnumerable<WhereIterator<Iterator, Function>> {
        return LinqEnumerable<WhereIterator<Iterator, Function>>(
            WhereIterator<Iterator, Function>(m_begin, m_end, f),
            WhereIterator<Iterator, Function>(m_end, m_end, f)
        );
    }

    /**
     * take function
     */
    auto take(size_t c)
    ->LinqEnumerable<TakeIterator<Iterator>> {
        return LinqEnumerable<TakeIterator<Iterator>>(
            TakeIterator<Iterator>(m_begin, m_end, c),
            TakeIterator<Iterator>(m_end, m_end, c)
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
     * to list functions
     */
    std::list<Element> to_list() const {
        std::list<Element> l;
        for(auto it = m_begin; it != m_end; ++it) {
            l.emplace_back(*it);
        }
        return std::move(l);
    }

    /**
     * to set function
     */
    std::unordered_set<Element> to_set() const {
        std::unordered_set<Element> s;
        for(auto it = m_begin; it != m_end; ++it) {
            s.emplace(std::move(*it));
        }
        return std::move(s);
    }

    /**
     * the first element function
     */
    Element first() {
        if(empty())
            throw std::logic_error("the Enumarable is empty");
        return *m_begin;
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