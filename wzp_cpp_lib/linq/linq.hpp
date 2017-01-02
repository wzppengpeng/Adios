#ifndef LINQ_HPP_
#define LINQ_HPP_

#include "linq/linq_enumerable.hpp"

namespace wzp
{

/**
 * change the STL container, to the LinqEnumerable type to store
 * the Iterator
 */
template<typename Container>
auto from(const Container& c)
 -> LinqEnumerable<decltype(std::begin(c))> {
    return LinqEnumerable<decltype(std::begin(c))>(std::begin(c), std::end(c));
}

/**
 * walk the iterators, and change the LinqEnumarable into a vector
 */
template<typename T, typename Iterator>
std::vector<T> walk_vector(const LinqEnumerable<Iterator>& linq_enumerable) {
    std::vector<T> res;
    for(auto q : linq_enumerable) {
        res.emplace_back(static_cast<T>(q));
    }
    return std::move(res);
}

} //wzp


#endif /*LINQ_HPP_*/