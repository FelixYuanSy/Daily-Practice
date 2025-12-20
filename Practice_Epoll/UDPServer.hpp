#pragma once
#include <cassert>
#include <cerrno>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <functional>
#include <iostream>
#include <netinet/in.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "InetAddr.hpp"
#include "Udp_socket.hpp"
const static uint16_t Default_Port = 8087;
const static int Default_Fd = -1;
const static int Default_Size = 1024;
typedef std::function<void (const std::string &,std::string * resp)> Handler;
class UdpServer {
private:
  UdpSocket _sock;

public:
  UdpServer()
  {
    assert(_sock.Socket());
    
  }
  bool Start(std::string &ip,const uint16_t &port, Handler handler)
  {
    bool ret = _sock.Bind(ip, port);
    if(!ret)
    {
      return false;
    }
    while(1)
    {
      std::string req;
      std::string remote_ip;
      uint16_t remote_port = 0;
      bool ret = _sock.RecvFrom(&req,&remote_ip,&remote_port);
      if(!ret)
      {
        continue;
      }
      std::string resp;
      handler(req,&resp);
      _sock.SendTo(resp, remote_ip, remote_port);
      printf("[%s : %d] req: %s, resp: %s\n",remote_ip.c_str(),remote_port,req.c_str(),resp.c_str());

    }
    _sock.Close();
    return true;
  }

  ~UdpServer(){}
};