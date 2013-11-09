#include <string.h>

#include "base/all.h"

using namespace base;

TEST(threading, condvar) {
    CondVar cv;
    Mutex m;
    Log::info("will wait mutex for 2 seconds");
    timespec tm;
    tm.tv_sec = 2;
    tm.tv_nsec = 0;
    m.lock();
    int ret = cv.timed_wait(m, tm);
    m.unlock();
    EXPECT_EQ(ret, ETIMEDOUT);
    Log::info("done waiting, return = %d, err = %s", ret, strerror(ret));

    Log::info("will wait mutex for 2 seconds (not holding locks, incorrect!)");
    ret = cv.timed_wait(m, tm);
    EXPECT_EQ(ret, EPERM);
    Log::info("done waiting, return = %d, err = %s", ret, strerror(ret));
}

