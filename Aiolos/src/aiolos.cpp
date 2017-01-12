#include "aiolos.hpp"


using namespace std;
using namespace wzp;

namespace aiolos
{

std::shared_ptr<Aiolos> Aiolos::m_instance(nullptr);

std::shared_ptr<Aiolos> Aiolos::Instance(const std::string& config) {
    if(!m_instance.get()) {
        m_instance = (std::shared_ptr<Aiolos>)(new Aiolos(config));
    }
    return m_instance;
}

std::shared_ptr<Aiolos> Aiolos::GetInstance() {
    return m_instance;
}

void Aiolos::run() {
    string objective;
    if(!parser.get("objective", objective)) {
        log::fatal("Miss Arg objective");
    }
    init(objective);
    if(m_objective_type == ObjectiveType::Classify) {
        classify();
    }
}

Aiolos::Aiolos(const std::string& config) : parser(config), m_classify(nullptr)
{
    log::log_init(log_level::Info, log_type::Console);
    if(!parser.config())
        log::fatal("config illegal!");
    m_io_helper.set_parser(&parser);
}

void Aiolos::init(const std::string& objective) {
    if(objective == "classify") {
        m_objective_type = ObjectiveType::Classify;
    }
    else {
        m_objective_type = ObjectiveType::Classify;
    }
    //create the point of Classify
    string task;
    if(!parser.get("task", task)) {
        log::fatal("Miss Arg task");
    }
    if(m_objective_type == ObjectiveType::Classify) {
        m_classify = TaskFactory<Classify>::create(task);
    }
    else {
        log::fatal("Wrong Task With Wrong Objective", task);
    }
    //get mode
    if(!parser.get("mode", task)) {
        log::error("Miss Arg mode");
    }
    if(task == "train") {
        m_mode_type = Mode::Train;
    }
    else if(task == "predict") {
        m_mode_type = Mode::Predict;
    }
    else {
        m_mode_type = Mode::Both;
    }
}

void Aiolos::classify() {
    m_classify->init(&parser);
    //judge train or prediction
    if(m_mode_type == Mode::Train) {
        //judge has validate?
        //TODO:
        auto input = m_io_helper.read_data("input_path");
        m_classify->train(input.first, input.second);
        auto path = m_io_helper.get_dump_path();
        if(path.empty()) {
            log::fatal("Miss Arg model_path");
        }
        m_classify->dump_model(path.c_str());
    }
    else if(m_mode_type == Mode::Predict) {
        auto path = m_io_helper.get_restore_path();
        if(path.empty()) {
            log::fatal("Miss Arg model_path");
        }
        m_classify->restore_model(path.c_str());
        auto input = m_io_helper.read_data("predict_path");
        auto res = m_classify->predict(input.first);
        m_io_helper.output_data(res);
    }
    else {
        auto input = m_io_helper.read_data("input_path");
        m_classify->train(input.first, input.second);
        input = std::move(m_io_helper.read_data("predict_path"));
        auto res = m_classify->predict(input.first);
        m_io_helper.output_data(res);
    }
    log::info("Aiolos Job Success!");
}

}

