#include "base/all.h"

using namespace base;

TEST(basetypes, rand_benchmark_single_thread) {
    Rand r;
    int n = 100 * 1000 * 1000;
    Timer t;
    t.start();
    for (int i = 0; i < n; i++) {
        r.next();
    }
    t.stop();
    Log::debug("Rand::next() %d ops in %lf sec (%.2lf nsec each)",
        n, t.elapsed(), t.elapsed() * 1e9 / n);
}

TEST(basetypes, rand_benchmark_multi_thread) {
    Rand r;
    int n_thread = 10;
    ThreadPool* tpool = new ThreadPool(n_thread);
    int n = 10 * 1000 * 1000;
    Timer t;
    t.start();
    Mutex m;
    for (int j = 0; j < n_thread; j++) {
        tpool->run_async([&r, n, &m] {
            for (int i = 0; i < n; i++) {
                ScopedLock sl(&m);
                r.next();
            }
        });
    }
    tpool->release();
    t.stop();
    Log::debug("with %d threads, Rand::next() %d ops in %lf sec (%.2lf nsec each)",
        n_thread, n * n_thread, t.elapsed(), t.elapsed() * 1e9 / n / n_thread);
}
