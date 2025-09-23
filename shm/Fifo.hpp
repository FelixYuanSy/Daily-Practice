#pragma once

#include <iostream>
#include <cstdio>
#include <string>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "common.hpp"

#define PATH "."
#define FILENAME "fifo"
class NamedFifo
{
public:
    NamedFifo(const std::string &path, const std::string &name) : _path(path), _name(name)
    {
        _fifoname = _path + "/" + _name;
        umask(0); // 用来设置0666权限
        int n = mkfifo(_fifoname.c_str(), 0666);
        if (n < 0)
        {
            ERR_EXIT("mkfifo");
        }
        else
        {
            std::cout << "mkfifo success!" << std::endl;
        }
    }
    ~NamedFifo()
    {
    }

private:
    std::string _path;
    std::string _name;
    std::string _fifoname;
};

class FileOper
{
public:
    FileOper(const std::string &path, const std::string &name) : _path(path), _name(name)
    {
        _fifoname = _path + "/" + _name;
    }
    void OpenForRead()
    {
        _fd = open(_fifoname.c_str(), O_RDONLY);
        if (_fd < 0)
        {
            ERR_EXIT("open");
        }
        std::cout << "open for read success!" << std::endl;
    }
    void OpenForWrite()
    {
        _fd = open(_fifoname.c_str(), O_WRONLY);
        if (_fd < 0)
        {
            ERR_EXIT("open");
        }
        std::cout << "open for write success!" << std::endl;
    }
    void Wakeup()
    {
        char c = 'c';
        int n = write(_fd, &c, 1);
        printf("尝试唤醒%d\n", n);
    }
    bool Wait()
    {
        char c;
        int number = read(_fd, &c, 1);
        if (number > 0)
        {
            printf("被唤醒,读取到字符:%c\n", c);
            return true;
        }
        return false;
    }
    void Close()
    {
        close(_fd);
    }
    ~FileOper()
    {
    }

private:
    std::string _path;
    std::string _name;
    std::string _fifoname;
    int _fd;
};