
#include "../header.h"

void sighandle(int v)
{
    printf("sighandle\n");
}

int main()
{
    signal(SIGINT, sighandle);

    pid_t pid = fork();
    if(pid == 0)
    {
        sleep(1);

        kill(0, SIGINT); // 会不会发给父进程

        while(1)
        {
            sleep(1);
        }
    }
    else
    {
        while(1)
        {
            sleep(1);
        }
    }
}
