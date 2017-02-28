#ifndef AIOLOS_PYTHON_HPP
#define AIOLOS_PYTHON_HPP

#include <string>
#include <memory>

#include <Python.h>

#include "log/log.hpp"


#include "task_factory.hpp"
#include "io_helper.hpp"

using std::string;
using wzp::log;

namespace aiolos
{

class Aiolos {
public:
    /**
     * Constructor
     */
    Aiolos() {
        log::log_init(wzp::log_level::Info, wzp::log_type::Console);
    }

    /**
     * train
     */
    inline void create_data(const std::string& input_path) {
        parser.update("input_path", input_path);
    }

    inline void create_data(const std::string& input_path, const std::string& validate_path) {
        parser.update("input_path", input_path);
        parser.update("validate_path", validate_path);
    }

    void train(PyObject* params) {
        add_config_items(params);
        m_io_helper.set_parser_simple(&parser);
        init();
        if(m_objective_type == ObjectiveType::Classify) {
            classify();
        }
    }

    /**
     * predict
     */
    inline void get_data(const std::string& predict_path) {
        parser.update("predict_path", predict_path);
    }

    void predict(const std::string& output_path) {
        parser.update("output", output_path);
        if(m_objective_type == ObjectiveType::Classify)
            classify_predict();
    }

    /**
     * dump_model
     */
    inline void dump_model(const string& model_path) {
        if(m_objective_type == ObjectiveType::Classify)
            m_classify->dump_model(model_path.c_str());
    }

    /**
     * restore_model
     */
    inline void restore_model(const string& model_path,
     const string& objective = "classify", const string& task = "knn") {
        parser.update("objective", objective);
        parser.update("task", task);
        m_io_helper.set_parser_simple(&parser);
        init();
        m_classify->restore_model(model_path.c_str());
    }

private:
    enum class ObjectiveType {
        Classify,
        Regression
    };

    //members
    wzp::ConfigParser parser;
    IoHelper m_io_helper;
    std::shared_ptr<Classify> m_classify;
    ObjectiveType m_objective_type;

private:
    /**
     * Private Functions
     */
    void add_config_items(PyObject* dict_obj) {
        //add all config path into the parser
        PyObject* key;
        PyObject* value;
        Py_ssize_t i = 0;
        while (PyDict_Next(dict_obj, &i, &key, &value)) {
            const char * skey = PyString_AS_STRING(key);
            if(PyFloat_Check(value)) {
                float k = PyFloat_AS_DOUBLE(value);
                parser.update(skey, std::to_string(k));
            }
            else if(PyInt_Check(value)) {
                int k = PyInt_AS_LONG(value);
                parser.update(skey, std::to_string(k));
            }
            else {
                const char * svalue = PyString_AS_STRING(value);
                parser.update(skey, svalue);
            }
        }
    }

    void init() {
        //first check objecttive
        string objective;
        if(!parser.get("objective", objective)) {
            log::fatal("Miss Arg objective");
        }
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
            if(m_classify.get() == nullptr) {
                log::fatal("Task Illegal", task);
            }
        }
        else {
            log::fatal("Wrong Task With Wrong Objective", task);
        }
    }

    void classify() {
        m_classify->init(&parser);
        classify_train();
    }

    void classify_train() {
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

    void classify_predict() {
        auto input = m_io_helper.read_data("predict_path");
        auto res = m_classify->predict(input.first);
        m_io_helper.output_data(res);
    }

};


} //aiolos


#endif /*AIOLOS_PYTHON_HPP*/