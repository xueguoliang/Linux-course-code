#include "../header.h"

// 多路IO问题

void* key_thread(void* ptr)
{
    int fd_key = open("/dev/input/event1", O_RDONLY);
    char key[1024];
    while(1)
    {
        int ret = read(fd_key, key, sizeof(key));
        printf("key event %d\n", ret);
    }
}

void* mice_thread(void* ptr)
{
    int fd = open("/dev/input/mice", O_RDONLY);
    char buf[8];
    while(1)
    {
        int ret = read(fd, buf, sizeof(buf));
        printf("0x%02x, 0x%02x, 0x%02x\n", 
                (unsigned char)buf[0], 
                (unsigned char)buf[1], 
                (unsigned char)buf[2]);
    }

}
int main()
{
    pthread_t tid_key, tid_mice;

    pthread_create(&tid_key,  NULL, key_thread,  NULL);
    pthread_create(&tid_mice, NULL, mice_thread, NULL);


    pthread_join(tid_key, NULL);
    pthread_join(tid_mice, NULL);
}
