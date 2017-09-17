
#include "../header.h"

int main()
{
    // 爷爷进程是独立一个组
    pid_t pid = fork();
    if(pid == 0)
    {
        // 父亲进程独立
//        setpgid(getpid(), getpid());

        pid = fork();
        if(pid == 0)
        {
            // 孙子进程在父亲进程组中
        }
    }

    int ch = getchar();
    printf("%c\n", ch);
}
