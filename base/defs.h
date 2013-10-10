#pragma once

#include <stdio.h>
#include <assert.h>

/**
 * Use assert() when the test is only intended for debugging.
 * Use verify() when the test is crucial for both debug and release binary.
 */
#ifdef NDEBUG
#define verify(expr) do { if (!(expr)) { printf("  *** verify failed: %s at %s, line %d\n", #expr, __FILE__, __LINE__); abort(); } } while (0)
#else
#define verify(expr) assert(expr)
#endif

// return v if v in [lower, upper], otherwise lower or upper, which is closest to v
#define clamp(v, lower, upper) ((v) < (lower) ? (lower) : ((v) > (upper) ? (upper) : (v)))
