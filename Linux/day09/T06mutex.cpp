
#include "../header.h"


int a  = 0;
pthread_mutex_t mutex; // 保护了a变量

class AutoLock
{
public:
    AutoLock(pthread_mutex_t& m) : mutex(m)
    {
        pthread_mutex_lock(&mutex);
    }
    ~AutoLock()
    {
        pthread_mutex_unlock(&mutex);
    }

    pthread_mutex_t& mutex;
};

void add()
{
    AutoLock lock(mutex);
    if(a >= 100)
        return;
    a++;
}

void* thread_func(void* ptr)
{
    int i;
    for(i=0; i<100000; ++i)
    {
        add();
    }
}

int main()
{
    // 设置循环锁属性
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&mutex, &attr);

    pthread_t tid;
    pthread_create(&tid, NULL, thread_func, NULL);
    int i;
    for(i=0; i<100000; ++i)
    {
        AutoLock lock(mutex);
        a++;
    }

    pthread_join(tid, NULL);
    printf("a is %d \n",a ); // 200000
}
