#include <cstdlib>
#include <iostream>
#include <string>

#include "worker/worker_node.hpp"

using namespace std;
using namespace adios;

int main(int argc, char const *argv[])
{
    /* code */
    short port = (short)atoi(argv[1]);
    WorkerNode worker_node(port);
    worker_node.set_redis_list_name("wzp_list");
    worker_node.run();
    return 0;
}