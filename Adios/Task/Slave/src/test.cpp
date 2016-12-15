#include <iostream>

#include "slaver.hpp"

using namespace std;

int main(int argc, char** argv) {
    if(argc != 2) {
        cerr<<"USAGE: ./add config.ini"<<endl;
        return -1;
    }
    adios::Slaver slaver(argv[1]);
    slaver.start();
    return 0;
}