#include "mapper.hpp"

#include "function/help_function.hpp"

using namespace std;
using namespace wzp;
using namespace adios;

void test_mapper() {
    print("-----test origin mapper------");
    Mapper mapper("127.0.0.1", 6379, "num_list", "numlist");
    std::vector<string> v = {"1", "2", "3", "4", "5", "6"};
    auto res = mapper.split(v);
    print("numlist's size is", res);
    mapper.change("letter_list", "letterlist");
    res = mapper.split("letter.txt");
    print("letter's size is", res);
    print("--------------------------");
}


int main(int argc, char const *argv[])
{
    test_mapper();
    return 0;
}