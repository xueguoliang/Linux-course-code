
#include "../header.h"
#include <list>
using namespace std;

/* 任务参数 */
typedef struct Task
{
    int a;
} Task;

pthread_mutex_t mutex;

// 定义条件变量
pthread_cond_t cond;

class AutoLock
{
    public:
        AutoLock(pthread_mutex_t& mutex) : mutex(mutex){
            pthread_mutex_lock(&mutex);
        }
        ~AutoLock(){
            pthread_mutex_unlock(&mutex);
        }

        pthread_mutex_t& mutex;
};

list<Task*> tasks;

void* send_thread(void* ptr)
{
    int i= 0 ;
    while(1)
    {
        usleep(100*1000);

        Task* t = new Task;
        t->a = i++;

        {
            AutoLock lock(mutex);
            tasks.push_back(t);

            // 唤醒调用pthread_cond_wait的线程
            pthread_cond_signal(&cond);
        }
    }
}


void* recv_thread(void* ptr)
{
    Task* t;
    while(1)
    {
        // 等待条件变量
        pthread_mutex_lock(&mutex);
        // 要一把锁来配合
        pthread_cond_wait(&cond, &mutex);  
        /* 
         * 这些代码需要加锁
         * pthread_mutex_unlock(&mutex)
         * wait ....
         * pthread_mutex_lock(&mutex);
         * 还有一些代码需要加锁
         *
         * */
        pthread_mutex_unlock(&mutex);


        // cond_wait成功之后，应该把所有的任务都处理掉
        while(1)
        {
            AutoLock lock(mutex);
            if(tasks.size() == 0) break;
            t = *tasks.begin();
            tasks.pop_front();
            printf("t->task is %d\n", t->a);
            delete t;
        }

    }
}

int main()
{
    // 初始化条件变量
    pthread_cond_init(&cond, NULL);

    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&mutex, &attr);

    pthread_t tid_s, tid_r;

    pthread_create(&tid_s, NULL, send_thread, NULL);
    pthread_create(&tid_r, NULL, recv_thread, NULL);


    pthread_join(tid_s, NULL);
    pthread_join(tid_r, NULL);
}
