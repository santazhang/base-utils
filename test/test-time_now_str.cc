#include "base/all.h"

using namespace base;

TEST(misc, time_now_str) {
    int n = 1000 * 1000;
    char now[TIME_NOW_STR_SIZE];
    Timer t;
    t.start();
    for (int i = 0; i < n; i++) {
        time_now_str(now);
    }
    t.stop();
    EXPECT_EQ(strlen(now), TIME_NOW_STR_SIZE - 1);
    Log::info("time_now_str() %d times takes %lf sec, that's %.2lf usec per op",
        n, t.elapsed(), t.elapsed() * 1e6 / n);
}
