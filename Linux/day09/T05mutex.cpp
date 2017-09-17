
#include "../header.h"

/*
 * 1. 同一个线程重复加锁
 * 2. 忘了解锁
 *
 * 要解决这两个问题，不要出现死锁
 * */



int a  = 0;
pthread_mutex_t mutex; // 保护了a变量

void add()
{
    pthread_mutex_lock(&mutex);
    pthread_mutex_lock(&mutex);
    a++;
    pthread_mutex_unlock(&mutex);
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
        pthread_mutex_lock(&mutex);
        a++;
        pthread_mutex_unlock(&mutex);
    }

    pthread_join(tid, NULL);
    printf("a is %d \n",a ); // 200000
}
