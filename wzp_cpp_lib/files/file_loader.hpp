#ifndef FILE_LOADER_HPP_
#define FILE_LOADER_HPP_


#include <fstream>
#include <string>
#include <vector>
#include <functional>

namespace wzp
{

template<typename R>
class FileLoader
{

public:
    FileLoader() : m_can_iter(true) {}

    //the regis function, to regis a function object to get process a line
    template<typename Fun>
    void regis(Fun f, const std::string& filename) {
        m_fn = std::move(f);
        ifile.open(filename, std::ios::in);
        if(!ifile) m_can_iter = false;
        else m_can_iter = true;
    }

    //check if has next
    bool has_next() {
        if(!m_can_iter) return false;
        if(std::getline(ifile, m_buffer)) {}
        else {
            m_can_iter = false;
            ifile.close();
        }
        return m_can_iter;
    }

    //get the next line and process to R
    R next() {
        R r = m_fn(m_buffer);
        if(ifile.eof()) {
            m_can_iter = false;
            ifile.close();
        }
        return std::move(r);
    }

private:
    //the function object
    std::function<R(const std::string&)> m_fn;

    //can iter sysbol
    bool m_can_iter;

    //the file stream
    std::ifstream ifile;
    std::string m_buffer;
};


} //wzp

#endif //FILE_LOADER_HPP_