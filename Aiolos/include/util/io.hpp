#ifndef AIOLOS_IO_HPP_
#define AIOLOS_IO_HPP_

#include <unordered_map>

#include "util/serialize.hpp"


namespace aiolos
{

namespace io
{

/**
 * seraize a map, but only the memory continuious type
 */
template<typename K, typename V>
std::string unordered_map_to_string(std::unordered_map<K, V>& m) {
    //first serialize the map size
    std::string buffer;
    size_t length = m.size();
    wzp::serialize(&buffer, length);
    for(auto& p : m) {
        wzp::serialize(&buffer, const_cast<K&>(p.first), const_cast<V&>(p.second));
    }
    return std::move(buffer);
}

/**
 * wzp::serialize a map
 */
template<typename K, typename V>
std::unordered_map<K, V> string_to_unordered_map(std::string& buffer) {
    size_t length;
    wzp::deserialize(buffer, length);
    buffer = std::move(buffer.substr(sizeof(size_t)));
    std::unordered_map<K, V> res;
    K key;
    V value;
    for(size_t i = 0; i < length; ++i) {
        wzp::deserialize(buffer, key, value);
        //insert into map
        res.emplace(key, value);
        buffer = std::move(buffer.substr(sizeof(K) + sizeof(V)));
    }
    return std::move(res);
}


} //io
} //aiolos

#endif /*AIOLOS_IO_HPP_*/