#include <functional>

#include "misc.h"
#include "threading.h"

using namespace std;

namespace base {

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
        Pthread_join(th_[i], nullptr);
    }
    // need to check if there's jobs not executed yet in queues
    for (int i = 0; i < n_; i++) {
        function<void()>* job;
        while (q_[i].try_pop(&job)) {
            (*job)();
        }
    }
    delete[] th_;
    delete[] q_;
}

int ThreadPool::run_async(const std::function<void()>& f) {
    if (should_stop_) {
        return EPERM;
    }
    // Randomly select a thread for the job.
    int queue_id = rand_engine_() % n_;
    q_[queue_id].push(new function<void()>(f));
    return 0;
}

void ThreadPool::run_thread(int id_in_pool) {
    struct timespec sleep_req;
    sleep_req.tv_nsec = 200 * 1000;
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

    // sleep: 1us ~ 1000us
    // fallback stages: pop -> sleep -> pop -> steal -> sleep
    // succeed: sleep - 1
    // failure: sleep + 10
    while (!should_stop_) {
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
        case 4:
            nanosleep(&sleep_req, NULL);
            stage++;
            break;
        case 3:
            for (int i = 0; i < n_; i++) {
                if (steal_order[i] != id_in_pool) {
                    if (q_[steal_order[i]].try_pop(&job)) {
                        stage = 0;
                    }
                }
            }
            if (stage != 0) {
                stage++;
            }
            break;
        }

        if (stage == 0) {
            (*job)();
            delete job;
            sleep_req.tv_nsec = clamp(sleep_req.tv_nsec - 1000, 1000, 1000 * 1000);
        } else {
            sleep_req.tv_nsec = clamp(sleep_req.tv_nsec + 10 * 1000, 1000, 1000 * 1000);
        }

        if (stage >= 5) {
            stage = 0;
        }
    }
    delete[] steal_order;
}

} // namespace base
