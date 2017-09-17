
#include "../network.h"

int server;

void handle(int server)
{
    while(1)
    {
        int newfd = accept(server, NULL, NULL);
        printf("newfd = %02d, pid = 0x%x\n", newfd, (int)getpid());
        close(newfd);
    }
}

void* thread_func(void* ptr)
{
    while(1)
    {
        int newfd = accept(server, NULL, NULL);
        printf("newfd=%02d\n, tid=%d\n", newfd, (int)pthread_self());
        close(newfd);
    }
    return NULL;
}

int main()
{
    server = myServer(9988, "0.0.0.0");

    pthread_t tid1, tid2, tid3, tid4, tid5;
    pthread_create(&tid1, NULL, thread_func, NULL);   
    pthread_create(&tid2, NULL, thread_func, NULL);   
    pthread_create(&tid3, NULL, thread_func, NULL);   
    pthread_create(&tid4, NULL, thread_func, NULL);   
    pthread_create(&tid5, NULL, thread_func, NULL);   

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);
    pthread_join(tid4, NULL);
    pthread_join(tid5, NULL);

#if 0
    int n = 5;
    int i;
    for(i=0; i<n; ++i)
    {
        pid_t pid = fork();
        if(pid == 0)
        {
            handle(server);
            return 0;
        }
    }

    for(i=0; i<5; ++i)
    {
        wait(NULL);
    }
#endif
}

