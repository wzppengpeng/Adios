#ifndef AIOLOS_TASK_FACTORY_HPP
#define AIOLOS_TASK_FACTORY_HPP

#include <string>
#include <memory>


#include "reflection/reflection.hpp"


#include "classify.hpp"

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
        if(task == "knn") {
            return wzp::Reflection<Classify>::create_shared("knn");
        }
        else if(task == "classify_tree") {
            return wzp::Reflection<Classify>::create_shared("classify_tree");
        }
        else {
            return nullptr;
        }
    }
};



} //aiolos

#endif /*AIOLOS_TASK_FACTORY_HPP*/