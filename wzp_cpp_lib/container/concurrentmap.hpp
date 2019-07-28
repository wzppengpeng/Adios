#ifndef WZP_CPP_LIB_CONTAINER_CONCURRENT_MAP_HPP_
#define WZP_CPP_LIB_CONTAINER_CONCURRENT_MAP_HPP_

#include <unordered_map>
#include <vector>
#include <mutex>

#include "wzp_cpp_lib/container/optional.hpp"

namespace wzp
{

// the basic type with a big lock
template<typename Key, typename Val, typename Container = std::unordered_map<Key, Val>>
class BigLockMap {

private:
    std::mutex m_mut;
    Container m_map;

public:
    BigLockMap() = default;

    // the insert interface
    void Insert(const Key& key, const Val& val) {
        std::lock_guard<std::mutex> lock(m_mut);
        m_map.emplace(key, val);
    }

    // the get option
    optional<Val> Get(const Key& key) {
        std::lock_guard<std::mutex> lock(m_mut);
        auto it = m_map.find(key);
        if(it != m_map.end()) {
            return it->second;
        }
        return {};
    }

    // the count option
    size_t Count(const Key& key) {
        std::lock_guard<std::mutex> lock(m_mut);
        return m_map.count(key);
    }

    // erase from map, return if erase success
    bool Erase(const Key& key) {
        std::lock_guard<std::mutex> lock(m_mut);
        auto n = m_map.erase(key);
        return n;
    }

};


// the segment map -> concurrent map
template<typename Key, typename Val, typename Container = std::unordered_map<Key, Val>>
class ConcurrentMap {
private:
    const size_t m_mask;
    using InnerMap = BigLockMap<Key, Val, Container>;
    std::vector<InnerMap> m_shared_map;

    // the getter ref of shared map
    InnerMap& GetShared(const Key& key) {
        auto h = std::hash<Key>()(key);
        return m_shared_map[h & m_mask];
    }

public:
    ConcurrentMap(size_t num_shared = 32) : m_mask(num_shared - 1), m_shared_map(num_shared) {
        if((num_shared & m_mask) != 0)
            throw std::runtime_error("num shared param must be a power of 2");
    }

    // insert value
    void Insert(const Key& key, const Val& val) {
        GetShared(key).Insert(key, val);
    }

    // the getter interface
    optional<Val> Get(const Key& key) {
        return GetShared(key).Get(key);
    }

    // the count interface
    size_t Count(const Key& key) {
        return GetShared(key).Count(key);
    }

    // the erase interface
    bool Erase(const Key& key) {
        return GetShared(key).Erase(key);
    }

};

} //wzp

#endif /*WZP_CPP_LIB_CONTAINER_CONCURRENT_MAP_HPP_*/