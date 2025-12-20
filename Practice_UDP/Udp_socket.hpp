#include <arpa/inet.h>
#include <cstdint>
#include <iostream>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

class UdpSocket {
private:
  int _fd;

public:
  UdpSocket() : _fd(-1) {}

  bool Socket() {
    _fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (_fd < 0) {
      perror("socket");
      return false;
    }
    return true;
  }

  bool Bind(const std::string &ip,
            const uint16_t &port) // port是电脑自己分配可能会出错
  {
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    int ret = bind(_fd, (struct sockaddr *)&addr, sizeof(addr));
    if (ret < 0) {
      perror("bind:");
      return false;
    }
    return true;
  }

  bool RecvFrom(std::string *buf, std::string *ip = NULL,
                uint16_t *port = NULL) {
    char temp[1024 * 10] = {0};
    sockaddr_in peer;
    socklen_t len = sizeof(peer);
    ssize_t read_size = recvfrom(_fd, temp, sizeof(temp) - 1, 0,
                                 (struct sockaddr *)&peer, &len);
    if (read_size < 0) {
      perror("recvfrom:");
      return false;
    }
    buf->assign(
        temp,
        read_size); // assign可以让buf强行读取read_size个字符到buf遇到0不停止，如果用=遇到0会自动停止
    if (ip != NULL)
      *ip = inet_ntoa(peer.sin_addr);
    if (port != NULL)
      *port = ntohs(peer.sin_port);
    return true;
  }

  bool SendTo(const std::string &buf, const std::string &ip,
              const uint16_t &port) {
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip.c_str());

    ssize_t write_size = sendto(_fd, buf.data(), buf.size(), 0,
                                (struct sockaddr *)&addr, sizeof(addr));
    if (write_size < 0) {
      perror("sendto:");
      return false;
    }
    return true;
  }

  bool Close() {
    close(_fd);
    return true;
  }
};