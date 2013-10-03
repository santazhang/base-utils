#pragma once

#include <random>
#include <inttypes.h>

#include "defs.h"

namespace base {

typedef int32_t i32;
typedef int64_t i64;

class NoCopy {
    NoCopy(const NoCopy&);
    const NoCopy& operator =(const NoCopy&);
protected:
    NoCopy() {
    }
    virtual ~NoCopy() = 0;
};
inline NoCopy::~NoCopy() {
}

/**
 * Note: All sub class of RefCounted *MUST* have protected destructor!
 * This prevents accidentally deleting the object.
 * You are only allowed to cleanup with release() call.
 * This is thread safe.
 */
class RefCounted: public NoCopy {
    volatile int refcnt_;
protected:
    virtual ~RefCounted() {}
public:
    RefCounted(): refcnt_(1) {
    }
    int ref_count() {
        return refcnt_;
    }
    RefCounted* ref_copy() {
        __sync_add_and_fetch(&refcnt_, 1);
        return this;
    }
    void release() {
        int r = __sync_sub_and_fetch(&refcnt_, 1);
        verify(r >= 0);
        if (r == 0) {
            delete this;
        }
    }
};

class Counter: public NoCopy {
    volatile i64 next_;
public:
    Counter(i64 start = 0) : next_(start) { }
    i64 peek_next() const {
        return next_;
    }
    i64 next(i64 step = 1) {
        return __sync_fetch_and_add(&next_, step);
    }
    void reset(i64 start = 0) {
        next_ = start;
    }
};

class Timer {
public:
    Timer() { reset(); }
    void start();
    void stop();
    void reset();
    double elapsed() const;
private:
    struct timeval begin_;
    struct timeval end_;
};

class Rand: public NoCopy {
    std::mt19937 rand_;
public:
    Rand();
    std::mt19937::result_type next() {
        return rand_();
    }
    std::mt19937::result_type operator() () {
        return rand_();
    }
};

} // namespace base