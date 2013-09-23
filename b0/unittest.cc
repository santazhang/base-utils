#include "unittest.h"
#include <stdio.h>

namespace b0 {

TestMgr* TestMgr::instance_s = nullptr;

TestMgr::TestMgr() {
    
}

TestMgr* TestMgr::instance() {
    if (instance_s == nullptr) {
        instance_s = new TestMgr;
    }
    return instance_s;
}

TestCase* TestMgr::reg(TestCase* t) {
    tests_.push_back(t);
    return t;
}

void TestMgr::run(int argc, char* argv[]) {
    for (auto t : tests_) {
        t->run();
    }
}

} // namespace b0
