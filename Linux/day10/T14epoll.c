#include "../header.h"
int main()
{
    int epollfd = epoll_create(512);
    
    int fd_key = open("/dev/input/event1", O_RDONLY|O_NONBLOCK);
    int fd_mice = open("/dev/input/mice", O_RDONLY|O_NONBLOCK);

    struct epoll_event ev;
    ev.events = EPOLLIN; // 监控可读

    ev.data.fd = fd_key; 
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd_key,  &ev);

    ev.data.fd = fd_mice;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd_mice, &ev);

    struct epoll_event evout[2];
    char buf[1024];
    while(1)
    {
        int ret = epoll_wait(epollfd, evout, 2, 5000);
        if(ret == 0) continue; // 超时
        if(ret < 0 && errno == EINTR) continue; // 被信号打断
        if(ret < 0) break; // 错误发生了

        // ret > 0情况
        int i;
        for(i=0; i<ret; ++i)
        {
            int fd = evout[i].data.fd;
            if(read(fd, buf, sizeof(buf)) < 0)
            {
                // close自动将它从epoll中移除
                close(fd); 
            }

            if(fd == fd_key) printf("键盘有消息\n");
            else if(fd == fd_mice) printf("鼠标有消息\n");
        }
    }
}
