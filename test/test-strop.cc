#include "base/all.h"

using namespace base;

TEST(strop, startswith) {
    EXPECT_TRUE(startswith("", ""));
    EXPECT_FALSE(startswith("", "a"));
}

TEST(strop, endswith) {
    EXPECT_TRUE(endswith("", ""));
    EXPECT_FALSE(endswith("", "a"));
}
