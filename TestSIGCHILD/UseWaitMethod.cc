#include <iostream>
#include <sys/wait.h>

void WaitAll()
{
    while (true)
    {
        pid_t n = waitpid(-1, nullptr, 0);
        if (n == 0)
        {
            break;
        }
        else if (n < 0)
        {
            std::cout << "waitpid error" << std::endl;
            break;
        }
    }
}

int main()
{
    WaitAll();
    for (int i = 0; i < 10; ++i)
    {
        pid_t id = fork();
        if (id == 0)
        {
            sleep(3);
            std::cout << "child process " << getpid() << " exit" << std::endl;
            exit(3);
        }
    }
    while (true)
    {
        std::cout << "parent process " << getpid() << " wait all child process" << std::endl;
        sleep(1);
    }
    return 0;
}