#ifndef TRAITS_HPP_
#define TRAITS_HPP_

namespace wzp
{

/**
 * Some Traits
 */
template<typename Iterator>
using IteratorType = decltype(**(Iterator*)nullptr);

}


#endif