#include "io_helper.hpp"

#include "log/log.hpp"

using namespace std;
using namespace wzp;

namespace aiolos
{

std::pair<wzp::Matrix<Type>, wzp::Matrix<int>> IoHelper::read_data(const std::string& path) noexcept {
    assert(m_parser != nullptr);
    string input_data_path;
    if(!m_parser->get(path, input_data_path)) {
        log::fatal("Miss Arg", path, "the input data should be csv");
    }
    wzp::Matrix<Type> raw_data;
    raw_data.read_csv(input_data_path.c_str());
    if(raw_data.empty() || raw_data.cols() == 0) {
        log::fatal("Input Data Illegal");
    }
    auto features = raw_data.slice(0, 0, raw_data.rows(), raw_data.cols() - 1);
    wzp::Matrix<int> labels(raw_data.rows(), 1);
    for(size_t i = 0; i < labels.rows(); ++i) {
        labels(i, 0) = static_cast<int>(raw_data(i, raw_data.cols() - 1));
    }
    return {features, labels};
}

std::pair<wzp::Matrix<Type>, wzp::Matrix<Type>> IoHelper::read_data_series(const std::string& path) noexcept {
    assert(m_parser != nullptr);
    string input_data_path;
    if(!m_parser->get(path, input_data_path)) {
        log::fatal("Miss Arg", path, "the input data should be csv");
    }
    wzp::Matrix<Type> raw_data;
    raw_data.read_csv(input_data_path.c_str());
    if(raw_data.empty() || raw_data.cols() == 0) {
        log::fatal("Input Data Illegal");
    }
    auto features = raw_data.slice(0, 0, raw_data.rows(), raw_data.cols() - 1);
    wzp::Matrix<Type> labels(raw_data.rows(), 1);
    for(size_t i = 0; i < labels.rows(); ++i) {
        labels(i, 0) = raw_data(i, raw_data.cols() - 1);
    }
    return {features, labels};
}

std::string IoHelper::get_dump_path() {
    assert(m_parser != nullptr);
    string path;
    if(!m_parser->get("model_path", path)) {
        return "";
    }
    return path;
}

std::string IoHelper::get_restore_path() {
    assert(m_parser != nullptr);
    string path;
    if(!m_parser->get("model_path", path)) {
        return "";
    }
    return path;
}

}