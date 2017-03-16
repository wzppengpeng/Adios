#ifndef AIOLOS_TASK_FACTORY_HPP
#define AIOLOS_TASK_FACTORY_HPP

#include <string>
#include <memory>


#include "reflection/reflection.hpp"


#include "classify.hpp"
#include "regression.hpp"
#include "cluster.hpp"

namespace aiolos
{

template<typename Objective>
class TaskFactory {

public:
    /**
     * create function
     */
    inline static std::unique_ptr<Objective> create(const std::string& task) {
        return wzp::Reflection<Objective>::create_unique(task);
    }

};

} //aiolos

#endif /*AIOLOS_TASK_FACTORY_HPP*/