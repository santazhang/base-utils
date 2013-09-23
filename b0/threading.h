#pragma once

#include <pthread.h>

#include "basetypes.h"

#define Pthread_spin_init(l, pshared) verify(pthread_spin_init(l, pshared) == 0)
#define Pthread_spin_lock(l) verify(pthread_spin_lock(l) == 0)
#define Pthread_spin_unlock(l) verify(pthread_spin_unlock(l) == 0)
#define Pthread_spin_destroy(l) verify(pthread_spin_destroy(l) == 0)
#define Pthread_mutex_init(m, attr) verify(pthread_mutex_init(m, attr) == 0)
#define Pthread_mutex_lock(m) verify(pthread_mutex_lock(m) == 0)
#define Pthread_mutex_unlock(m) verify(pthread_mutex_unlock(m) == 0)
#define Pthread_mutex_destroy(m) verify(pthread_mutex_destroy(m) == 0)
#define Pthread_cond_init(c, attr) verify(pthread_cond_init(c, attr) == 0)
#define Pthread_cond_destroy(c) verify(pthread_cond_destroy(c) == 0)
#define Pthread_cond_signal(c) verify(pthread_cond_signal(c) == 0)
#define Pthread_cond_broadcast(c) verify(pthread_cond_broadcast(c) == 0)
#define Pthread_cond_wait(c, m) verify(pthread_cond_wait(c, m) == 0)
#define Pthread_create(th, attr, func, arg) verify(pthread_create(th, attr, func, arg) == 0)
#define Pthread_join(th, attr) verify(pthread_join(th, attr) == 0)

namespace b0 {

class Lockable: public NoCopy {
public:
    virtual void lock() = 0;
    virtual void unlock() = 0;
};

class SpinLock: public Lockable {
public:
    SpinLock(): locked_(0) { }
    void lock() {
        if (!lock_state() && !__sync_lock_test_and_set(&locked_, 1)) {
            return;
        }
        int wait = 1000;
        while ((wait-- > 0) && lock_state()) {
            // spin for a short while
        }
        struct timespec t;
        t.tv_sec = 0;
        t.tv_nsec = 50000;
        while (__sync_lock_test_and_set(&locked_, 1)) {
            nanosleep(&t, NULL);
        }
    }
    void unlock() {
        __sync_lock_release(&locked_);
    }
private:
    int locked_ __attribute__((aligned (64)));
    int lock_state() const volatile {
        return locked_;
    }
};

class Mutex: public Lockable {
public:
    Mutex() {
        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);
        Pthread_mutex_init(&m_, &attr);
    }
    ~Mutex() { Pthread_mutex_destroy(&m_); }
    void lock() { Pthread_mutex_lock(&m_); }
    void unlock() { Pthread_mutex_unlock(&m_); }
private:
    friend class CondVar;
    pthread_mutex_t m_;
};

// choice between spinlock & mutex:
// * when n_thread > n_core, use mutex
// * on virtual machines, use mutex


class ScopedLock: public NoCopy {
public:
    explicit ScopedLock(Lockable* lock): m_(lock) { m_->lock(); }
    explicit ScopedLock(Lockable& lock): m_(&lock) { m_->lock(); }
    ~ScopedLock() { m_->unlock(); }
private:
    Lockable* m_;
};

class CondVar: public NoCopy {
public:
    CondVar() { Pthread_cond_init(&cv_, NULL); }
    ~CondVar() { Pthread_cond_destroy(&cv_); }

    void wait(Mutex* mutex) { Pthread_cond_wait(&cv_, &(mutex->m_)); }
    void signal() { Pthread_cond_signal(&cv_); }
    void bcast() { Pthread_cond_broadcast(&cv_); }

    int timed_wait(Mutex* mutex, const struct timespec* timeout) {
        return pthread_cond_timedwait(&cv_, &(mutex->m_), timeout);
    }
private:
    pthread_cond_t cv_;
};

} // namespace b0
