#include <cstdlib>

#include "http_server.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    if(argc != 2) {
        cerr<<"USAGE: ./server 9000"<<endl;
        return -1;
    }
    short port = (short)atoi(argv[1]);
    adios::HttpServer server(port);
    unordered_map<string, string> m = {{"/test", "test"}};
    server.registry(std::move(m));
    server.run();
    return 0;
}