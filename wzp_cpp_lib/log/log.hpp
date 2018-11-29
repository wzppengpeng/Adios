#ifndef LOG_HPP_
#define LOG_HPP_

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <ratio>
#include <chrono>

#include "wzp_cpp_lib/common.h"

/**
 * How to Use:
 * as it is a staic class, just call the global function
 * it is default as console and Info level
 * LOG_INFO(xxx) is ok, maybe use it with wzp::format together
 */

namespace wzp {

/*the type of log level*/
enum class log_level: int{
    Fatal = -1,
    Warning = 0,
    Info = 1,
    Debug = 2,
};

enum class log_type: int{
    Console = 0,
    File = 1,
};

class log {
public:
    /*set the log level*/
    static void set_log_level(log_level level){
        get_level() = level;
    }

    /*set the log type*/
    static void set_log_type(log_type type){
        get_type() = type;
    }

    /*set the log file location*/
    static void set_log_file_path(std::string&& file_path){
        get_file_path() = file_path;
    }

    /*init function*/
    static void log_init(log_level level=log_level::Info, log_type type=log_type::Console,
                std::string&& file_path="wzp.log"){
        set_log_level(level);
        set_log_type(type);
        set_log_file_path(std::move(file_path));
    }


    /*the debug level log*/
    template<typename... Args>
    static void debug(const std::string& format, Args&&... args){
        if(get_level() >= log_level::Debug){
            std::string debug_str {"[Adios] [Debug] "};
            debug_str.append(format);
            write(debug_str, std::forward<Args>(args)...);
        }
    }

    template<typename... Args>
    static void info(const std::string& format, Args&&... args){
        if(get_level() >= log_level::Info){
            std::string info_str {"[Adios] [Info] "};
            info_str.append(format);
            if(get_type() == log_type::Console)
                get_stream() << WHITE;
            write(info_str, std::forward<Args>(args)...);
            if(get_type() == log_type::Console)
                get_stream() << RESET;
        }
    }

    template<typename... Args>
    static void warning(const std::string& format, Args&&... args){
        if(get_level() >= log_level::Warning){
            std::string error_str {"[Adios] [Warning] "};
            error_str.append(format);
            if(get_type() == log_type::Console)
                get_stream() << YELLOW;
            write(error_str, std::forward<Args>(args)...);
            if(get_type() == log_type::Console)
                get_stream() << RESET;
        }
    }

    template<typename... Args>
    static void fatal(const std::string& format, Args&&... args){
        if(get_level() >= log_level::Fatal){
            std::string error_str {"[Adios] [Fatal] "};
            error_str.append(format);
            if(get_type() == log_type::Console)
                get_stream() << BOLDRED;
            write(error_str, std::forward<Args>(args)...);
            if(get_type() == log_type::Console)
                get_stream() << RESET;
            exit(1);
        }
    }

private:
    /*define a global log level*/
    static log_level& get_level(){
        static log_level level = log_level::Info;
        return level;
    }

    /*define a global log type*/
    static log_type& get_type(){
        static log_type type = log_type::Console;
        return type;
    }

    /*get the output stream*/
    static std::ostream& get_stream(){
        if(get_type() == log_type::Console)
            return std::cout;
        else {
            static std::ofstream outfile(get_file_path(), std::ofstream::app);
            return outfile;
        }
    }

    /*get the log file path*/
    static std::string& get_file_path(){
        static std::string log_file_path("wzp.log");
        return log_file_path;
    }

    //the tool write funciton
    /*the end function*/
    template<typename T>
    static void write(T&& t){
        using std::chrono::system_clock;
        auto tp = system_clock::now();
        auto time = system_clock::to_time_t(tp);
        std::string format {"["};
        std::string str_time = std::ctime(&time);
        str_time.pop_back();
        format.append(str_time);
        format.push_back(']');
        get_stream() << t << ' ' << format << std::endl;
    }

    /*the normal write*/
    template<typename T, typename... Args>
    static void write(T&& t, Args&&... args){
        get_stream() << t << ", ";
        write(std::forward<Args>(args)...);
    }

};

//a macro to assert some bool
#ifndef ASSERT
#define ASSERT(BOOL_SYSBOL, STR, ...) { \
            if(!(BOOL_SYSBOL)) { \
                wzp::log::fatal(STR, ##__VA_ARGS__); \
            } \
        }
#endif


//a macro to CHECK, more good
#ifndef CHECK
#define CHECK(condition) \
        if (!(condition)) wzp::log::fatal("Check Fatal: [" #condition \
            "] in file", __FILE__,  "line", __LINE__);
#endif //CHECK

// a macro to simple the call for functions
#ifndef LOG_INFO
#define LOG_INFO(STR, ...) wzp::log::info(STR, ##__VA_ARGS__)
#endif

#ifndef LOG_FATAL
#define LOG_FATAL(STR, ...) wzp::log::fatal(STR, ##__VA_ARGS__)
#endif

#ifndef LOG_WARNING
#define LOG_WARNING(STR, ...) wzp::log::warning(STR, ##__VA_ARGS__)
#endif

}//wzp

#endif /*LOG_HPP_*/