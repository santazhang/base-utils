#include <stdio.h>

#include "unittest.h"

namespace base {

void TestCase::fail() {
    if (failures_ == 0) {
        printf("  fail\n");
    }
    failures_++;
}

TestMgr* TestMgr::instance_s = nullptr;

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
    // TODO only run selected tests
    int failures = 0;
    int passed = 0;
    for (auto t : tests_) {
        printf("%s/%s  ", t->group(), t->name());
        fflush(stdout);
        t->run();
        failures += t->failures();
        if (t->failures() == 0) {
            printf("  pass\n");
            passed++;
        }
    }
    printf("----\n%d/%lu passed, %d failures\n", passed, tests_.size(), failures);
}

} // namespace base
