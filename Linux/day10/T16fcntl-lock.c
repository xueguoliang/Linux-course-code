
#include "../header.h"
int main()
{
    int fd = open("a.txt", O_RDWR);

    // 描述锁的具体信息，比如文件区间
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 4096;   // 从文件开始位置
    lock.l_len = 1024;  // 锁1024字节


    // F_SETLK F_SETLKW
    fcntl(fd, F_SETLKW, &lock);  

    // 写操作
    printf("get lock\n");
    getchar();


    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLKW, &lock);
}

