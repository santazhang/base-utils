#include <unistd.h>

#include <iostream>
#include <vector>
#include <string>

#include "base/all.h"

using namespace std;

namespace std {

template <typename T>
ostream& operator <<(ostream& os, const vector<T>& vec) {
    for (const auto& t : vec) {
        os << t << ' ';
    }
    return os;
}

}

static int heavy_slow_func() {
    ::sleep(1);
    return 1;
}

TEST(logging, basic_op) {
    vector<int> vec({1, 2, 3});
    INFO << "A simple log" << " at INFO level " << vector<int>({1, 2, 3});
    WARN << "A simple log" << " at WARN level";
    ERROR << "A simple log " << 12345 << " at ERROR level";
    FATAL << "A simple log " << 'x' << " at FATAL level";
    cout << vec << endl;
    ostringstream ostr;
    ostr << vec << endl;
    INFO << vector<int>({1, 2, 3});

    INFO("%s ~ %d", "hello", 1987);
    Log_info("%s ~ %d", "hello", 1987);

    INFO << "a simple log" << 123 << vec;
    WARN << vec << 234;

    WARN_IF(3 < 2) << heavy_slow_func();
    INFO_IF(1 > 0) << 2;
    INFO_IF(-1 > 0) << 2 << heavy_slow_func();
}

