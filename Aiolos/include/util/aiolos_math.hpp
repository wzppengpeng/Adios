#ifndef AIOLOS_MATH_HPP
#define AIOLOS_MATH_HPP

#include <cmath>

#include "common.hpp"

namespace aiolos
{

namespace math
{

/**
 * log by any down value
 * @param  val  the value
 * @param  down down value
 * @return      log(val)
 */
inline Type aiolos_log(Type val, int down) {
    return log(val) / log(down);
}

} //math

}//aiolos


#endif /*AIOLOS_MATH_HPP*/