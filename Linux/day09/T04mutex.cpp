
#include "../header.h"

// 锁的粒度 大， 小
int a  = 0;
int b  = 0;
pthread_mutex_t mutex; // 保护了a变量

void* thread_func(void* ptr)
{
    int i;
    for(i=0; i<100000; ++i)
    {
        pthread_mutex_lock(&mutex);
        a++; // 既会影响别人，也是被保护的代码
        pthread_mutex_unlock(&mutex);

        pthread_mutex_lock(&mutex);
        b++; // 既会影响别人，也是被保护的代码
        pthread_mutex_unlock(&mutex);
    }
}

int main()
{
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&mutex1, NULL);

    pthread_t tid;
    pthread_create(&tid, NULL, thread_func, NULL);
    int i;
    for(i=0; i<100000; ++i)
    {
        pthread_mutex_lock(&mutex);
        a++;
        pthread_mutex_unlock(&mutex);

        pthread_mutex_lock(&mutex);
        b++;
        pthread_mutex_unlock(&mutex);

    }

    pthread_join(tid, NULL);
    printf("a is %d b is %d\n", a, b); // 200000
}
