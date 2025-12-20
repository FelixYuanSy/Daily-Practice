#pragma once
#include <cerrno>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "InetAddr.hpp"

const static uint16_t Default_Port = 8087;
const static int Default_Fd = -1;
const static int Default_Size = 1024;
class UdpServer {
private:
  int _socket_fd;
  uint16_t _port;

public:
  UdpServer(uint16_t port = Default_Port)
      : _port(port), _socket_fd(Default_Fd) {}

  void Init() {
    // 初始化创建Socket
    _socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (_socket_fd < 0) {
      std::cout << errno << std::endl;
      exit(errno);
    }
    std::cout << "Create UDP Socket Successed!\n";
    // 设置addr_in属性，绑定socket
    struct sockaddr_in local;
    bzero(&local, sizeof(local));
    local.sin_family = AF_INET;
    local.sin_port = htons(_port);
    local.sin_addr.s_addr = INADDR_ANY; // used to accept any incoming address，因为允许绑定具体ip所以让他自己分配

    int ret = bind(_socket_fd, (sockaddr *)&local, sizeof(local));
    if (ret != 0) {
      std::cout << "UDP Server Bind Failed!\n";
      std::cout << errno << std::endl;
      exit(errno);
    }
    std::cout << "UDP Bind Successed!\n";
  }

  /*服务器是一直运行的不退出
      什么方法不退出一直保持循环？while？
      用什么来接受数据 造buffer！defaultsize 为什么1024？

  */
  void start() {
    char buffer[Default_Size];
    while (1) {
      // 接收数据放入buffer中
      sockaddr_in peer;
      socklen_t lens = sizeof(peer);
      ssize_t recv_ret = recvfrom(_socket_fd, buffer, sizeof(buffer) - 1, 0,
                                  (struct sockaddr *)&peer, &lens);
      if (recv_ret > 0) {
        //解析收到链接的ip和port
        InetAddr addr(peer);
        //刷新一下buffer
        buffer[recv_ret]=0;
        std::cout<<"["<<addr.PrintDebug()<<"]#" << buffer <<std::endl;
        sendto(_socket_fd, buffer, strlen(buffer), 0, (struct sockaddr *)&peer, lens);
      }
    }
  }
  ~UdpServer(){}
};