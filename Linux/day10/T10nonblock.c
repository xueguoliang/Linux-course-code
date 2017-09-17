#include "../header.h"

void set_nonblock(int fd)
{
    int flag = fcntl(fd, F_GETFL);
    flag |= O_NONBLOCK;
    fcntl(fd, F_SETFL, flag);
}

int main()
{
    int fd = open("/dev/input/mice", O_RDONLY); //|O_NONBLOCK);

    set_nonblock(fd);

    char buf[8];
    while(1)
    {
        int ret = read(fd, buf, sizeof(buf));
        if(ret < 0)
        {
            // 使用非阻塞的read去读取设备数据时，如果
            // 设备没有数据，那么返回-1，错误码是EAGAIN
            if(errno == EAGAIN)
            {
                printf("EAGAIN\n");
            }
           // perror("read");
        }
    }
}
