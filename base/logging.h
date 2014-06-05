#ifndef BASE_LOGGING_H_
#define BASE_LOGGING_H_

#include <sstream>

#include <stdarg.h>

#include "misc.h"

#define Log_debug(msg, ...) ::base::Log::debug(__LINE__, __FILE__, msg, ## __VA_ARGS__)

// NEW API

#define LOG_INFO ::base::INFO
#define LOG_WARN ::base::WARN
#define LOG_ERROR ::base::ERROR
#define LOG_FATAL ::base::FATAL

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

struct dont_create_your_own__ {};

class LogManager {
    MAKE_NOCOPY(LogManager);
public:
    LogManager(LogLevel level, dont_create_your_own__): level_(level) {}

    void operator() (const char* fmt, ...);
private:
    void vlog(const char* fmt, va_list args);
    LogLevel level_;
};

class LogHelper {
public:
    LogHelper(LogManager* lm) {
        rep_ = new rep;
        rep_->lm = lm;
    }
    LogHelper(const LogHelper& o): rep_(o.rep_) {
        rep_->ref++;
    }
    ~LogHelper();

    void operator= (const LogHelper&) = delete;

    std::ostream& stream() {
        return rep_->buf;
    }

private:

    struct rep {
        int ref = 1;
        LogManager* lm = nullptr;
        std::ostringstream buf;
    };
    rep* rep_ = nullptr;
};

template <class T>
LogHelper operator<< (LogManager& lm, const T& t) {
    LogHelper lh(&lm);
    lh << t;
    return lh;
}

template <class T>
LogHelper operator<< (LogHelper lh, const T& t) {
    lh.stream() << t;
    return lh;
}

extern LogManager INFO, WARN, ERROR, FATAL;

} // namespace base

#endif // BASE_LOGGING_H_

