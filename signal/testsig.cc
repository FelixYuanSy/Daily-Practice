#include <iostream>
#include <unistd.h>
#include <signal.h>

void hanlderSig(int sig)
{
    std::cout << "收到一个信号" << sig << std::endl;
}

int main()
{
    signal(SIGINT, hanlderSig); // 信号自动传递给函数指针
    int cnt = 0;
    while (true)
    {
        std::cout << "HEllo world" << cnt++ << getpid() << std::endl;
        sleep(1);
    }
}