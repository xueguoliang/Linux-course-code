
#include "../header.h"

#if 0
void sighandle(int v)
{
    printf("sigusr1\n");
}
#endif

void sig_handle(int v, siginfo_t* p, void* ptr)
{
    printf("signal\n");
}

int main()
{
    struct sigaction action;
    action.sa_handler = NULL;
    action.sa_sigaction = sig_handle;
    sigemptyset(&action.sa_mask);
    action.sa_flags = SA_RESTART;
    action.sa_restorer = NULL;

    sigaction(SIGUSR1, &action, NULL);
//    signal(SIGUSR1, sighandle);  调用sigaction实现的，默认具有SA_RESTART属性

//    getchar(); // 阻塞调用
    int fd = open("/dev/input/mice", O_RDONLY);
    if(fd < 0)
    {
        printf("error open mouse file\n");
        return 0;
    }
    char buf[8];
    printf("before read\n");
    int ret;
    ret = read(fd, buf, sizeof(buf)); // 阻塞的，恢复运行的条件是，鼠标移动或者点击
    if(ret == -1) // read error
    {
        if(errno == EINTR)
        {
            printf("errno is EINTR\n");
        }
        perror("read");
    }
    printf("end read\n");

    printf("...go\n");
}
