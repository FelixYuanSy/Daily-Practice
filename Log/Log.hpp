#pragma once
#include "Mutex.hpp"
#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <unistd.h>
namespace LogModule
{
    const std::string gsep = "\r\n"; // 用来给linux和windows公用换行
    using namespace MutexMoudle;
    class LogStrategy
    {
    public:
        ~LogStrategy() = default;
        virtual void SyncLog(const std::string &message) = 0;
    };
    class ConsoleLogStrategy : public LogStrategy
    {
    public:
        ConsoleLogStrategy() {}
        void SyncLog(const std::string &message) override
        {
            LockGuard lockguard(_mutex);
            std::cout << message << gsep;
        }

    private:
        Mutex _mutex;
    };
    const std::string defaultpath = "./log";
    const std::string defaultfile = "my.log";
    class FileLogStrategy : public LogStrategy
    {
    public:
        FileLogStrategy(const std::string &path = defaultpath, const std::string &filename = defaultfile)
            : _path(path), _filename(filename)
        {
            LockGuard lockguard(_mutex);
            if (std::filesystem::exists(_path))
            {
                return;
            }
            try
            {
                std::filesystem::create_directories(_path);
            }
            catch (const std::filesystem::filesystem_error &e)
            {
                std::cerr << e.what() << '\n';
            }
        }
        void SyncLog(const std::string &message) override
        {
            LockGuard lockguard(_mutex);
            std::string filename = _path + (_path.back() == '/' ? "" : "/") + _filename;
            std::ofstream out(filename, std::ios::app);
            if (!out.is_open())
            {
                return;
            }
            out << message << gsep;
            out.close();
        }

    private:
        Mutex _mutex;
        std::string _path;
        std::string _filename;
    };

    enum class LogLevel
    {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        FATAL
    };

    std::string Level2Str(LogLevel level)
    {
        switch (level)
        {
        case LogLevel::DEBUG:
            return "DEBUG";
        case LogLevel::INFO:
            return "INFO";
        case LogLevel::WARNING:
            return "WARNING";
        case LogLevel::ERROR:
            return "ERROR";
        case LogLevel::FATAL:
            return "FATAL";
        default:
            return "UNKNOWN";
        }
    }

    std::string GetTime()
    {
        time_t curr = time(nullptr);
        struct tm curr_tm;
        localtime_r(&curr, &curr_tm);
        char timebuffer[128];
        snprintf(timebuffer, sizeof(timebuffer), "%4d-%02d-%02d %02d:%02d:%02d",
                 curr_tm.tm_year + 1900,
                 curr_tm.tm_mon + 1,
                 curr_tm.tm_mday,
                 curr_tm.tm_hour,
                 curr_tm.tm_min,
                 curr_tm.tm_sec);
        return timebuffer;
    }

    class Logger
    {
    public:
        Logger()
        {
            EnableConsoleStrategy();
        }
        void EnableConsoleStrategy()
        {
            _fflush_strategy = std::make_unique<ConsoleLogStrategy>();
        }
        void EnableFileStrategy()
        {
            _fflush_strategy = std::make_unique<FileLogStrategy>();
        }

        class LogMessage
        {
        public:
            LogMessage(LogLevel &level, std::string &src_name, int line, Logger &logger)
                : _current_time(GetTime()), _level(level), _pid(getpid()), _line_number(line), _src_name(src_name), _logger(logger)
            {
                std::stringstream ss;
                ss << "[" << _current_time << "] "
                   << "[" << _pid << "] "
                   << "[" << Level2Str(_level) << "] "
                   << "[" << _src_name << "] "
                   << "[" << _line_number << "] "
                   << "-";
                _loginfo = ss.str();
            }
            // LogMessage(INFO,my.log,10,this*) << "hello" << "123"
            // 需要解决<<拼接问题
            template <typename T>
            LogMessage &operator<<(const T &info)
            {
                std::stringstream ss;
                ss << info;
                _loginfo += ss.str();
                return *this; // 返回类型LogMessage,如果为void , 那么就不能继续拼接 <<, 链式调用
            }

            ~LogMessage()
            {
                if (_logger._fflush_strategy)
                {
                    _logger._fflush_strategy->SyncLog(_loginfo);
                }
            }

        private:
            std::string _current_time;
            pid_t _pid;
            LogLevel _level;
            std::string _src_name;
            int _line_number;
            std::string _loginfo;
            Logger &_logger;
        };

        // 简化LogMessage调用
        LogMessage operator()(LogLevel level, std::string name, int line)
        {
            return LogMessage(level, name, line, *this);
        }

    private:
        std::unique_ptr<LogStrategy> _fflush_strategy;
    };

    // 创建全局使用对象
    Logger logger;

#define LOG(level) logger(level, __FILE__, _LINE_)
#define Enable_Console_Log_Strategy() logger.EnableConsoleStrategy()
#define Enable_File_Log_Strategy() logger.EnableFileStrategy()

}
#endif