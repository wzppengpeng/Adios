#include "reflection/factory.hpp"

#include "function/help_function.hpp"

using namespace std;
using namespace wzp;

//test the reflection
class B {
public:
    B(int x, int y) : x_(x), y_(y)
    {}

    virtual void fuck() = 0;

protected:
    int x_;
    int y_;

};

class D : public B {
public:
    D(int x, int y) : B(x, y) {}
    void fuck() override {
        print("I'm D");
        print(x_, y_);
    }
private:
    static FactoryRegister(B, D, int, int) regis_d;
};

class C : public B {
public:
    C(int x, int y) : B(x, y) {}
    void fuck() override {
        print("I'm C");
        print(x_, y_);
    }
private:
    static FactoryRegister(B, C, int, int) regis_c;
};

FactoryRegister(B, D, int, int) D::regis_d("D");
FactoryRegister(B, C, int, int) C::regis_c("C");


int main(int argc, char const *argv[])
{
    auto it = wzp::Factory<B, int, int>::create_unique("D", std::make_tuple(1, 3));
    it->fuck();
    auto ii = wzp::Factory<B, int, int>::create_shared("C", std::make_tuple(2, 4));
    ii->fuck();
    return 0;
}



