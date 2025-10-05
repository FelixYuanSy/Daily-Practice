#include "Sem.hpp"
#include "Mutex.hpp"
static const int gcap = 5;
using namespace SemModule;
using namespace MutexMoudle;

template <typename T>
class RingQueue
{
public:
    RingQueue(int cap = gcap)
        : _cap(cap), _rq(cap), _blank_sem(cap), _p_step(0), _data_sem(0), _c_step(0)
    {
    }

    void Equeue(const T &in)
    {
        _blank_sem.P();
        {
            LockGuard(_pmutex);
            _rq[_p_step] = in;
            ++_p_step;
            _p_step %= _cap;
        }
        _data_sem.V();
    }

    void Pop(T *out)
    {
        // 消费者
        // 1. 申请信号量，数据信号量
        _data_sem.P();
        {
            LockGuard lockguard(_cmutex);
            // 2. 消费
            *out = _rq[_c_step];
            // 3. 更新下标
            ++_c_step;
            // 4. 维持环形特性
            _c_step %= _cap;
        }
        _blank_sem.V();
    }

private:
    int _cap;
    std::vector<T> _rq;
    Sem _blank_sem;
    int _p_step;
    Sem _data_sem;
    int _c_step;

    Mutex _cmutex;
    Mutex _pmutex;
};
