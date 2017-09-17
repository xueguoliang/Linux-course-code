#include "../header.h"

void sighandle(int v) // 
{
    if(v == SIGINT)
    {

    }
    else if(v == SIGCHLD)
    {
#if 0
        wait(NULL); // 收尸
        printf("sigchld\n");
        // 由于SIGCHLD是不可靠的，会导致多个子进程结束，但是只收了一个尸体
#endif

        // 完美收尸
        while(1)
        {
        //    pit_t pid = wait(NULL);
            pid_t pid = waitpid(-1, //这个参数指定要收尸体的pid，但是这里我们不知道哪个子进程挂了，-1表示任意子进程
                    NULL,
                    WNOHANG); // 非阻塞的版本
            if(pid == -1) break;
        }
    }
}

int main()
{
    signal(SIGINT, sighandle);
    signal(SIGCHLD, sighandle);

    pid_t pid = fork();

    if(pid == 0)
    {
        return 0;
    }

    while(1)
    {
        sleep(1);
    }
}
