// 创建多线程并集中等待关闭多线程
#include <iostream>
#include <pthread.h>
#include <vector>
#include <unistd.h>

void *routine(void *args)
{
    sleep(1);
    std::string name = static_cast<const char *>(args);

    int cnt = 5;
    while (cnt--)
    {
        std::cout << "new线程名字" << name << std::endl;
        sleep(1);
    }
    return nullptr;
}
int main()
{
    std::vector<pthread_t> tids;
    for (int i = 0; i < 10; i++)
    {
        pthread_t tid;
        char *id = new char[64];
        snprintf(id, 64, "thread-%d", i);
        // char id[64];//这样写在新线程sleep完之后,主线程已经把id写完9了,新线程拿到的是同一个数组地址,所以都是9
        // snprintf(id, sizeof(id), "thread-%d", i);
        int n = pthread_create(&tid, NULL, routine, id);
        if (n == 0)
        {
            tids.push_back(tid);
        }
        else
            continue;
    }

    for (int i = 0; i < 10; i++)
    {
        int n = pthread_join(tids[i], nullptr);
        if (n == 0)
        {
            std::cout << "等待线程成功" << std::endl;
        }
    }
    return 0;
}
// 打印时候会产生线程间打印互相抢位置,怎么解决?