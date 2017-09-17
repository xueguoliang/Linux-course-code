#include "../header.h"
int main()
{
//    int fd = open("share_file", O_RDWR);
    int fd = shm_open("/share_file", O_RDWR|O_CREAT|O_EXCL, 0777);
    if(fd < 0)
    {
        fd = shm_open("/share_file", O_RDWR, 0777);
    }
    else
    {
        ftruncate(fd, 4096);
    }

    void* ptr = mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    strcpy((char*)ptr, "write by T08");
    printf("write\n");
    getchar();

    close(fd);
    shm_unlink("/share_file");
    munmap(ptr, 4096);
}
