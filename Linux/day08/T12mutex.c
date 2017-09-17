#include "../header.h"

// 定义锁对象
// 如果希望锁在进程间有效果，必须要将其定义在共享内存中
//pthread_mutex_t mutex;

char* shm;
pthread_mutex_t* mutex;

char* attach_shm()
{
    int fd = shm_open("/share_memory", O_RDWR|O_CREAT|O_EXCL, 0777);
    if(fd > 0)
    {
        ftruncate(fd, 4096);
    }
    else
    {
        fd = shm_open("/share_memory", O_RDWR, 0777);
    }

    printf("fd is %d\n", fd);
    void* ptr = mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if(ptr == MAP_FAILED)
    {
        printf("map error");
    }

    return ptr;
}

int main()
{
    shm = attach_shm();
    printf("%s\n", (char*)shm);
    mutex = (pthread_mutex_t*)(shm+10);

    // 初始化
    pthread_mutexattr_t mutexattr;
    pthread_mutexattr_init(&mutexattr);
    pthread_mutexattr_setpshared(&mutexattr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(mutex, &mutexattr);

    // 加锁
    pthread_mutex_lock(mutex);

    // 做一些数据的操作
    printf("get mutex\n");
    getchar();
    printf("ready to release mutex\n");
    
    // 解锁
    pthread_mutex_unlock(mutex);
    printf("release mutex done\n");


}
