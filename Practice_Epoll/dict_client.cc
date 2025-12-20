#include <iostream>
#include "UdpClient.hpp"

int main(int argc,char *argv[])
{
    if(argc!=3)
    {
        printf("Usage ./dict_server ip port \n");
    }
    UdpClient client(argv[1],atoi(argv[2]));
    while(1)
    {
        std::string word;
        std::cout << "请输入您要查的词语： ";
        std::cin>>word;
        if(!std::cin)
        {
            std::cout<<"GOOD BYE\n";
            break;
        }
        client.SendTo(word);
        std::string result;
        client.RecvFrom(&result);
        std::cout<<word <<" means: "<<result<<std::endl;
    }
    return 0;
}