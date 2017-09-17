#include "../header.h"
int main()
{
    int fd_key = open("/dev/input/event1", O_RDONLY);
    int fd_mice = open("/dev/input/mice", O_RDONLY);

    // 默认select的fd_set能容纳1024个文件描述符
    // 他使用了位域(bitmap)的技术

    /* 
     * int select(
     * int nfds,        // 指集合中最大的文件描述符+1
     * fd_set *readfds, // readfds是被监控的可读文件描述符集合
     * fd_set *writefds,       // writefds是监控的可写文件描述符集合
     * fd_set *exceptfds,       // 异常文件描述符集合
     * struct timeval *timeout); // 超时时间, timeout->tv_sec = 0, timeout->tv_usec = 0, 
     *                                 如果timeout = NULL，表示永久等待
     *
     * select函数是一个阻塞的调用，运行的条件是集合中，有文件有消息
     * select运行的第二个条件是超时
     *
     * select的返回值，表示有多少文件有消息
     * 返回0表示超时
     * 返回-1表示失败，如果错误码是EINTR，表示select的阻塞被中断打断了
    */

    while(1)
    {
        int nfds = fd_key;
        if(nfds < fd_mice) nfds = fd_mice;

        fd_set readfds;
        struct timeval timeout;

        // 怎么把fd_key, fd_mice --> readfds
        FD_ZERO(&readfds);
        FD_SET(fd_key, &readfds);
        FD_SET(fd_mice, &readfds);

        timeout.tv_sec = 5; //5秒
        timeout.tv_usec = 0;

        // 检查fd_key和fd_mice，究竟哪个文件有数据
        int ret = select(nfds+1, &readfds, NULL, NULL, &timeout);

        // 说明select的运行条件成立
        if(ret == 0)
        {
            continue;
        }
        // 说明被信号打断
        if(ret == -1 && errno == EINTR)
        {
            continue;
        }
        if(ret == -1)
        {
            // 严重错误
            break;
        }

        char buf[1024];
        if(FD_ISSET(fd_key, &readfds))
        {
            printf("key message\n");
            read(fd_key, buf, sizeof(buf));
            // 键盘有消息
        }
        if(FD_ISSET(fd_mice, &readfds))
        {
            read(fd_mice, buf, sizeof(buf));
            printf("mice message\n");
            // 鼠标有消息
        }
    }
}
