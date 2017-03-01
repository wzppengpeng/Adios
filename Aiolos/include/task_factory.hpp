#ifndef AIOLOS_TASK_FACTORY_HPP
#define AIOLOS_TASK_FACTORY_HPP

#include <string>
#include <memory>


#include "reflection/reflection.hpp"


#include "classify.hpp"
#include "regression.hpp"

namespace aiolos
{

template<typename Objective>
class TaskFactory {

public:
    /**
     * create function
     */
    inline static std::shared_ptr<Objective> create(const std::string& task) {
        return nullptr;
    }

};

template<>
class TaskFactory<Classify> {
public:
    inline static std::shared_ptr<Classify> create(const std::string& task) {
        return wzp::Reflection<Classify>::create_shared(task);
    }
};

template<>
class TaskFactory<Regression> {
public:
    inline static std::shared_ptr<Regression> create(const std::string& task) {
        return wzp::Reflection<Regression>::create_shared(task);
    }
};

} //aiolos

#endif /*AIOLOS_TASK_FACTORY_HPP*/