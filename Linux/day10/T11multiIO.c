#include "../header.h"

// 多路IO问题
int main()
{
    int fd = open("/dev/input/mice", O_RDONLY);
    int fd_key = open("/dev/input/event1", O_RDONLY);

    int ret;
    char buf[8];
    char key[1024];
    while(1)
    {
        // 阻塞
        ret = read(fd, buf, sizeof(buf));

        printf("0x%02x, 0x%02x, 0x%02x\n", 
                (unsigned char)buf[0], 
                (unsigned char)buf[1], 
                (unsigned char)buf[2]);
        
        ret = read(fd_key, key, sizeof(key));
        printf("recv key ... ret=%d\n", ret);
    }
}
