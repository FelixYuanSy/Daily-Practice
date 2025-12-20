#include <arpa/inet.h>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>



void Usage(const std::string &process)
{
    std::cout<<"Usage: "<<process<< "server_ip server_port\n";
}
//执行方式 ./client server_ip server_port
int main(int argc,char* argv[])
{
    //如果执行格式错误会打印提醒
    if(argc != 3)
    {
        Usage(argv[0]);
        return 1;
    }

    //读取ip，port
    std::string serverip = argv[1];
    uint16_t serverport = std::stoi(argv[2]);  

    int sock = socket(AF_INET,SOCK_DGRAM,0);
    if(sock < 0)
    {
        //使用strerror 字符化描述错误码
        std::cerr<<"client socket error: "<<strerror(errno)<<std::endl;
        return 2;
    }
    //填充server信息
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(serverport);    //知道为什么要argv了吧
    server.sin_addr.s_addr = inet_addr(serverip.c_str());

    //client在写时候可以省却bind，因为client在首次发送时候会自己绑定端口

    while(1)
    {
        //定义需要发送的数据
        std::string inbuffer;
        std::cout<<"Please Enter your Command# ";
        std::getline(std::cin,inbuffer);

        //发送给服务器
        ssize_t n = sendto(sock,inbuffer.c_str(),inbuffer.size(),0,(struct sockaddr*)&server,sizeof(server));

        if(n > 0)
        {
            //定义buffer收消息
            char buffer[1024];
            struct sockaddr_in temp;
            socklen_t len = sizeof(temp);
            ssize_t rcv = recvfrom(sock, buffer, sizeof(buffer)-1, 0, (struct sockaddr*)&temp, &len);
            if(rcv > 0)
            {
                buffer[rcv] = 0;
                std::cout<<"server echo#"<<buffer<<std::endl;
            }
            else {
                break;
            }
        }
        else {
            break;
        }

    }
    close(sock);
    return 0;

    

}