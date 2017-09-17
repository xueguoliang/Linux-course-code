
#include "../header.h"
#include <list>
using namespace std;

/* 任务参数 */
typedef struct Task
{
    int a;
} Task;


list<Task*>* tasks_send;
list<Task*>* tasks_recv;

int fd[2];
int f_pipe[2];

void* send_thread(void* ptr)
{
    int needSwap = 0;
    int i= 0 ;
    while(1)
    {
        char buf;
        // 如果没有数据，那么read会阻塞
        read(fd[0], &buf, 1);

        if(buf == 1)
        {
            Task* t = new Task;
            t->a = i++;
            tasks_send->push_back(t);

            if(needSwap)
            {
                // 交换队列
                list<Task*>* tmp = tasks_send;
                tasks_send = tasks_recv;
                tasks_recv = tmp;
                // 通知消费者有数据
                write(f_pipe[1], &buf, 1);
                needSwap = 0;
            }
        }
        else if(buf==2)
        {
            if(tasks_send->size() > 0)
            {
                // 交换队列
                list<Task*>* tmp = tasks_send;
                tasks_send = tasks_recv;
                tasks_recv = tmp;
                // 通知消费者有数据
                write(f_pipe[1], &buf, 1);
                needSwap = 0;
            }
            else
            {
                needSwap = 1;
            }
        }
    }
}


void* recv_thread(void* ptr)
{
    Task* t;
    while(1)
    {
        if(tasks_recv->size() == 0)
        {
            char buf = 2;
            write(fd[1], &buf, 1);
            read(f_pipe[0], &buf, 1);
            continue;
        }
        t = *tasks_recv->begin();
        tasks_recv->pop_front();

        printf("t->task is %d\n", t->a);
        delete t;
    }
}

int main()
{
#if 0
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&mutex, &attr);
#endif
    tasks_send = new list<Task*>;
    tasks_recv = new list<Task*>;

    socketpair(AF_UNIX, SOCK_STREAM, 0, fd);
    pipe(f_pipe);

    pthread_t tid_s, tid_r;

    pthread_create(&tid_s, NULL, send_thread, NULL);
    pthread_create(&tid_r, NULL, recv_thread, NULL);

    while(1)
    {
        char buf = 1;
        getchar();
        write(fd[1], &buf, 1);
    }


    pthread_join(tid_s, NULL);
    pthread_join(tid_r, NULL);
}
