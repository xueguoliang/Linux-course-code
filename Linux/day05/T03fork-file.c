
#include "../header.h"

int main()
{
    int fd = open("a.txt", O_WRONLY|O_CREAT, 0777);

    pid_t pid = fork();

    // 子进程也有fd（3）
    write(fd, "hello fork\n", 11);

    if(pid == 0)
    {
        int flag = fcntl(fd, F_GETFL);
        flag |= O_APPEND;
        fcntl(fd, F_SETFL, flag);
    }
    else
    {
        sleep(1); // 保证子进程先设置属性
        int flag = fcntl(fd, F_GETFL);
        if(flag & O_APPEND)
        {
            // 子进程修改文件描述符属性，会影响父进程的对应的文件描述符
            printf("has append\n");
        }
        else
        {
            printf("no append\n");
        }
    }


}
