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
    LOG_INFO << "A simple log" << " at INFO level " << vector<int>({1, 2, 3});
    LOG_WARN << "A simple log" << " at WARN level";
    LOG_ERROR << "A simple log " << 12345 << " at ERROR level";
    LOG_FATAL << "A simple log " << 'x' << " at FATAL level";
    cout << vec << endl;
    ostringstream ostr;
    ostr << vec << endl;
    LOG_INFO << vector<int>({1, 2, 3});

    LOG_INFO("%s ~ %d", "hello", 1987);
    Log_info("%s ~ %d", "hello", 1987);

    LOG_INFO << "a simple log" << 123 << vec;
    LOG_WARN << vec << 234;

    LOG_WARN_IF(3 < 2) << heavy_slow_func() << "this will not display";
    DLOG_INFO_IF(1 > 0) << 2 << " this will show";
    LOG_INFO_IF(-1 > 0) << 2 << heavy_slow_func() << " this will not show";

    DLOG_LEVEL(1) << "level 1";
    LOG_LEVEL_IF(2, 4 < 5) << "level " << 2;
    DLOG_INFO << "hi" << heavy_slow_func();
    DLOG_INFO("%d", heavy_slow_func());
}

