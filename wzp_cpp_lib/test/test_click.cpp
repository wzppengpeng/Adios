#include "config/click.hpp"

using namespace std;
using namespace wzp;

int main(int argc, char *argv[])
{
    wzp::Click::add_argument("--name", "the name of who");
    Click::add_argument("--age", "the age of who");
    Click::parse(argc, argv);
    string name = Click::get("--name");
    int age = Click::get<int>("--age");
    cout << name << " " << age << endl;
    double ll = Click::get<double>("--ll");
    return 0;
}