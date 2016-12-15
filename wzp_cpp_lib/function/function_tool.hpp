#ifndef FUNCION_TOOL_HPP_
#define FUNCION_TOOL_HPP_

#include <utility>

using std::declval;

namespace wzp{

/*a wise function wrapper, receive any type param*/
template<class Func, class... Args, class = typename std::enable_if<!std::is_member_function_pointer<Func>::value>::type>
inline auto function_wrapper(Func&& f, Args&&... args)
 -> decltype(f(std::forward<Args>(args)...)){
    return f(std::forward<Args>(args)...);
}


/*the wrapper for class function*/
template<class R, class C, class... DArgs, class P, class... Args>
inline auto function_wrapper(R(C::*f)(DArgs...), P && p, Args && ... args)
 -> decltype((*p.*f)(std::forward<Args>(args)...))
{
    return (*p.*f)(std::forward<Args>(args)...);
}


/*
the compose function, get only one param and a list of function,
return the list function called result
 */
template<typename OutFun, typename InnerFun>
class Composed
{
public:
    explicit Composed(OutFun out, InnerFun inner) : m_out_fn(out), m_inner_fn(inner) {}

public:
    //the operater (), for function object, this is to say that the function can only recieve
    //the arg by value, and only one value
    template<typename Arg>
    auto operator()(Arg arg)
    -> decltype(declval<OutFun>()((declval<InnerFun>()(declval<Arg>())))) {
        return m_out_fn(m_inner_fn(arg));
    }

private:
    OutFun m_out_fn;
    InnerFun m_inner_fn;
};

//the function for using
template<typename Func1, typename Func2>
Composed<Func1, Func2> compose(Func1 fun1, Func2 fun2) {
    return Composed<Func1, Func2>(fun1, fun2);
}

template<typename Func1, typename Func2, typename Func3, typename... Fs>
auto compose(Func1 f1, Func2 f2, Func3 f3, Fs... fs)
 -> decltype(compose(compose(f1, f2), f3, fs...)) {
    return compose(compose(f1, f2), f3, fs...);
}


}//wzp

#endif /*FUNCION_TOOL_HPP_*/