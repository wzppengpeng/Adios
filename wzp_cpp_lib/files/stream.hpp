#ifndef WZP_CPP_LIB_FILES_STREAM_HPP_
#define WZP_CPP_LIB_FILES_STREAM_HPP_

/**
 * 输入输出流，提供简单统一的Read和Write接口供下游使用
 * 主要是文件流、字符串流，接口与string_buffer保持一致
 */

#include <string>
#include <fstream>
#include <sstream>

#include "wzp_cpp_lib/uncopyable.hpp"

namespace wzp
{

namespace io
{

// the virtual base function
class BasicStream : Uncopyable {
public:
    // only a virtual function of template
    template<typename T>
    bool Read(T* val);

    // only a virtual function to match some template
    template<typename T>
    void Write(const T& t);

};

class TextFileIStream : public BasicStream {
// Only Has The Read Interface
public:
    // initialize the stream
    TextFileIStream(const std::string& file) {
        ifile_.open(file, std::ios::in);
    }

    ~TextFileIStream() {
        Close();
    }

    void Close() {
        if (ifile_.is_open()) {
            ifile_.close();
        }
    }

    template<typename T>
    bool Read(T* val) {
        if (!ifile_.is_open())
            return false;
        return ifile_ >> (*val);
    }

    void Seek(std::streampos pos) {
        ifile_.seekg(pos);
    }

private:
    std::ifstream ifile_; // the file stream of stl
};


class TextFileOStream : public BasicStream {
// only has the write interface to write something into stream
public:
    TextFileOStream(const std::string& file) {
        ofile_.open(file, std::ios::out);
    }

    ~TextFileOStream() {
        Close();
    }

    void Close() {
        if (ofile_.is_open()) {
            ofile_.close();
        }
    }

    template<typename T>
    void Write(const T& t) {
        if (!ofile_.is_open())
            return;
        ofile_ << t << '\n';
    }

    std::streampos Tell() {
        return ofile_.tellp();
    }

private:
    std::ofstream ofile_; // the file stream of stl
};


/**
 * The binary part of stream
 */
class BinaryIStream : public BasicStream {
// only has the read interface to read, and the seek function
public:
    BinaryIStream(const std::string& file) {
        ifile_.open(file, std::ios::binary);
    }

     ~BinaryIStream() {
        Close();
    }

    void Close() {
        if (ifile_.is_open()) {
            ifile_.close();
        }
    }

    template<typename T>
    bool Read(T* val, typename std::enable_if<std::is_pod<T>::value, T*>::type t_ptr = nullptr) {
        if (!ifile_.is_open())
            return false;
        ifile_.read((char*)val, sizeof(T));
        return true;
    }

    void Seek(std::streampos pos) {
        ifile_.seekg(pos);
    }

private:
    std::ifstream ifile_;

};

class BinaryOStream : public BasicStream {
// only has the write interface to write something into stream, and tell the position
public:
    BinaryOStream(const std::string& file) {
        ofile_.open(file, std::ios::binary);
    }

    ~BinaryOStream() {
        Close();
    }

    void Close() {
        if (ofile_.is_open()) {
            ofile_.close();
        }
    }

    template<typename T>
    void Write(const T& t, typename std::enable_if<std::is_pod<T>::value, T*>::type t_ptr = nullptr) {
        if (!ofile_.is_open())
            return;
        ofile_.write((char*)(&t), sizeof(T));
    }

    std::streampos Tell() {
        return ofile_.tellp();
    }

private:
    std::ofstream ofile_; // the file stream of stl
};

} // io

} // wzp

#endif /*WZP_CPP_LIB_FILES_STREAM_HPP_*/