#ifndef AIOLOS_IO_HELPER_HPP
#define AIOLOS_IO_HELPER_HPP

#include <utility>


#include "config/config_parser.hpp"
#include "container/matrix.hpp"

#include "common.hpp"

namespace aiolos
{

/**
 * Class to handle input and output
 */
class IoHelper {
public:
    IoHelper() = default;

    /**
     * set parser
     */
    inline void set_parser(wzp::ConfigParser* parser);

    /**
     * input data and trainsorm types
     */
    std::pair<wzp::Matrix<Type>, wzp::Matrix<int>> read_data(const std::string& path) noexcept;

    /**
     * output data for a template matrix
     */
    template<typename Mat>
    inline void output_data(const Mat& m) {
        assert(m_parser != nullptr);
        std::string arg;
        if(!m_parser->get("output", arg) || arg == "console") {
            //set Console
            m.print();
        }
        else {
            m.to_csv(arg.c_str());
        }
    }

    /**
     * get model dump area
     */
    std::string get_dump_path();

    /**
     * get model restore path
     */
    std::string get_restore_path();

private:
    wzp::ConfigParser* m_parser = nullptr;
};

void IoHelper::set_parser(wzp::ConfigParser* parser) {
    m_parser = parser;
}


} //aiolos


#endif /*AIOLOS_IO_HELPER_HPP*/