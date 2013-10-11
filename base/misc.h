#pragma once

#include <inttypes.h>

namespace base {

inline uint64_t rdtsc() {
    uint32_t hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    return (((uint64_t) hi) << 32) | ((uint64_t) lo);
}

template<class T, class T1, class T2>
inline T clamp(const T& v, const T1& lower, const T2& upper) {
    if (v < lower) {
        return lower;
    }
    if (v > upper) {
        return upper;
    }
    return v;
}

// YYYY-MM-DD HH:MM:SS.mmm, 24 bytes required for now
#define TIME_NOW_STR_SIZE 24
void time_now_str(char* now);

} // namespace base
