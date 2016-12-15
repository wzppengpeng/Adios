#include <iostream>
#include "../container/Variant.hpp"
#include "../container/Any.hpp"

void TestVariant()
{
    using wzp::Variant;
    typedef Variant<int, double, string, int> cv;
    //根据index获取类型
    cout << typeid(cv::IndexType<1>).name() << endl;

    //根据类型获取索引
    cv v=10;
    int i = v.GetIndexOf<string>();

    //通过一组lambda访问vairant
    v.Visit([&](double i){cout << i << endl; }, [&](short i){cout << i << endl; }, [](int i){cout << i << endl; },
        [](string i){cout << i << endl; }
    );

    cv v_ = string("wzp");

    cout<<v_.Get<string>()<<endl;

    bool emp1 = v.Empty();
    cout << v.Type().name() << endl;
}

void TestAny()
{
    using wzp::Any;
    Any n;
    auto r = n.IsNull();//true
    string s1 = "hello";
    n = s1;
    n = string("hhh");
    cout<<n.AnyCast<string>()<<std::endl;; //can not cast int to string
    Any n1 = 1;
    cout<<n1.AnyCast<int>()<<endl;
    n1.Is<int>(); //true
}

int main() {
    // TestVariant();
    TestAny();
    return 0;
}
