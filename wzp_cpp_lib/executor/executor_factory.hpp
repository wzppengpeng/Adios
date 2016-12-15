#ifndef EXECUTOR_FACTORY_HPP_
#define EXECUTOR_FACTORY_HPP_

#include <unordered_map>
#include <string>
#include <functional>
#include <memory>

#include "executor.hpp"

namespace wzp {

struct factory {
    template<typename T>
    struct register_t {
        register_t(const std::string& key) {
            factory::get().m_map.emplace(key, []() { return new T(); });
        }

        //with params
        template<typename... Args>
        register_t(const std::string& key, Args... args) {
            factory::get().m_map.emplace(key, [&]() { return new T(args...); });
        }
    };

    static Executor* produce(const std::string& key) {
        if(m_map.find(key) == m_map.end()) {
            return nullptr;
        }

        return m_map[key]();
    }

    static std::unique_ptr<Executor> produce_unique(const std::string& key) {
        return std::unique_ptr<Executor>(produce(key));
    }

    static std::shared_ptr<Executor> produce_shared(const std::string& key) {
        return std::shared_ptr<Executor>(produce(key));
    }

private:
    factory() {};
    factory(const factory&) = delete;
    factory(factory&&) = delete;

    static factory& get() {
        static factory instance;
        return instance;
    }

    static std::unordered_map<std::string, std::function<Executor*()>> m_map;
};

//上面只是声明！！类的静态成员变量要在全局范围下定义！！
std::unordered_map<std::string, std::function<Executor*()>> factory::m_map;


} // wzp

#define REGISTER_EXECUTOR_VNAME(T) reg_exe_##T##_
#define REGISTER_EXECUTOR(T, key, ...) static wzp::factory::register_t<T> REGISTER_EXECUTOR_VNAME(T)(key, ##__VA_ARGS__);
//the define  REGISTER_EXECUTOR(T, "key")

#endif /*EXECUTOR_FACTORY_HPP_*/