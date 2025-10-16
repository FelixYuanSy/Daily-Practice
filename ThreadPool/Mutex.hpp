#pragma once
#include <pthread.h>
namespace MutexMoudle
{
    class Mutex
    {
    public:
        Mutex()
        {
            pthread_mutex_init(&_mutex, nullptr);
        }
        void Lock()
        {
            int n = pthread_mutex_lock(&_mutex);
            (void)n;
            // 如果不写n是忽略返回值,可能会出现未使用告警
        }
        void Unlock()
        {
            int n = pthread_mutex_unlock(&_mutex);
            (void)n;
        }
        ~Mutex()
        {
            pthread_mutex_destroy(&_mutex);
        }
        pthread_mutex_t *Getmutex()
        {
            return &_mutex;
        }

    private:
        pthread_mutex_t _mutex;
    };

    class LockGuard
    {
    public:
        LockGuard(Mutex &mutex) : _mutex(mutex)
        {
            _mutex.Lock();
        }
        ~LockGuard()
        {
            _mutex.Unlock();
        }

    private:
        Mutex &_mutex;
    };

}
