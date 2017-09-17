#include "../header.h"
int main()
{
    int n = 4;
    int i;
    for(i=0; i<n; ++i)
    {
        pid_t pid = fork();
        if(pid == 0)
            break;
    }

    printf("hello world\n");
}
