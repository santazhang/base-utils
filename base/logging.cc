#include <stdarg.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>

#include "misc.h"
#include "threading.h"
#include "logging.h"

namespace {

// Serialize printing logs to prevent mangled output in multithread applications.
//
// Using recursive mutex, because it might be possible that a Log operation
// is interrupted by a signal, and then inside the signal handler, Log is
// called again. Using recursive mutex prevents the thread from deadlocking
// itself. See issue 11 on github.com/santazhang/simple-rpc
#ifdef __APPLE__
static pthread_mutex_t log_mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER;
#else
static pthread_mutex_t log_mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
#endif // __APPLE__

static char log_indicator[] = { 'F', 'E', 'W', 'I', 'D' };

} // namespace


namespace base {

int Log::level_s = Log::DEBUG;
FILE* Log::fp_s = stdout;


void Log::set_level(int level) {
    Pthread_mutex_lock(&log_mutex);
    level_s = level;
    Pthread_mutex_unlock(&log_mutex);
}

void Log::set_file(FILE* fp) {
    verify(fp != nullptr);
    Pthread_mutex_lock(&log_mutex);
    fp_s = fp;
    Pthread_mutex_unlock(&log_mutex);
}

static const char* basename(const char* fpath) {
    if (fpath == nullptr) {
        return nullptr;
    }
    const char sep = '/';
    int len = strlen(fpath);
    int idx = len - 1;
    while (idx > 0) {
        if (fpath[idx - 1] == sep) {
            break;
        }
        idx--;
    }
    verify(idx >= 0 && idx < len);
    return &fpath[idx];
}

void Log::log_v(int level, int line, const char* file, const char* fmt, va_list args) {
    static char indicator[] = { 'F', 'E', 'W', 'I', 'D' };
    assert(level <= Log::DEBUG);
    if (level <= level_s) {
        const char* filebase = basename(file);
        char now_str[TIME_NOW_STR_SIZE];
        Pthread_mutex_lock(&log_mutex);
        time_now_str(now_str);
        fprintf(fp_s, "%c ", indicator[level]);
        if (filebase != nullptr) {
            fprintf(fp_s, "[%s:%d] ", filebase, line);
        }
        fprintf(fp_s, "%s | ", now_str);
        vfprintf(fp_s, fmt, args);
        fprintf(fp_s, "\n");
        Pthread_mutex_unlock(&log_mutex);
        fflush(fp_s);
    }
}

void Log::log(int level, int line, const char* file, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log_v(level, line, file, fmt, args);
    va_end(args);
}

void Log::fatal(int line, const char* file, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log_v(Log::FATAL, line, file, fmt, args);
    va_end(args);
    abort();
}

void Log::error(int line, const char* file, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log_v(Log::ERROR, line, file, fmt, args);
    va_end(args);
}

void Log::warn(int line, const char* file, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log_v(Log::WARN, line, file, fmt, args);
    va_end(args);
}

void Log::info(int line, const char* file, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log_v(Log::INFO, line, file, fmt, args);
    va_end(args);
}

void Log::debug(int line, const char* file, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log_v(Log::DEBUG, line, file, fmt, args);
    va_end(args);
}


void Log::fatal(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log_v(Log::FATAL, 0, nullptr, fmt, args);
    va_end(args);
    abort();
}

void Log::error(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log_v(Log::ERROR, 0, nullptr, fmt, args);
    va_end(args);
}

void Log::warn(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log_v(Log::WARN, 0, nullptr, fmt, args);
    va_end(args);
}

void Log::info(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log_v(Log::INFO, 0, nullptr, fmt, args);
    va_end(args);
}

void Log::debug(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log_v(Log::DEBUG, 0, nullptr, fmt, args);
    va_end(args);
}


// NEW API
void LogManager::operator() (const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vlog(fmt, args);
    va_end(args);
}

void LogManager::vlog(const char* fmt, va_list args) {
    char now_str[TIME_NOW_STR_SIZE];
    time_now_str(now_str);

    Pthread_mutex_lock(&log_mutex);
    printf("%c %s | ", log_indicator[int(level_)], now_str);
    vprintf(fmt, args);
    printf("\n");
    Pthread_mutex_unlock(&log_mutex);

    if (level_ == LogLevel::FATAL) {
        print_stack_trace();
    }
}

LogHelper::~LogHelper() {
    rep_->ref--;
    if (rep_->ref == 0) {
        (*rep_->lm)("%s", rep_->buf.str().c_str());
        delete rep_;
    }
}

LogManager INFO(LogLevel::INFO, dont_create_your_own__());
LogManager WARN(LogLevel::WARN, dont_create_your_own__());
LogManager ERROR(LogLevel::ERROR, dont_create_your_own__());
LogManager FATAL(LogLevel::FATAL, dont_create_your_own__());

} // namespace base

