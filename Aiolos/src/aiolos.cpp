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
    else if(m_objective_type == ObjectiveType::Regression) {
        regression();
    }
    else if(m_objective_type == ObjectiveType::Cluster) {
        cluster();
    }
    log::info("Aiolos Job Success!");
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
    else if(objective == "regression") {
        m_objective_type = ObjectiveType::Regression;
    }
    else if(objective == "cluster") {
        m_objective_type = ObjectiveType::Cluster;
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
        if(m_classify.get() == nullptr) {
            log::fatal("Task Illegal", task);
        }
    }
    else if(m_objective_type == ObjectiveType::Regression) {
        m_regression = TaskFactory<Regression>::create(task);
        if(m_regression.get() == nullptr) {
            log::fatal("Task Illegal", task);
        }
    }
    else if(m_objective_type == ObjectiveType::Cluster) {
        m_cluster = TaskFactory<Cluster>::create(task);
        if(m_cluster.get() == nullptr) {
            log::fatal("Task Illegal", task);
        }
    }
    else {
        log::fatal("Wrong Task With Wrong Objective", task);
    }
    //get mode
    if(m_objective_type == ObjectiveType::Cluster) return;
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
        classify_train();
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
        classify_predict();
    }
    else {
        classify_train();
        classify_predict();
    }
}

void Aiolos::regression() {
    m_regression->init(&parser);
    if(m_mode_type == Mode::Train) {
        //judge has validate?
        //TODO:
        regression_train();
        auto path = m_io_helper.get_dump_path();
        if(path.empty()) {
            log::fatal("Miss Arg model_path");
        }
        m_regression->dump_model(path.c_str());
    }
    else if(m_mode_type == Mode::Predict) {
        auto path = m_io_helper.get_restore_path();
        if(path.empty()) {
            log::fatal("Miss Arg model_path");
        }
        m_regression->restore_model(path.c_str());
        regression_predict();
    }
    else {
        regression_train();
        regression_predict();
    }
}

void Aiolos::classify_train() {
    if(m_io_helper.has_validate()) {
        auto input = m_io_helper.read_data("input_path");
        auto validate = std::move(m_io_helper.read_data("validate_path"));
        m_classify->train(input.first, input.second, validate.first, validate.second);
    }
    else {
        auto input = m_io_helper.read_data("input_path");
        m_classify->train(input.first, input.second);
    }
}

void Aiolos::regression_train() {
    if(m_io_helper.has_validate()) {
        auto input = m_io_helper.read_data_series("input_path");
        auto validate = std::move(m_io_helper.read_data_series("validate_path"));
        m_regression->train(input.first, input.second, validate.first, validate.second);
    }
    else {
        auto input = m_io_helper.read_data_series("input_path");
        m_regression->train(input.first, input.second);
    }
}

inline void Aiolos::classify_predict() {
    auto input = m_io_helper.read_data("predict_path");
    auto res = m_classify->predict(input.first);
    m_io_helper.output_data(res);
}

inline void Aiolos::regression_predict() {
    auto input = m_io_helper.read_data_series("predict_path");
    auto res = m_regression->predict(input.first);
    m_io_helper.output_data(res);
}

void Aiolos::cluster() {
    m_cluster->init(&parser);
    auto input = m_io_helper.read_raw_data("input_path");
    auto res = m_cluster->run(input);
    m_io_helper.output_data(res);
}

}

