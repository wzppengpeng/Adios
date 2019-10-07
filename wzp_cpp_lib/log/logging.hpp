#ifndef WZP_CPP_LIB_LOGGING_HPP_
#define WZP_CPP_LIB_LOGGING_HPP_

/**
 * C风格的logging模块，支持打印等级控制，（通过vprinf实现，单条信息线程安全，但可能出现头部信息不对应情况）
 * 为源log.hpp的改良版，支持c风格格式化、线程安全、切换接口丰富
 */

#include <ctime>
#include <cstdarg>

#include <fstream>
#include <string>

#include "wzp_cpp_lib/uncopyable.hpp"

namespace wzp
{

// A enumeration type of log message levels. The values are ordered:
// Debug < Info < Warning < Fatal.
enum class LogLevel : int {
    Debug = 0,
    Info = 1,
    Warning = 2,
    Fatal = 3
};


/**!
 * \brief The Logger class is for write infomation into output stream
 */
class Logger : Uncopyable {

// enable the static class Logging to call the object
friend class Logging;

public:
    /**
     * write to stdout and the level is info
     * @param  level the level of logging
     * @return       none
     */
    explicit Logger(LogLevel level = LogLevel::Info)
        : file_(nullptr), level_(level), is_kill_fatal_(true), is_to_log_err_(false) {}

    /**
     * output to file
     * @param  filename the output file stream
     * @param  level    the level of now
     * @return          none
     */
    explicit Logger(const std::string& filename, LogLevel level = LogLevel::Info)
        : file_(nullptr), level_(level), is_kill_fatal_(true), is_to_log_err_(false) {
        ResetLogFile(filename);
    }

    ~Logger() {
        CloseLogFile();
    }

    /**
     * reset the log output file
     * @param  filename the output log filename
     * @return          true or false (open/close file)
     */
    bool ResetLogFile(const std::string& filename) {
        CloseLogFile();
        file_ = fopen(filename.c_str(), "w");
        if(!file_) {
            Write(LogLevel::Warning, "Cannot create log file %s", filename.c_str());
            return false;
        }
        return true;
    }

    /**
     * reset the log level
     * @param level the level of new one
     */
    void ResetLogLevel(LogLevel level) { level_ = level; }

    /**
     * reset the option if kill the process when fatal has been touched
     * @param is_kill_fatal option of kill
     */
    void ResetKillFatal(bool is_kill_fatal) { is_kill_fatal_ = is_kill_fatal; }

    /**
     * reset if the log output is to stderr
     * @param log_to_std_err the option
     */
    void ResetLogToStdErr(bool log_to_std_err) { is_to_log_err_ = log_to_std_err; }

    /**
     * reset the logging header
     * @param header the head info
     */
    void ResetLogHeader(const std::string& header) { header_ = header; }

    /**
     * the write info help functions
     */
    void Write(LogLevel level, const char *format, ...) {
        va_list val;
        va_start(val, format);
        WriteImpl(level, format, &val);
        va_end(val);
    }

private:
    void WriteImpl(LogLevel level, const char* format, va_list* val) {
        if(level < level_)
            return;
        auto level_str = GetLevelStr(level);
        auto time_str = GetSystemTime();
        va_list val_copy;
        va_copy(val_copy, *val);
        // write to std out
        if(is_to_log_err_) {
            fprintf(stderr, "[%s] [%s] [%s] ", header_.c_str(), level_str.c_str(), time_str.c_str());
            vfprintf(stderr, format, *val);
            fprintf(stderr, "\n");
        } else {
            printf("[%s] [%s] [%s] ", header_.c_str(), level_str.c_str(), time_str.c_str());
            vprintf(format, *val);
            printf("\n");
            fflush(stdout);
        }
        // write to log file
        if(file_) {
            fprintf(file_, "[%s] [%s] [%s] ", header_.c_str(), level_str.c_str(), time_str.c_str());
            vfprintf(file_, format, val_copy);
            fprintf(file_, "\n");
            fflush(file_);
        }
        va_end(val_copy);
        if(is_kill_fatal_ && level_ == LogLevel::Fatal) {
            CloseLogFile();
            exit(-1);
        }
    }
    // close the log file
    void CloseLogFile() {
        if(file_) {
            fclose(file_);
            file_ = nullptr;
        }
    }
    // Returns current system time as a string.
    std::string GetSystemTime() {
        time_t t = time(0);
        char str[64];
        strftime(str, sizeof(str), "%Y-%m-%d %H:%M:%S", localtime(&t));
        return str;
    }
    // Returns the string of a log level.
    std::string GetLevelStr(LogLevel level) {
        switch (level) {
            case LogLevel::Debug: return "DEBUG";
            case LogLevel::Info: return "INFO";
            case LogLevel::Warning: return "WARNING";
            case LogLevel::Fatal: return "FATAL";
            default: return "UNKNOW";
        }
    }

    std::FILE *file_;  // A file pointer to the log file.
    LogLevel level_;   // Only the message not less than level_ will be outputted.
    bool is_kill_fatal_;  // If kill the process when fatal error occurs.
    bool is_to_log_err_; // The option if is to stderr

    // the basic header info to print
    std::string header_ = "LOGGING";

}; // Logger


/**
 * the Logging static class is a wrapper to write msg to the log file
 */
class Logging {

public:
    /**
     * reset the log file
     * @param  filename the new output file name
     * @return          false if fail to set the otuput file, true otherwise
     */
    inline static bool ResetLogFile(const std::string& filename) {
        return GetLogger().ResetLogFile(filename);
    }

    /**
     * reset the minimal log level
     * @param level the level
     */
    inline static void ResetLogLevel(LogLevel level) {
        GetLogger().ResetLogLevel(level);
    }

    // reset if kill fatal
    inline static void ResetKillFatal(bool is_kill_fatal) {
        GetLogger().ResetKillFatal(is_kill_fatal);
    }

    // reset if the output write to std err
    inline static void ResetLogToStdErr(bool log_to_std_err) {
        GetLogger().ResetLogToStdErr(log_to_std_err);
    }

    // reset the log header info
    inline static void ResetLogHeader(const std::string& header) {
        GetLogger().ResetLogHeader(header);
    }

    /**
     * The C-style write msg
     */
    static void Debug(const char *format, ...) {
        va_list val;
        va_start(val, format);
        GetLogger().WriteImpl(LogLevel::Debug, format, &val);
        va_end(val);
    }

    static void Info(const char *format, ...) {
        va_list val;
        va_start(val, format);
        GetLogger().WriteImpl(LogLevel::Info, format, &val);
        va_end(val);
    }

    static void Warning(const char *format, ...) {
        va_list val;
        va_start(val, format);
        GetLogger().WriteImpl(LogLevel::Warning, format, &val);
        va_end(val);
    }

    static void Fatal(const char *format, ...) {
        va_list val;
        va_start(val, format);
        GetLogger().WriteImpl(LogLevel::Fatal, format, &val);
        va_end(val);
    }

private:
    static Logger& GetLogger() {
        static Logger logger_;
        return logger_;
    }

}; // Logging


} //wzp


// a macro to assert some bool
#ifndef ASSERT
#define ASSERT(BOOL_SYSBOL, STR, ...) { \
            if(!(BOOL_SYSBOL)) { \
                wzp::Logging::Fatal(STR, ##__VA_ARGS__); \
            } \
        }
#endif


// a macro to simple the call for functions
#ifndef LOG_INFO
#define LOG_INFO(STR, ...) wzp::Logging::Info(STR, ##__VA_ARGS__)
#endif

#ifndef LOG_FATAL
#define LOG_FATAL(STR, ...) wzp::Logging::Fatal(STR, ##__VA_ARGS__)
#endif

#ifndef LOG_WARNING
#define LOG_WARNING(STR, ...) wzp::Logging::Warning(STR, ##__VA_ARGS__)
#endif

#endif /*WZP_CPP_LIB_LOGGING_HPP_*/