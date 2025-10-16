#pragma once
#include "Mutex.hpp"
#include "Thread.hpp"
#include "Cond.hpp"

namespace ThreadPoolModule
{
    using namespace MutexMoudle;
    using namespace CondModule;
    using namespace ThreadModule;

    static const int gnum = 5;
    template <typename T>
    class ThreadPool
    {
        public: 
        ThreadPool(int num = gnum):_num(num)
        {
            for(int i = 0; i < _num; i++)
            {
                _threads.emplace_back(
                    [this](){
                        HandlerTask();
                    }
                )
            }
        }
        void Start()
        {

        }
        void HandlerTask()
        {
            char name[128];
            pthread_getname_np(pthread_self(),name,sizeof(name));
            while(true)
            {
                T t;
                {
                    LockGuard lockguard(_mutex);
                    while(_task_q.empty())
                    {
                        _cond.Wait(_mutex);
                    }
                    t = _task_q.front();
                    _task_q.pop();
                }
                t();
            }
        }
        private:
        std::vector<Thread> _threads;//存线程
        int num;//创建线程数量
        std::queue<T> _task_q;
        Mutex _mutex;
        Cond _cond;
    };
}