#include <stdio.h>

#include "unittest.h"
#include "logging.h"

namespace base {

void TestCase::fail() {
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

int TestMgr::run(int argc, char* argv[]) {
    // TODO only run selected tests
    int failures = 0;
    int passed = 0;
    for (auto t : tests_) {
        Log::info("--> starting test: %s/%s", t->group(), t->name());
        // TODO run tests in parallel, and in subprocesses
        t->run();
        failures += t->failures();
        if (t->failures() == 0) {
            Log::info("<-- passed test: %s/%s", t->group(), t->name());
            passed++;
        } else {
            Log::error("X-- failed test: %s/%s", t->group(), t->name());
        }
    }
    Log::info("%d/%lu passed, %d failures\n", passed, tests_.size(), failures);
    return failures;
}

} // namespace base
