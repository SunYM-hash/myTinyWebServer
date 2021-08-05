#ifndef _LOCK_H
#define _LOCK_H

#include <pthread.h>
#include <exception>
#include <semaphore.h>
using namespace std;


class locker
{
public:
    locker() {
        if (! pthread_mutex_init(&mutex, nullptr))
            throw exception();
    }
    ~locker() {
        if (! pthread_mutex_destroy(&mutex))
            throw exception();
    }
    bool lock() {
        return pthread_mutex_lock(&mutex);
    }
    bool unlock() {
        return pthread_mutex_unlock(&mutex);
    }
    pthread_mutex_t* get() {
        return &mutex;
    }

private:
    pthread_mutex_t mutex;
};


class sem {
public:
    sem() {
        sem_init(&tSem, 0, 0);
    }
    sem(int value) {
        sem_init(&tSem, 0, value);
    }
    ~sem() {
        sem_destroy(&tSem);
    }
    bool wait() {
        return sem_wait(&tSem) == 0;
    }
    bool post() {
        return sem_post(&tSem) == 0;
    }

private:
    sem_t tSem;
};


class cond {
public:
    cond() {
        pthread_cond_init(&tCond, nullptr);
    }
    ~cond() {
        pthread_cond_destroy(&tCond);
    }
    bool wait(pthread_mutex_t *mutex) {
        return pthread_cond_wait(&tCond, mutex) == 0;
    }
    bool timewait(pthread_mutex_t *mutex, struct timespec t) {
        return pthread_cond_timedwait(&tCond, mutex, &t) == 0;
    }
    bool signal() {
        return pthread_cond_signal(&tCond) == 0;
    }
    bool broadcast() {
        return pthread_cond_broadcast(&tCond) == 0;
    }

private:
    pthread_cond_t tCond;

};

#endif