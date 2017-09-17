
#include "../header.h"
#include <list>
using namespace std;

/* 任务参数 */
typedef struct Task
{
    int a;
} Task;

pthread_mutex_t mutex;
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
        }
    }
}


void* recv_thread(void* ptr)
{
    Task* t;
    while(1)
    {
        usleep(50*1000);
        {
            AutoLock lock(mutex);
            if(tasks.size() == 0) continue;
            t = *tasks.begin();
            tasks.pop_front();
        }

        printf("t->task is %d\n", t->a);
        delete t;
    }
}

int main()
{
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
