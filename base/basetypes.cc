#include <sys/time.h>

#include "basetypes.h"

namespace base {

void Timer::start() {
    gettimeofday(&begin_, nullptr);
}

void Timer::stop() {
    gettimeofday(&end_, nullptr);
}

void Timer::reset() {
    begin_.tv_sec = 0;
    begin_.tv_usec = 0;
    end_.tv_sec = 0;
    end_.tv_usec = 0;
}

double Timer::elapsed() const {
    return end_.tv_sec - begin_.tv_sec + (end_.tv_usec - begin_.tv_usec) / 1000000.0;
}

Rand::Rand() {
    struct timeval now;
    gettimeofday(&now, NULL);
    rand_.seed(now.tv_sec + now.tv_usec + (long long) pthread_self() + (long long) this);
}

} // namespace base
