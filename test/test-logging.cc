#include <iostream>
#include <vector>
#include <string>

#include "base/all.h"

using namespace base;
using namespace std;

template <typename T>
ostream& operator <<(ostream& os, const vector<T>& vec) {
    for (const auto& t : vec) {
        os << t << ' ';
    }
    return os;
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
}

