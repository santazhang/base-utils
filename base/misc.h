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


// This template function declaration is used in defining arraysize.
// Note that the function doesn't need an implementation, as we only
// use its type.
template <typename T, size_t N>
char (&ArraySizeHelper(T (&array)[N]))[N];

// That gcc wants both of these prototypes seems mysterious. VC, for
// its part, can't decide which to use (another mystery). Matching of
// template overloads: the final frontier.
#ifndef COMPILER_MSVC
template <typename T, size_t N>
char (&ArraySizeHelper(const T (&array)[N]))[N];
#endif

#define arraysize(array) (sizeof(base::ArraySizeHelper(array)))

} // namespace base
