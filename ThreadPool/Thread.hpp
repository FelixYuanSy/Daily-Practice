#pragma once
// 自己造线程函数
#include <iostream>
#include <pthread.h>
#include <cstdint>
#include <functional>
namespace ThreadModule
{
    static uint32_t number = 1;
    class Thread
    {
        using func_t = std::function<void()>;

        static void *Routine(void *args)
        {
            Thread *self = static_cast<Thread *>(args);
            self->EnableRuning();
            if (self->_isdetach)
            {
                self->Detach();
            }
            pthread_setname_np(self->_tid, self->_name.c_str());
            self->_func();
            return nullptr;
        }

    public:
        Thread(func_t func)
            : _tid(0), _isdetach(false), _isrunning(false), ret(nullptr), _func(func)
        {
            _name = "thread-" + std::to_string(number++);
        }

        bool Start()
        {
            if (_isrunning)
                return false;
            int n = pthread_create(&_tid, nullptr, Routine, this); // 传入this可以让Routine函数里面调用类里的函数
            if (n != 0)
            {
                return false;
            }
            else
                return true;
        }

        void Detach()
        {
            if (_isdetach)
            {
                return;
            }
            if (_isrunning)
            {
                pthread_detach(_tid);
            }
            EnableDetach();
        }
        bool Stop()
        {
            if (_isrunning)
            {
                int n = pthread_cancel(_tid);
                if (n != 0)
                    return false;
                else
                {
                    _isrunning = false;
                    return true;
                }
            }
            else
            {
                return false;
            }
        }

        void Join()
        {
            if (_isdetach)
                return;

            int n = pthread_join(_tid, &ret);
            if (n != 0)
            {
            }
            else
            {
            }
        }

        // 用于让routine里面的对象来改变状态
        void EnableRuning()
        {
            _isrunning = true;
        }

        void EnableDetach()
        {
            _isdetach = true;
        }

    private:
        func_t _func;
        pthread_t _tid;
        std::string _name;
        bool _isdetach;
        bool _isrunning;
        void *ret;
    };
}