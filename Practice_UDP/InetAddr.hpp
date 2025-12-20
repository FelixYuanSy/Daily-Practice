#pragma once

#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>

class InetAddr {
private:
  struct sockaddr_in _addr;
  std::string _ip;
  uint16_t _port;

public:
  InetAddr(struct sockaddr_in &addr) : _addr(addr) {
    _port = ntohs(_addr.sin_port);
    _ip = inet_ntoa(_addr.sin_addr);
  }
  ~InetAddr(){}
  std::string Ip(){return _ip;}
  uint16_t Port(){return _port;}
  std::string PrintDebug()
  {
    std::string info = _ip;
    info += " : ";
    info += std::to_string(_port);
    return info;
  }

};