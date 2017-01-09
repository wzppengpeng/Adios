#ifndef LINQ_ENUMERABLE_HPP
#define LINQ_ENUMERABLE_HPP

#include <vector>
#include <list>
#include <unordered_set>
#include <unordered_map>
#include <stdexcept>
#include <memory>
#include <algorithm>

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
     * order by function
     * sort the range first, then return new LinqEnumrable
     */

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

    /**
     * group by function, group by a fcuntion
     * return pair<key, vector<element>>
     */
    template<typename Function>
    auto group_by(const Function& f)
    ->std::unordered_map<decltype(f(*(Element*)nullptr)), std::vector<Element>> {
        using Key = decltype(f(*(Element*)nullptr));
        using ValueVec = std::vector<Element>;
        std::unordered_map<Key, ValueVec> map;
        for(auto it = m_begin; it != m_end; ++it) {
            auto value = *it;
            auto key = f(value);
            auto it_f = map.find(key);
            if(it_f == map.end()) {
                ValueVec tmp {value};
                map.emplace(key, std::move(tmp));
            }
            else {
                it_f->second.emplace_back(value);
            }
        }
        return std::move(map);
    }

private:
    Iterator m_begin;
    Iterator m_end;
};

} //wzp

/**
 * easy the code
 */
#define Linq LinqEnumerable

#endif /*LINQ_ENUMERABLE_HPP*/