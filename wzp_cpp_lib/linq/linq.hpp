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


} //wzp


#endif /*LINQ_HPP_*/