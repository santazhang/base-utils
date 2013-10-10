#include <functional>

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
    // Randomly select a thread for the job.
    int queue_id = rand_engine_() % n_;
    q_[queue_id].push(new function<void()>(f));
    return 0;
}

void ThreadPool::run_thread(int id_in_pool) {
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

    for (;;) {
        // fallback stages: try_pop -> steal -> pop
        function<void()>* job = nullptr;
        bool got_job = q_[id_in_pool].try_pop(&job);
        if (!got_job) {
            for (int i = 0; i < n_; i++) {
                if (steal_order[i] != id_in_pool) {
                    // just don't steal other thread's death pill, otherwise they won't die
                    if (q_[steal_order[i]].try_pop_but_ignore(&job, nullptr)) {
                        got_job = true;
                        break;
                    }
                }
            }
        }
        if (!got_job) {
            job = q_[id_in_pool].pop();
        }
        if (job == nullptr) {
            break;
        }
        (*job)();
        delete job;
    }
    delete[] steal_order;
}

} // namespace base
