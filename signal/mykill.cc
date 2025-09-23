#include <iostream>
#include <signal.h>

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cout << "输入格式错误" << std::endl;
        return 1;
    }
    int signum = std::stoi(argv[1]);
    pid_t target = std::stoi(argv[2]);

    int n = kill(target, signum);
    if (n == 0)
    {
        std::cout << "send " << signum << " to" << target << std::endl;
    }
    return 0;
}