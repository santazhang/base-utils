#include <unistd.h>

#include "base/all.h"

using namespace base;

TEST(threading, threadpool) {
    int n_thread = 16;
    ThreadPool* tpool = new ThreadPool(n_thread);
    int n_ops = 1000 * 1000;
    Timer t;
    t.start();
    for (int i = 0; i < n_ops; i++) {
        tpool->run_async([] {});
    }
    tpool->release();
    t.stop();
    Log::info("ThreadPool(%d) took %lf sec to execute %d nop jobs, that's %.2lf usec each",
        n_thread, t.elapsed(), n_ops, t.elapsed() * 1e6 / n_ops);
}

TEST(threading, empty_threadpool) {
    int n_thread = 32;
    int n_sec = 10;
    ThreadPool* tpool = new ThreadPool(n_thread);
    Log::info("starting ThreadPool(%d) with no jobs, wait for %d seconds. CPU usage should be low.", n_thread, n_sec);
    sleep(n_sec);
    tpool->release();
}
