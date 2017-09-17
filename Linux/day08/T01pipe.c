
#include "../header.h"

void sighandle(int v)
{
    printf("SIGPIPE\n");
}

int main()
{
    int fd[2];
    int ret = pipe(fd);

    signal(SIGPIPE, sighandle);

    close(fd[0]);

    // 导致SIGPIPE信号
    ret = write(fd[1], "buf", 3);
    if(ret < 0)
    {
        perror("write");
        printf("ERRNO: %d\n", errno);
    }

    if(ret != 0)
        return 0;

#if 0
    // 测试buffer长度
    int i;
    char buf[1];
    for(i=0;; ++i)
    {
        write(fd[1], buf, 1);
        printf("%d\n", i);
    }
#endif

#if 0
    pid_t pid = fork();
    if(pid == 0)
    {
        sleep(1);
        close(fd[0]);
        // write
        write(fd[1], "hello", 5);
    }
    else
    {
        close(fd[1]);
        // read
        char buf[10] = {0};
        read(fd[0], buf, sizeof(buf));  // 阻塞的
        printf("%s\n", buf);
    }
#endif
#if 0
    // 管道fd[2]创建完毕
    // fd[0]是读端，fd[1]写端
    write(fd[1], "hello", 5);

    char buf[1024];
    memset(buf, 0, sizeof(buf));
    read(fd[0], buf, sizeof(buf));

    printf("%s\n", buf);
#endif
}


#if 0
int main()
{
    int fd[2];
    pipe(fd);

    int readside = fd[0];

}
#endif




