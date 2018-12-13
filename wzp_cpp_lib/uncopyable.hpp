#ifndef WZP_UNCOPYABLE_HPP_
#define WZP_UNCOPYABLE_HPP_

namespace wzp {

class Uncopyable {

protected:
    Uncopyable() {}
    ~Uncopyable() {}

private:
    Uncopyable(const Uncopyable&);
    Uncopyable& operator= (const Uncopyable&);

};

} //wzp

#endif /*WZP_UNCOPYABLE_HPP_*/