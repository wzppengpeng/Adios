#ifndef WZP_CPP_LIB_FILES_FILE_SYSTEM_HPP_
#define WZP_CPP_LIB_FILES_FILE_SYSTEM_HPP_

#include <fstream>
#include <string>
#include <iterator>

namespace wzp
{

namespace file_system
{

using ll = long long;

// here define a input text stream iterator
template<typename IStream, typename String = std::string>
class TextIStreamIterator : public std::iterator<std::input_iterator_tag, String> {
public:
    TextIStreamIterator(String* buffer, IStream* istream, ll idx = 0, char split_delemeter = '\n')
        : buffer_(buffer), istream_(istream), idx_(idx), split_delemeter_(split_delemeter) {}

    // copy function just use default
    // the de-ref
    String& operator* () { return *buffer_; }
    const String& operator* () const { return *buffer_; }

    // == & !=
    bool operator != (const TextIStreamIterator& other) const {
        return !(istream_ == other.istream_ && idx_ == other.idx_);
    }

    bool operator == (const TextIStreamIterator& other) const {
        return istream_ == other.istream_ && idx_ == other.idx_;
    }

    // opeator ++ (no --)
    TextIStreamIterator& operator ++ () {
        if(std::getline(*istream_, *buffer_, split_delemeter_)) {
            ++idx_;
        } else {
            idx_ = -1;
        }
        return *this;
    }

private:
    String* buffer_; // a line data buffer
    IStream* istream_;
    ll idx_;
    char split_delemeter_;
};

// the txt file reader, just as a stl container
template<typename String>
class TextLineReader {

public:
    // the typedef of for range
    typedef TextIStreamIterator<std::ifstream, String> iterator;
    typedef String value_type;

public:
    TextLineReader(const String& file_name)
        : file_name_(file_name), is_open_(true) {
        Init();
    }

    // close the file handle
    ~TextLineReader() {
        UnInit();
    }

    // check if opened
    bool IsOpen() const {
        return is_open_;
    }

    inline void ReOpen() {
        Clear();
    }

    // seek to the fstream first
    void Clear() {
        UnInit();
        Init();
    }

    // begin and end for this range
    iterator begin() {
        return is_open_ ? iterator(&buffer_, &ifile_) : iterator(&buffer_, &ifile_, -1);
    }

    // the end
    iterator end() {
        return iterator(&buffer_, &ifile_, -1);
    }

private:
    void Init() {
        ifile_.open(file_name_, std::ios::in);
        if(!ifile_) {
            is_open_ = false;
        }
    }

    void UnInit() {
        if(is_open_)
            ifile_.close();
    }

private:
    // the stream
    String file_name_;
    // the ifstream
    std::ifstream ifile_;
    // the text line buffer
    String buffer_;
    // check if is open
    bool is_open_;
};


// the text line writer
enum class FileWriteMode {
    Write,
    Append
};
// write anything, write by line
template<typename String>
class TextLineWriter {

public:
    TextLineWriter(const String& file_name, FileWriteMode mode = FileWriteMode::Write) {
        Init(file_name, mode);
    }

    ~TextLineWriter() {
        if(ofile_.is_open()) {
            ofile_.close();
        }
    }

    // write any thing into a line
    template<typename T>
    void Write(T&& val) {
        ofile_ << std::forward<T>(val);
    }

    // recursive
    template<typename T, typename... Rest>
    void Write(T&& t, Rest&&... rest) {
        Write(std::forward<T>(t));
        Write(std::forward<Rest>(rest)...);
    }

    // the basic function to write line
    template<typename... Args>
    void WriteLine(Args&&... args) {
        Write(std::forward<Args>(args)...);
        Write('\n');
    }

    // the flush
    void Flush() {
        ofile_ << std::flush;
    }

    // write lines
    template<typename Range>
    void WriteLines(const Range& range) {
        for(auto it = std::begin(range); it != std::end(range); ++it) {
            WriteLine(*it);
        }
    }

private:
    void Init(const String& file_name, FileWriteMode mode) {
        if(mode == FileWriteMode::Write) {
            ofile_.open(file_name, std::ios::out);
        } else if (mode == FileWriteMode::Append) {
            ofile_.open(file_name, std::ios::app);
        }
    }

private:
    // the ofstream object
    std::ofstream ofile_;

};

} //file_system

} //wzp

#endif /*WZP_CPP_LIB_FILES_FILE_SYSTEM_HPP_*/