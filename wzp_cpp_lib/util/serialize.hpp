#ifndef SERIALIZE_HPP_
#define SERIALIZE_HPP_

#include <string>
#include <vector>

using std::vector;
using std::string;

namespace wzp
{

template<typename T>
/**
 * [serialize one data into a string cache]
 * @param cache the point of string
 * @param t     the data need to be serialize
 */
void serialize(std::string* cache, T& t) {
    string temp;
    temp.resize(sizeof(T));
    auto work_ptr = reinterpret_cast<char*>(&t);
    for (int i = 0; i < sizeof(T); ++i)
    {
        temp[i] = work_ptr[i];
    }
    cache->append(std::move(temp));
}

template<typename Dtype>
/**
 * [serialize vector data]
 * @param cache the point of cache string
 * @param v     the Dtype vector
 */
void serialize(std::string* cache, vector<Dtype>& v) {
    string temp;
    temp.resize(v.size() * sizeof(Dtype));
    auto work_ptr = reinterpret_cast<char*>(&v[0]);
    for (int i = 0; i < temp.size(); ++i)
    {
        temp[i] = work_ptr[i];
    }
    cache->append(std::move(temp));
}

template<typename T, typename... Rest>
/**
 * [serialize description]
 * @param cache point of string
 * @param t     the one data
 * @param rest  the rest of template data
 */
void serialize(std::string* cache, T& t, Rest&... rest) {
    serialize<T>(cache, t);
    serialize(cache, rest...);
}

template<typename Dtype, typename... Rest>
/**
 * [serialize description]
 * @param cache point of string
 * @param v     vector of Dtype data
 * @param rest  the rest of template data
 */
void serialize(std::string* cache, vector<Dtype>& v, Rest&... rest) {
    serialize<Dtype>(cache, v);
    serialize(cache, rest...);
}

template<typename T>
/**
 * [deserialize deserialize a string into a data]
 * @param cache reference of string
 * @param t     the reference of a data need to be deserialize
 */
void deserialize(const std::string& cache, T& t) {
    auto work_ptr = reinterpret_cast<T*>(const_cast<char*>(cache.c_str()));
    t = *work_ptr;
}

template<typename Dtype>
/**
 * [deserialize description]
 * @param cache reference of string
 * @param v     vector of data referece
 */
void deserialize(const std::string& cache, vector<Dtype>& v) {
    auto work_ptr = reinterpret_cast<Dtype*>(const_cast<char*>(cache.c_str()));
    for (int i = 0; i < v.size(); ++i)
    {
        v[i] = work_ptr[i];
    }
}

template<typename T, typename... Rest>
/**
 * [deserialize description]
 * @param cache reference of string
 * @param t     the reference of normal type
 * @param rest  the rest of template data refence
 */
void deserialize(const std::string& cache, T& t, Rest&... rest) {
    deserialize<T>(cache, t);
    auto rest_str = cache.substr(sizeof(T));
    deserialize(rest_str, rest...);
}

template<typename Dtype, typename... Rest>
/**
 * [deserialize description]
 * @param cache the refence of string
 * @param t     the refence of normal type
 * @param rest  the rest of template data reference
 */
void deserialize(const std::string& cache, vector<Dtype>& t, Rest&... rest) {
    deserialize<Dtype>(cache, t);
    auto rest_str = cache.substr(sizeof(Dtype) * t.size());
    deserialize(rest_str, rest...);
}


} //wzp

#endif /*SERIALIZE_HPP_*/