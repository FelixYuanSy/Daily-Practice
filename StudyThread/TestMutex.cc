// 用模拟抢票来进行加锁
#include <iostream>
#include <pthread.h>
#include <unistd.h>

// 采用全局锁
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int ticket = 1000;
void *routine(void *arg)
{
    // 把参数转换为char
    char *id = (char *)arg;
    while (1)
    {
        pthread_mutex_lock(&lock);
        if (ticket > 0)
        {
            usleep(1000);
            printf("%s gets ticket: %d\n", id, ticket);
            ticket--;
            pthread_mutex_unlock(&lock);
        }
        else
        {
            pthread_mutex_unlock(&lock);
            break;
        }
    }
    return nullptr;
}
int main()
{
    pthread_t t1, t2, t3; // 用三个线程来模拟
    pthread_create(&t1, nullptr, routine, (void *)"thread1");
    pthread_create(&t2, nullptr, routine, (void *)"thread2");
    pthread_create(&t3, nullptr, routine, (void *)"thread3");

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
}