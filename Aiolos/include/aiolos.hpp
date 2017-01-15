#ifndef AILOS_HPP
#define AILOS_HPP

#include <string>
#include <memory>

#include "log/log.hpp"


#include "task_factory.hpp"
#include "io_helper.hpp"

namespace aiolos
{

class Aiolos {
public:
    static std::shared_ptr<Aiolos> Instance(const std::string& config);

    static std::shared_ptr<Aiolos> GetInstance();

    //public function
    void run();

private:
    Aiolos(const Aiolos&) = delete;
    Aiolos(Aiolos&&) = delete;

    Aiolos& operator= (const Aiolos&) = delete;

    Aiolos(const std::string& config);

    void init(const std::string& objective);

    /**
     * classify function
     */
    void classify();

    void classify_train();

    inline void classify_predict();

private:
    enum class ObjectiveType {
        Classify,
        Regression
    };

    enum class Mode {
        Train,
        Predict,
        Both
    };
    //members
    wzp::ConfigParser parser;
    IoHelper m_io_helper;
    std::shared_ptr<Classify> m_classify;
    ObjectiveType m_objective_type;
    Mode m_mode_type = Mode::Train;

    static std::shared_ptr<Aiolos> m_instance;

};

}

#endif /*AIOLOS_HPP*/