#include "base/all.h"

using namespace base;
using namespace std;

TEST(status_flag, basic_op) {
    StatusFlag sf;
    EXPECT_EQ(sf.get(), 0);
    EXPECT_EQ(sf.compare_and_swap(0, 4), 0);
    EXPECT_EQ(sf.get(), 4);
    EXPECT_EQ(sf.compare_and_swap(0, -3), 4);
    EXPECT_EQ(sf.get(), 4);
    EXPECT_EQ(sf.compare_and_swap(4, -3), 4);
    EXPECT_EQ(sf.get(), -3);
    StatusFlag sf2(sf);
    EXPECT_EQ(sf.get(), -3);
    EXPECT_EQ(sf2.get(), -3);
    EXPECT_EQ(sf.compare_and_swap(-3, 7), -3);
    sf2 = sf;
    EXPECT_EQ(sf.get(), 7);
    EXPECT_EQ(sf2.get(), 7);
    EXPECT_EQ(sf.compare_and_swap(7, 7), 7);
}

TEST(status_flag, bench) {
    StatusFlag sf(1987);
    EXPECT_EQ(sf.get(), 1987);
    Timer t;
    const int n = 100000000;
    t.start();
    for (int i = 0; i < n; i++) {
        EXPECT_EQ(sf.compare_and_swap(1987, 1987), 1987);
    }
    t.stop();
    Log::debug("doing %d compare_and_swap took %.2lf sec, op/s = %d",
        n, t.elapsed(), int(n / t.elapsed()));

    t.start();
    for (int i = 0; i < n; i++) {
        EXPECT_EQ(sf.get(), 1987);
    }
    t.stop();
    Log::debug("doing %d get took %.2lf sec, op/s = %d",
        n, t.elapsed(), int(n / t.elapsed()));
}
