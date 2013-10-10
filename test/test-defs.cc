#include "base/all.h"

TEST(defs, clamp) {
    EXPECT_EQ(clamp(5, 5, 5), 5);
    EXPECT_EQ(clamp(5, 1, 2), 2);
    EXPECT_EQ(clamp(5, 1, 1), 1);
    EXPECT_EQ(clamp(5, 7, 8), 7);
    EXPECT_EQ(clamp(5, 8, 8), 8);
    EXPECT_EQ(clamp(5, 1, 8), 5);
    EXPECT_EQ(clamp(1.0, 1, 8), 1);
    EXPECT_EQ(clamp(1.0, 2.0, 8), 2.0);
    EXPECT_EQ(clamp(1.0, 0.4, 0.8), 0.8);
}
