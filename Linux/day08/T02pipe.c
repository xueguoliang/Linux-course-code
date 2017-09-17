#include "../header.h"
int main()
{
    int fd[2];
    pipe(fd);

    // "ps aux"
    // "grep a.out"
    //
    pid_t pid = fork();
    if(pid == 0)
    {
        // 第一个子进程
        // 重定向 stdout--> fd[1]
        dup2(fd[1], 1);

        // 关闭多余的文件描述符
        close(fd[0]);
        close(fd[1]);
        execlp("ps", "ps", "aux", NULL);
        return 0;
    }

    pid = fork();
    if(pid == 0)
    {
        dup2(fd[0], 0); // 重定向 grep "hello world"

        close(fd[0]);
        close(fd[1]);
        execlp("grep", "grep", "a.out", NULL); // fgets(...) scanf(...)
    }

    close(fd[0]);
    close(fd[1]);

    wait(NULL);
    wait(NULL);

    return 0;
}
