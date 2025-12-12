#pragma once
#include "InetAddr.hpp"
#include "Log.hpp"
#include "Mutex.hpp"
#include <string>
#include <functional>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
using func_t = std::function<std::string(const std::string &, InetAddr &)>;
const int defaultfd = -1;
using namespace LogModule;
class UdpServer
{
public:
    UdpServer(uint16_t port, func_t func) : _sockfd(defaultfd), _port(), _isrunning(false), _func(func)
    {
    }
    void Init()
    {
        //1. 创建套接字
        _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if(_sockfd < 0)
        {
            LOG(LogLevel::FATAL) << "socket create error";
            exit(1);
        }
        LOG(LogLevel::INFO) << "socket create success" << _sockfd;
        //2. 绑定地址信息
        struct sockaddr_in local;
        bzero(&local,sizeof(local));
        local.sin_family = AF_INET;
        local.sin_port = htons(_port);
        local.sin_addr.s_addr = INADDR_ANY;

        int n = bind(_sockfd,(struct sockaddr*)&local,sizeof(local));
        if(n < 0)
        {
            LOG(LogLevel::FATAL) << "bind error";
            exit(2);
        }
        LOG(LogLevel::INFO) << "bind success"<<_sockfd;

    }

    void Start()
    {
        _isrunning = true;
        while(_isrunning)
        {
            char buffer[1024];
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);
            //3. 接收数据
            ssize_t n = recvfrom(_sockfd,buffer,sizeof(buffer)-1,0,(struct sockaddr*)&peer,&len);
            if( n > 0)
            {
                //4. 处理数据(回调函数,把网络地址转为本地地址,让服务器直到是谁传进来的)
                InetAddr client(peer);
                buffer[n] = 0;
                std::string result = _func(buffer,client);
                //回调处理
            }
            
        }

    }

private:
    uint16_t _port;
    int _sockfd;
    bool _isrunning;
    func_t _func; // 回调函数
};