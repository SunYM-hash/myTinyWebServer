#ifndef _THREADPOOL_H
#define _THREADPOOL_H

#include <cstdio>
#include <exception>
#include <list>
#include <pthread.h>
#include "../lock/lock.h"
#include <vector>
#include <algorithm>
using namespace  std;

template<typename T>
class threadPool {
public:
    threadPool(int maxThreadNum, int maxTaskNum) {
        thrPool.reserve(maxThreadNum + 2);
    };
    ~threadPool();
    bool addTask(T* request , int state); 
    bool addTaskP(T* request);
    
private:
    int m_maxThreadNum;
    int m_maxTaskNum;
   // int threadNum;   //线程池中的线程数
    vector<pthread_t> *m_thrPool;
    list<T*> m_taskQuene;
    locker m_taskLock;
    sem m_taskSem;

private:    
    void work();
    static void* threadRun(void *arg);   //创建线程时回调函数一定要声明静态
};

template<typename T>
threadPool<T>::threadPool(int maxThreadNum, int maxTaskNum){
    if(maxThreadNum <= 0 || maxTaskNum <= 0 ) {
        throw exception();
    }
    m_thrPool = new vector<pthread_t> (maxThreadNum);
    for (auto it = m_thrPool->begin(); it != m_thrPool.begin() + maxThreadNum; ++it) {
        if (pthread_create(it, nullptr, threadRun, this) != 0){
            delete m_thrPool;
            throw exception();
        }
        if (pthread_detach(*it) != 0) {
            delete m_thrPool;
            throw exception();
        }
    }
}


template<typename T>
void* threadPool<T>::threadRun(void* arg) {
    threadPool* pool = arg;
    pool->work();
    return pool;
}

template<typename T>
void threadPool<T>::work() {

}

#endif
