#pragma once
#include <pthread.h>
#include "Mutex.hpp"

using namespace MutexMoudle;
namespace CondModule
{
    class Cond
    {
        Cond()
        {
            pthread_cond_init(&_cond, nullptr);
        }
        void Signal()
        {
            int n = pthread_cond_signal(&_cond);
            (void)n;
        }
        void Wait(Mutex &mutex)
        {
            int n = pthread_cond_wait(&_cond, mutex.Getmutex());
            (void)n;
        }
        void Broadcast()
        {
            int n = pthread_cond_broadcast(&_cond);
            (void)n;
        }
        ~Cond()
        {
            pthread_cond_destroy(&_cond);
        }

    private:
        pthread_cond_t _cond;
    };
}
