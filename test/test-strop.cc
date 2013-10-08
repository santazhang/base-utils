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

TEST(strop, strsplit) {
    std::vector<std::string> split = strsplit("hello world");
    EXPECT_EQ(split.size(), 2);
    EXPECT_EQ(split[0], "hello");
    EXPECT_EQ(split[1], "world");

    split = strsplit("hello  world");
    EXPECT_EQ(split.size(), 2);
    EXPECT_EQ(split[0], "hello");
    EXPECT_EQ(split[1], "world");

    split = strsplit(" hello  world");
    EXPECT_EQ(split.size(), 2);
    EXPECT_EQ(split[0], "hello");
    EXPECT_EQ(split[1], "world");

    split = strsplit("hello  world ");
    EXPECT_EQ(split.size(), 2);
    EXPECT_EQ(split[0], "hello");
    EXPECT_EQ(split[1], "world");

    split = strsplit("   hello  world   ");
    EXPECT_EQ(split.size(), 2);
    EXPECT_EQ(split[0], "hello");
    EXPECT_EQ(split[1], "world");

    split = strsplit("      ");
    EXPECT_EQ(split.size(), 0);

    split = strsplit("");
    EXPECT_EQ(split.size(), 0);

    split = strsplit(" ");
    EXPECT_EQ(split.size(), 0);

    split = strsplit("a");
    EXPECT_EQ(split.size(), 1);
    EXPECT_EQ(split[0], "a");

    split = strsplit("a ");
    EXPECT_EQ(split.size(), 1);
    EXPECT_EQ(split[0], "a");

    split = strsplit(" a");
    EXPECT_EQ(split.size(), 1);
    EXPECT_EQ(split[0], "a");

    split = strsplit(" a ");
    EXPECT_EQ(split.size(), 1);
    EXPECT_EQ(split[0], "a");
}
