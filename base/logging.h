#pragma once

#include <sstream>

#include <stdarg.h>

#include "misc.h"

#define Log_debug(msg, ...) ::base::Log::debug(__LINE__, __FILE__, msg, ## __VA_ARGS__)

// NEW API

#define LOG_INFO ::base::LogEntry(::base::LogLevel::INFO, __FILE__, __LINE__)
#define LOG_WARN ::base::LogEntry(::base::LogLevel::WARN, __FILE__, __LINE__)
#define LOG_ERROR ::base::LogEntry(::base::LogLevel::ERROR, __FILE__, __LINE__)
#define LOG_FATAL ::base::LogEntry(::base::LogLevel::FATAL, __FILE__, __LINE__)

// support old logging api
#define Log_info(msg, ...) LOG_INFO(msg, ## __VA_ARGS__)
#define Log_warn(msg, ...) LOG_WARN(msg, ## __VA_ARGS__)
#define Log_error(msg, ...) LOG_ERROR(msg, ## __VA_ARGS__)
#define Log_fatal(msg, ...) LOG_FATAL(msg, ## __VA_ARGS__)

namespace base {

class Log {
    static int level_s;
    static FILE* fp_s;

    static void log_v(int level, int line, const char* file, const char* fmt, va_list args);
public:

    enum {
        FATAL = 0, ERROR = 1, WARN = 2, INFO = 3, DEBUG = 4
    };

    static void set_file(FILE* fp);
    static void set_level(int level);

    static void log(int level, int line, const char* file, const char* fmt, ...);

    static void fatal(int line, const char* file, const char* fmt, ...);
    static void error(int line, const char* file, const char* fmt, ...);
    static void warn(int line, const char* file, const char* fmt, ...);
    static void info(int line, const char* file, const char* fmt, ...);
    static void debug(int line, const char* file, const char* fmt, ...);

    static void fatal(const char* fmt, ...);
    static void error(const char* fmt, ...);
    static void warn(const char* fmt, ...);
    static void info(const char* fmt, ...);
    static void debug(const char* fmt, ...);
};


// NEW API
enum class LogLevel {
    FATAL = 0,
    ERROR = 1,
    WARN = 2,
    INFO = 3,
};

class LogEntry {
    MAKE_NOCOPY(LogEntry);
public:

    LogEntry(LogLevel level, const char* file, int line): level_(level), file_(file), line_(line) {}
    ~LogEntry();

    template <typename T>
    LogEntry& operator<< (const T& t) {
        content_ << t;
        return *this;
    }

    void operator() (const char* fmt, ...);

private:

    void do_log(const char* fmt, ...);
    void do_vlog(const char* fmt, va_list args);

    LogLevel level_;
    const char* file_;
    int line_;
    std::ostringstream content_;
};

} // namespace base

