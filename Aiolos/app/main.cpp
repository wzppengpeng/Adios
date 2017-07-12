#include "aiolos.hpp"

#include "config/arg_parser.hpp"

using namespace std;
using namespace aiolos;

int main(int argc, char *argv[])
{
    wzp::ArgParser::parse(argc, argv);
    auto it = Aiolos::Instance(wzp::ArgParser::get("conf"));
    it->run();
    return 0;
}