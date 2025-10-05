// 封装信号量,用信号量来限制生产者和消费者无限的获取与产出
#include <semaphore.h>
namespace SemModule
{
    int defaultvalue = 1;
    class Sem
    {
    public:
        Sem(unsigned int sem_value = defaultvalue)
        {
            sem_init(&_sem, 0, sem_value);
        }
        void P()
        {
            int n = sem_wait(&_sem);
            (void)n;
        }

        void V()
        {
            int n = sem_post(&_sem);
            (void)n;
        }
        ~Sem()
        {
            sem_destroy(&_sem);
        }

    private:
        sem_t _sem;
    };
}