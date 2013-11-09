#include <functional>
#include <sys/time.h>

#include "misc.h"
#include "threading.h"

using namespace std;

namespace base {

int CondVar::timed_wait(Mutex& m, const struct timespec& timeout) {
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    timespec abstime;
    abstime.tv_sec = tv.tv_sec + timeout.tv_sec;
    abstime.tv_nsec = tv.tv_usec * 1000 + timeout.tv_nsec;
    if (abstime.tv_nsec > 1000 * 1000 * 1000) {
        abstime.tv_nsec -= 1000 * 1000 * 1000;
        abstime.tv_sec += 1;
    }
    return pthread_cond_timedwait(&cv_, &m.m_, &abstime);
}

struct start_thread_pool_args {
    ThreadPool* thrpool;
    int id_in_pool;
};

void* ThreadPool::start_thread_pool(void* args) {
    start_thread_pool_args* t_args = (start_thread_pool_args *) args;
    t_args->thrpool->run_thread(t_args->id_in_pool);
    delete t_args;
    pthread_exit(NULL);
    return NULL;
}

ThreadPool::ThreadPool(int n /* =... */): n_(n), should_stop_(false) {
    th_ = new pthread_t[n_];
    q_ = new Queue<function<void()>*> [n_];

    for (int i = 0; i < n_; i++) {
        start_thread_pool_args* args = new start_thread_pool_args();
        args->thrpool = this;
        args->id_in_pool = i;
        Pthread_create(&th_[i], NULL, ThreadPool::start_thread_pool, args);
    }
}

ThreadPool::~ThreadPool() {
    should_stop_ = true;
    for (int i = 0; i < n_; i++) {
        q_[i].push(nullptr);  // death pill
    }
    for (int i = 0; i < n_; i++) {
        Pthread_join(th_[i], nullptr);
    }
    // check if there's left over jobs
    for (int i = 0; i < n_; i++) {
        function<void()>* job;
        while (q_[i].try_pop(&job)) {
            if (job != nullptr) {
                (*job)();
            }
        }
    }
    delete[] th_;
    delete[] q_;
}

int ThreadPool::run_async(const std::function<void()>& f) {
    if (should_stop_) {
        return EPERM;
    }
    int queue_id = round_robin_.next() % n_;
    q_[queue_id].push(new function<void()>(f));
    return 0;
}

void ThreadPool::run_thread(int id_in_pool) {
    struct timespec sleep_req;
    const int min_sleep_nsec = 1000;  // 1us
    const int max_sleep_nsec = 1000 * 1000;  // 1ms
    sleep_req.tv_nsec = 1000;  // 1us
    sleep_req.tv_sec = 0;
    int stage = 0;

    // randomized stealing order
    int* steal_order = new int[n_];
    for (int i = 0; i < n_; i++) {
        steal_order[i] = i;
    }
    Rand r;
    for (int i = 0; i < n_ - 1; i++) {
        int j = r.next(i, n_);
        if (j != i) {
            int t = steal_order[j];
            steal_order[j] = steal_order[i];
            steal_order[i] = t;
        }
    }

    // fallback stages: try_pop -> sleep -> try_pop -> steal -> pop
    // succeed: sleep - 1
    // failure: sleep + 10
    for (;;) {
        function<void()>* job = nullptr;

        switch(stage) {
        case 0:
        case 2:
            if (q_[id_in_pool].try_pop(&job)) {
                stage = 0;
            } else {
                stage++;
            }
            break;
        case 1:
            nanosleep(&sleep_req, NULL);
            stage++;
            break;
        case 3:
            for (int i = 0; i < n_; i++) {
                if (steal_order[i] != id_in_pool) {
                    // just don't steal other thread's death pill, otherwise they won't die
                    if (q_[steal_order[i]].try_pop_but_ignore(&job, nullptr)) {
                        stage = 0;
                        break;
                    }
                }
            }
            if (stage != 0) {
                stage++;
            }
            break;
        case 4:
            job = q_[id_in_pool].pop();
            stage = 0;
            break;
        }

        if (stage == 0) {
            if (job == nullptr) {
                break;
            }
            (*job)();
            delete job;
            sleep_req.tv_nsec = clamp(sleep_req.tv_nsec - 1000, min_sleep_nsec, max_sleep_nsec);
        } else {
            sleep_req.tv_nsec = clamp(sleep_req.tv_nsec + 10 * 1000, min_sleep_nsec, max_sleep_nsec);
        }
    }
    delete[] steal_order;
}

} // namespace base
