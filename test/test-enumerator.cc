#include "base/all.h"

using namespace base;

class IntRange: public Enumerator<int> {
public:
    IntRange(int last): cur_(0), last_(last) { }
    bool has_next() {
        return cur_ < last_;
    }
    int next() {
        int v = cur_;
        cur_++;
        return v;
    }
private:
    int cur_, last_;
};

TEST(enumer, int_range_enum) {
    IntRange rng(5);
    while (rng) {
        Log::debug("range enumerator: %d", rng());
    }
}
