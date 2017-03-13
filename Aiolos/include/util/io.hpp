#ifndef AIOLOS_IO_HPP_
#define AIOLOS_IO_HPP_

#include <unordered_map>

#include "util/serialize.hpp"


namespace aiolos
{

namespace io
{

/**
 * the basic function to write a string into a file
 */
inline void write_buffer(const std::string& buffer, const char* filename) {
    std::ofstream ofile(filename, std::ios::binary);
    ofile.write(buffer.c_str(), sizeof(char) * buffer.size());
    ofile.close();
}

/**
 * the basic function to read a file into a string buffer
 */
inline std::string read_buffer(const char* filename) {
    std::ifstream ifile;
    ifile.open(filename);
    std::string buffer((std::istreambuf_iterator<char>(ifile)),
        std::istreambuf_iterator<char>());
    ifile.close();
    return std::move(buffer);
}

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


/**
 * need to append matrix to a string
 */
template<typename Mat>
inline void append_matrix(Mat& mat, std::string* buffer) {
    mat.matrix_to_string(buffer);
}

/**
 * need to read matrix from string buffer
 */
template<typename T, typename Mat>
inline void read_matrix(Mat& mat, std::string& buffer) {
    mat.string_to_matrix(buffer);
    //cut the string
    buffer = std::move(buffer.substr(mat.rows() * mat.cols() * sizeof(T)));
}

} //io
} //aiolos

#endif /*AIOLOS_IO_HPP_*/