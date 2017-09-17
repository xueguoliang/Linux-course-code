
// 线程之间无父子关系
//
#include "../header.h"

pthread_t tid1;

int a = 50;

void* thread_func2(void* ptr)
{
    pthread_join(tid1, NULL);
}

void* thread_func(void* ptr)
{
    pthread_t tid2;
    pthread_create(&tid2, NULL, thread_func2, NULL);

    sleep(1);
    // 线程结束
    printf("a:%d\n", a);
}

int main()
{

    pthread_create(&tid1, NULL, thread_func, NULL);
    a = 100;

    void* ret;
    // join会阻塞等待，一直到线程退出
    // 退出之后，回收资源，得到返回值
    pthread_join(tid1, &ret);
}
