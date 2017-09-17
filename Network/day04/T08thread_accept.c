
#include "../network.h"


void handle(int server)
{
    while(1)
    {
        int newfd = accept(server, NULL, NULL);
        printf("newfd = %02d, pid = 0x%x\n", newfd, (int)getpid());
        close(newfd);
    }
}

int main()
{
    int server = myServer(9988, "0.0.0.0");

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
}

