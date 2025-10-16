#pragma once
#include "Mutex.hpp"
#include <iostream>

namespace LogModule
{
    using namespace MutexMoudle;
    const std::string gsep = "\r\n";
    class LogStrategy
    {
        public:
        ~LogStrategy() = default;
        virtual void SyncLog(const std::string &message) = 0;
    };
    class ConsoleLogStrategy : public LogStrategy
    {
    public:
        ConsoleLogStrategy()
        {
        }
        void SyncLog(const std::string &message) override
        {
            LockGuard lockgurad(_mutex);
            std::cout << message << gsep;
        }
        ~ConsoleLogStrategy()
        {
        }

    private:
        Mutex _mutex;
    };
}