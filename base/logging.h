#ifndef BASE_LOGGING_H_
#define BASE_LOGGING_H_

#include <sstream>

#include <stdarg.h>

#include "misc.h"

#define Log_debug(msg, ...) ::base::Log::debug(__LINE__, __FILE__, msg, ## __VA_ARGS__)

// compatible code
#define Log_info(msg, ...) INFO(msg, ## __VA_ARGS__)
#define Log_warn(msg, ...) WARN(msg, ## __VA_ARGS__)
#define Log_error(msg, ...) ERROR(msg, ## __VA_ARGS__)
#define Log_fatal(msg, ...) FATAL(msg, ## __VA_ARGS__)

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
class LogManager;

enum class LogLevel {
    FATAL = 0,
    ERROR = 1,
    WARN = 2,
    INFO = 3,
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

    std::ostream* stream() {
        if (!rep_->disabled && rep_->buf == nullptr) {
            rep_->buf = new std::ostringstream;
        }
        return rep_->buf;
    }

    void disable() {
        rep_->disabled = true;
    }

    bool disabled() const {
        return rep_->disabled;
    }

private:

    struct rep {
        int ref = 1;
        bool disabled = false;
        LogManager* lm = nullptr;
        std::ostringstream* buf = nullptr;
    };
    rep* rep_ = nullptr;
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


template <class T>
LogHelper operator<< (LogManager& lm, const T& t) {
    LogHelper lh(&lm);
    lh << t;
    return lh;
}

template <class T>
LogHelper operator<< (LogHelper lh, const T& t) {
    if (!lh.disabled()) {
        *lh.stream() << t;
    }
    return lh;
}

} // namespace base

extern base::LogManager INFO, WARN, ERROR, FATAL;

#define INFO_IF(cond) \
    !(cond) ? 0 : INFO
#define WARN_IF(cond) \
    !(cond) ? 0 : WARN
#define ERROR_IF(cond) \
    !(cond) ? 0 : ERROR
#define FATAL_IF(cond) \
    !(cond) ? 0 : FATAL

#endif // BASE_LOGGING_H_

