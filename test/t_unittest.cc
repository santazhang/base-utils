#include "b0/b0all.h"

using namespace b0;

TEST(demo, expect_eq) {
    EXPECT_NEQ(2, 2);
}

TEST(demo, expect_neq) {
    EXPECT_EQ(1 + 2, 2);
}

int main(int argc, char* argv[]) {
    Log_info("This shall be done!");
    RUN_TESTS(argc, argv);
}
