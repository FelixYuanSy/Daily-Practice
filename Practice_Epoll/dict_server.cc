#include "UDPServer.hpp"
#include <unordered_map>
#include <utility>

std::unordered_map<std::string, std::string> _dict;

void Translate(const std::string &req, std::string *resp) {
  auto it = _dict.find(req);
  if (it == _dict.end()) {
    *resp = "未查到该词";
    return;
  }
  *resp = it->second;
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Usage ./dict_server ip port \n");
    return 1;
  }

  _dict.insert(std::make_pair("hello", "你好"));
  _dict.insert(std::make_pair("computer", "电脑"));
  _dict.insert(std::make_pair("headphone", "耳机"));
  _dict.insert(std::make_pair("c++", "神的语言"));
  _dict.insert(std::make_pair("Felix", "Oh God"));

  UdpServer server;
  server.Start(argv[1], atoi(argv[2]), Translate);
  return 0;
}
