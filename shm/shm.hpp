#pragma once
#include <iostream>
#include <sys/shm.h>
#include <sys/ipc.h>
#include "common.hpp"
const int defaultid = -1;
const int gsize = 4096;
const std::string pathname = ".";
const int projid = 0x66;

#define USER "User"
#define CREATER "Creater"


class Shm
{

private:
    void CreateHelper(int flag)
    {
        _key = ftok(pathname.c_str(), projid);
        if (_key < 0)
        {
            ERR_EXIT("ftok");
        }
        printf("key: 0x%x\n", _key);
        _shmid = shmget(_key, _size, flag);
        if (_shmid < 0)
        {
            ERR_EXIT("shmget");
        }
        printf("shmid: %d\n", _shmid);
    }

public:
    Shm(const std::string &pathname, int projid, const std::string &usertype)
        : _shmid(defaultid), _size(gsize), _start_mem(nullptr), _usertype(usertype)
    {
        _key = ftok(pathname.c_str(), projid);
        if (_key < 0)
        {
            ERR_EXIT("ftok");
        }
        if (usertype == CREATER)
        {
            Create();
        }
        else if (usertype == USER)
        {
            Get();
        }
        else
        {
            ERR_EXIT("usertype error");
        }
        Attach();
    }

    void Create()
    {
        CreateHelper(IPC_CREAT | IPC_EXCL | 0666);
    }
    void Get()
    {
        CreateHelper(IPC_CREAT);
    }
    void Destory()
    {
        int n = shmctl(_shmid, IPC_RMID, nullptr);
        if (n > 0)
        {
            printf("shmctl delete shm: %d success", n);
        }
        else
        {
            ERR_EXIT("shmctl");
        }
    }
    // 挂接到进程的虚拟地址空间
    void Attach()
    {
        _start_mem = shmat(_shmid, nullptr, 0666);
        if ((long long)_start_mem < 0)
        {
            ERR_EXIT("shmat");
        }
        printf("attach success\n");
    }
    // 查看虚拟地址
    void *Virtualaddr()
    {
        printf("start_mem: %p\n", _start_mem);
        return _start_mem;
    }
    ~Shm()
    {
        if (_usertype == CREATER)
        {
            Destory();
        }
    }

private:
    int _shmid;
    int _size;
    key_t _key;
    void *_start_mem;
    std::string _usertype;
};