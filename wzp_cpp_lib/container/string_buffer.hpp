#ifndef WZP_CPP_LIB_CONTAINER_STRING_BUFFER_HPP_
#define WZP_CPP_LIB_CONTAINER_STRING_BUFFER_HPP_

#include <string>
#include <type_traits>

#include "wzp_cpp_lib/uncopyable.hpp"

/**
 * A string buffer to write and read for serialize
 */

namespace wzp
{

class StringBuffer : Uncopyable {

public:
    StringBuffer() = default;

    StringBuffer(const std::string& str) {
        FromString(str);
    }

    StringBuffer(std::string&& str) noexcept {
        FromString(std::move(str));
    }

    void FromString(const std::string& str) {
        buffer_ = str;
    }

    void FromString(std::string&& str) noexcept {
        buffer_ = std::move(str);
    }

    inline size_t Size() const { return buffer_.size(); }

    inline const std::string& GetString() const { return buffer_; }

    std::string ToString() const { return buffer_; }

    void Clear() {
        buffer_.clear();
        offset_ = 0;
    }

    void Destroy() {
        Clear();
        std::string().swap(buffer_);
    }

    void Write(const void* ptr, size_t len) {
        const char* loc = static_cast<const char*>(ptr);
        std::string tmp(loc, loc + len);
        buffer_.append(std::move(tmp));
    }

    bool Read(void* ptr, size_t len) {
        if(offset_ >= buffer_.size())
            return false;
        char* loc = &buffer_[offset_];
        char* ptr_copy = static_cast<char*>(ptr);
        for(size_t i = 0; i < len; ++i) {
            ptr_copy[i] = loc[i];
        }
        offset_ += len;
        return true;
    }

    template<typename T>
    void Write(const T& t, typename std::enable_if<std::is_pod<T>::value, T*>::type t_ptr = nullptr) {
        Write(&t, sizeof(T));
    }

    template<typename T>
    bool Read(T* t, typename std::enable_if<std::is_pod<T>::value, T*>::type t_ptr = nullptr) {
        return Read((void*)t, sizeof(T));
    }

private:
    std::string buffer_ = {};
    size_t offset_ = 0;
};

} //wzp


#endif /*WZP_CPP_LIB_CONTAINER_STRING_BUFFER_HPP_*/