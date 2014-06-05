#ifndef BASE_LOGGING_H_
#define BASE_LOGGING_H_

#include <stdarg.h>

#include <sstream>

#include "misc.h"

#ifdef NDEBUG
#define DEBUG_LOGGING_ON false
#else
#define DEBUG_LOGGING_ON true
#endif

#define LOG(verbosity) base::LogManager_INFO.new_log(__FILE__, __LINE__, verbosity)
#define LOG_IF(verbosity, cond) \
    !(cond) ? void(0) : base::hack_for_conditional_logging() & LOG(verbosity)
#define DLOG(verbosity) \
    !(DEBUG_LOGGING_ON) ? void(0) : base::hack_for_conditional_logging() & LOG(verbosity)
#define DLOG_IF(cond) \
    !((DEBUG_LOGGING_ON) && (cond)) ? void(0) : base::hack_for_conditional_logging() & LOG(verbosity)

#define INFO base::LogManager_INFO.new_log(__FILE__, __LINE__, 0)
#define INFO_IF(cond) \
    !(cond) ? void(0) : base::hack_for_conditional_logging() & INFO
#define DINFO \
    !(DEBUG_LOGGING_ON) ? void(0) : base::hack_for_conditional_logging() & INFO
#define DINFO_IF(cond) \
    !((DEBUG_LOGGING_ON) && (cond)) ? void(0) : base::hack_for_conditional_logging() & INFO

#define WARN base::LogManager_WARN.new_log(__FILE__, __LINE__, 0)
#define WARN_IF(cond) \
    !(cond) ? void(0) : base::hack_for_conditional_logging() & WARN
#define DWARN \
    !(DEBUG_LOGGING_ON) ? void(0) : base::hack_for_conditional_logging() & WARN
#define DWARN_IF(cond) \
    !((DEBUG_LOGGING_ON) && (cond)) ? void(0) : base::hack_for_conditional_logging() & WARN

#define ERROR base::LogManager_ERROR.new_log(__FILE__, __LINE__, 0)
#define ERROR_IF(cond) \
    !(cond) ? void(0) : base::hack_for_conditional_logging() & ERROR
#define DERROR \
    !(DEBUG_LOGGING_ON) ? void(0) : base::hack_for_conditional_logging() & ERROR
#define DERROR_IF(cond) \
    !((DEBUG_LOGGING_ON) && (cond)) ? void(0) : base::hack_for_conditional_logging() & ERROR

#define FATAL base::LogManager_FATAL.new_log(__FILE__, __LINE__, 0)
#define FATAL_IF(cond) \
    !(cond) ? void(0) : base::hack_for_conditional_logging() & FATAL
#define DFATAL \
    !(DEBUG_LOGGING_ON) ? void(0) : base::hack_for_conditional_logging() & FATAL
#define DFATAL_IF(cond) \
    !((DEBUG_LOGGING_ON) && (cond)) ? void(0) : base::hack_for_conditional_logging() & FATAL

// for compatibility
#define Log_debug LOG(1)
#define Log_info INFO
#define Log_warn WARN
#define Log_error ERROR
#define Log_fatal FATAL


namespace base {

class LogManager;

class Log {
    void operator= (const Log&) = delete;

public:

    Log(LogManager* lm, const char* file, int line, int verbosity) {
        rep_ = new rep;
        rep_->lm = lm;
        rep_->file = file;
        rep_->line = line;
        rep_->verbosity = verbosity;
    }

    Log(const Log& l): rep_(l.rep_) {
        rep_->ref++;
    }

    ~Log();

    std::ostream* stream() {
        if (rep_->buf == nullptr) {
            rep_->buf = new std::ostringstream;
        }
        return rep_->buf;
    }

    Log& operator() (const char* fmt, ...) {
        va_list va;
        va_start(va, fmt);
        vlog(fmt, va);
        va_end(va);
        return *this;
    }

    // for compatibility
    static void debug(const char* fmt, ...);
    static void info(const char* fmt, ...);
    static void warn(const char* fmt, ...);
    static void error(const char* fmt, ...);
    static void fatal(const char* fmt, ...);

private:

    void vlog(const char* fmt, va_list va);

    struct rep {
        int ref;
        LogManager* lm;
        std::ostringstream* buf;
        const char* file;
        int line;
        int verbosity;

        rep(): ref(1), lm(nullptr), buf(nullptr), file(nullptr), line(0), verbosity(0) {
        }
    };

    rep* rep_;
};


template <class T>
Log operator<< (Log log, const T& t) {
    *log.stream() << t;
    return log;
}

struct do_not_create_your_own {
};

class LogManager {
    MAKE_NOCOPY(LogManager);

public:


    LogManager(char _severity, struct do_not_create_your_own): severity_(_severity) {
    }

    Log new_log(const char* file, int line, int verbosity) {
        return Log(this, file, line, verbosity);
    }

    char severity() const {
        return severity_;
    }

private:

    char severity_;
};


struct hack_for_conditional_logging {

    // from google-glog library
    void operator& (Log) {
    }

};

extern LogManager LogManager_INFO, LogManager_WARN, LogManager_ERROR, LogManager_FATAL;

} // namespace base

#endif // BASE_LOGGING_H_
