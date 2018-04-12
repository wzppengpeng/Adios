#ifndef WZP_FASHION_FUNCTOR_HPP_
#define WZP_FASHION_FUNCTOR_HPP_

#include "apply_tuple.hpp"

namespace wzp
{

/**
 * the functor of curry functor
 */
namespace curry
{
template<typename F, typename Before = std::tuple<>, typename After = std::tuple<> >
class curry_functor {
private:
    F f_;
    Before before_ = std::tuple<>();
    After after_ = std::tuple<>();

public:
    curry_functor(F&& f) : f_(std::forward<F>(f)) {}
    // must be the copy functor!!
    curry_functor(const F& f, const Before& before, const After& after)
        : f_(f), before_(before), after_(after) {}

    template<typename... Args>
    auto operator() (Args&&... args)
    -> decltype(wzp::apply(f_, std::tuple_cat(before_, std::make_tuple(std::forward<Args>(args)...), after_)))
    {
        return wzp::apply(f_, std::tuple_cat(before_, std::make_tuple(std::forward<Args>(args)...), after_));
    }

    template<typename... Args>
    auto curry_before(Args&&... params) const
    -> curry_functor<F, decltype(std::tuple_cat(before_, std::make_tuple(std::forward<Args>(params)...))), After>
    {
        return curry_functor<F, decltype(std::tuple_cat(before_, std::make_tuple(std::forward<Args>(params)...))), After>(
            f_, std::tuple_cat(before_, std::make_tuple(std::forward<Args>(params)...)), after_);
    }

    template<typename... Args>
    auto curry_after(Args&&... params) const
    -> curry_functor<F, Before, decltype(std::tuple_cat(after_, std::make_tuple(std::forward<Args>(params)...)))>
    {
        return curry_functor<F, Before, decltype(std::tuple_cat(after_, std::make_tuple(std::forward<Args>(params)...)))>(
            f_, before_, std::tuple_cat(after_, std::make_tuple(std::forward<Args>(params)...)));
    }

};
} //curry functor namespace

// the help function to change function into curry one
template<typename F>
auto fun_to_curry(F&& f) -> curry::curry_functor<F>
{
    return curry::curry_functor<F>(std::forward<F>(f));
}


} //wzp

// the help function to use << and >> for curry before and after
template<typename CF, typename Arg>
auto operator<< (const CF& f, Arg&& arg)
-> decltype(f.curry_before(std::forward<Arg>(arg)))
{
    return f.curry_before(std::forward<Arg>(arg));
}

template<typename CF, typename Arg>
auto operator>> (const CF& f, Arg&& arg)
-> decltype(f.curry_after(std::forward<Arg>(arg)))
{
    return f.curry_after(std::forward<Arg>(arg));
}

#endif /*WZP_FASHION_FUNCTOR_HPP_*/