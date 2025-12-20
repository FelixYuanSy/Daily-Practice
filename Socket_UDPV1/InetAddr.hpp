#pragma once
#include <string>
#include <cstdint>
#include <netinet/in.h>
#include <arpa/inet.h>

class InetAddr
{
public:
    InetAddr(struct sockaddr_in &addr) : _addr(addr)
    {
        _port = ntohs(_addr.sin_port);
        _ip = inet_ntoa(_addr.sin_addr);
    }
    uint16_t GetPort()
    {
        return _port;
    }
    std::string GetIp()
    {
        return _ip;
    }

private:
    uint16_t _port;
    std::string _ip;
    struct sockaddr_in _addr;
};