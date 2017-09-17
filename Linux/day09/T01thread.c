#include "../header.h"

void foo()
{
    // 当前运行这个函数的线程
    pthread_t tid = pthread_self();
    while(1)
    {
        // 在线程中调用exit会导致整个进程退出
        // 线程退出应该调用pthread_exit
        //
        // exit(0);
        //pthread_exit(0);

        usleep(1000*10);
        printf("in thread = %d\n", (int)tid);
    }

}

void* thread_func(void* ptr)
{
    foo();
}

int main()
{
    // 如果在主线程中调用pthread_exit，主线程退出，子线程不退出
    // pthread_exit(0);

    // 创建线程
    pthread_t tid; // 传出的线程id
    pthread_create(&tid, NULL, thread_func, NULL);

    pthread_t tid1;
    pthread_create(&tid1, NULL, thread_func, NULL);

    if(pthread_equal(tid, tid1) == 0)
    {
        printf("same thread\n");
    }

    foo();
}
