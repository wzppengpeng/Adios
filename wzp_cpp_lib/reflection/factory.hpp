#ifndef REFLECTION_FACTORY_HPP_
#define REFLECTION_FACTORY_HPP_

#include <memory>
#include <string>
#include <functional>
#include <map>

/**
 *
 *this class is a singleton class to registry class and users can get class just vy their names
 *the method to solve ths problem is that using the static virables will build first in gcc
 *the problem now is that this model cannot solve the template, but I think that not enssencial
 *another problem is that now it can  receive the class which has any paramter
 *
 * assum B(int, const string&), D : public B
 * define a static variable in the complier unit, like:
 * //e.g.
 *  FACTORY_REGISTER(B, D, int, const string&) regis_d("D";)
 *
 * then can be called in other files
 * auto it = FACTORY(B, int, const string&)::create_unique("D");
 *
 * // after testing, it can worked on no inputs and (T, T&, T&&, const T&, inputs)
 */

namespace wzp {

template<typename Base, typename... Args>
class Factory {
public:
    template<typename Sub>
    struct Registry {
        Registry(const std::string& class_name) {
            Factory<Base, Args...>::get().registry_map_.emplace(class_name,
                [](Args... args) { return new Sub(std::forward<Args>(args)...); } );
        }

        Registry(std::string&& class_name) {
            Factory<Base, Args...>::get().registry_map_.emplace(class_name,
                [](Args... args) { return new Sub(std::forward<Args>(args)...); } );
        }
    };

    template<typename... A>
    static Base* create(const std::string& class_name, A&&... args) {
        auto it = get().registry_map_.find(class_name);
        if(it != get().registry_map_.end()) {
            return it->second(std::forward<A>(args)...);
        } else {
            return nullptr;
        }
    }

    template<typename... A>
    inline static std::unique_ptr<Base> create_unique(const std::string& class_name, A&&... args) {
        return std::unique_ptr<Base>(create(class_name, std::forward<A>(args)...));
    }

    template<typename... A>
    inline static std::shared_ptr<Base> create_shared(const std::string& class_name, A&&... args) {
        return std::shared_ptr<Base>(create(class_name, std::forward<A>(args)...));
    }

private:
    // delete the construct and copy move construct
    Factory() = default;
    Factory(const Factory<Base, Args...>&) = delete;
    Factory(Factory<Base, Args...>&&) = delete;

    // create singleton
    static Factory<Base, Args...>& get() {
        static Factory<Base, Args...> instance_;
        return instance_;
    }

private:
    std::map<std::string, std::function<Base*(Args...)> > registry_map_;

};

} //wzp

// the macro for get the factory
#ifndef FACTORY
#define FACTORY(Base, ...) wzp::Factory<Base, ##__VA_ARGS__>
#endif

//the define macro
#ifndef FACTORY_REGISTER
#define FACTORY_REGISTER(Base, Sub, ...) FACTORY(Base, ##__VA_ARGS__)::Registry<Sub>
#endif

#endif /*REFLECTION_FACTORY_HPP_*/