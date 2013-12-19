#include <stdio.h>
#include <limits>

#include "base/all.h"

using namespace base;
using namespace std;

static void print_binary(const void* vbuf, int len) {
    char* buf = (char *) vbuf;
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < 8; j++) {
            uint8_t u = buf[i];
            printf("%d", (u >> (7 - j)) & 0x1);
        }
        printf(" ");
    }
    printf("\n");
}

TEST(sparseint, dump_load_i32) {
    char buf[9];
    const i32 values[] = {0, 1, -1, -64, 63, 64, -65, -8192, 8191, -8193, 8192, -1048576, 1048575,
                    1048576, -1048577, -134217728, 134217727,
                    134217728, -134217729,
                    numeric_limits<i32>::max(), numeric_limits<i32>::min()};
    const i32 bs[] = {1, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3,
                    4, 4, 4, 4,
                    5, 5, 5, 5};
    for (int i = 0; i < arraysize(values); i++) {
        const i32 v = values[i];
        memset(buf, 0, 9);
        int bsize = SparseInt::dump(v, buf);
        print_binary(buf, bsize);
        print_binary(&v, 4);
        const i32 u = SparseInt::load_i32(buf);
        Log::debug("%ld -> bsize=%ld -> %ld", (long) v, bsize, (long) u);
        EXPECT_EQ(v, u);
        EXPECT_EQ(bsize, bs[i]);
    }
}
