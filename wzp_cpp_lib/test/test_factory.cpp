#include "reflection/factory.hpp"

#include "function/help_function.hpp"

using namespace std;
using namespace wzp;

//test the reflection
template<typename T>
class B {
public:
    B(T x, T y) : x_(x), y_(y)
    {}

    virtual void fuck() = 0;

protected:
    T x_;
    T y_;

};

class D : public B<int> {
public:
    D(int x, int y) : B(x, y) {}
    void fuck() override {
        print("I'm D");
        print(x_, y_);
    }
private:
    static FactoryRegister(B<int>, D, int, int) regis_d;
};

class C : public B<int> {
public:
    C(int x, int y) : B(x, y) {}
    void fuck() override {
        print("I'm C");
        print(x_, y_);
    }
private:
    static FactoryRegister(B<int>, C, int, int) regis_c;
};

FactoryRegister(B<int>, D, int, int) D::regis_d("D");
FactoryRegister(B<int>, C, int, int) C::regis_c("C");


int main(int argc, char const *argv[])
{
    auto it = wzp::Factory<B<int>, int, int>::create_unique("D", std::make_tuple(1, 3));
    it->fuck();
    auto ii = wzp::Factory<B<int>, int, int>::create_shared("C", std::make_tuple(2, 4));
    ii->fuck();
    return 0;
}



