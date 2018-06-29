#ifndef CMD_HPP
#define CMD_HPP

#include <stdio.h>

#include <string>
#include <vector>

#include "wzp_cpp_lib/my_string/string.hpp"

#define MAXLINE 100

namespace wzp {

//the command exec function
inline static bool exec_with_res(const std::string& cmd, std::vector<std::string>& cmd_result) {
    auto ptr = popen(cmd.c_str(), "r");
    if(ptr == nullptr) return false;
    char str[MAXLINE];
    while(fgets(str, MAXLINE, ptr)) {
        std::string cache(str);
        cmd_result.emplace_back(cache.substr(0, cache.size() - 1));
    }
    if(pclose(ptr) == -1) return false;
    return true;
}


// the command exec function for a simple version, no return values
inline static void exec(const std::string& cmd) {
    int res = system(cmd.c_str());
}

} // wzp

#endif // CMD_HPP
