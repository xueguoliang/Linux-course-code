
#include "../header.h"

int main()
{
    char buf[] = "hello world";
    char* p = malloc(100);
    strcpy(p, "xxx");
    void* ptr = mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANON, -1, 0);

    pid_t pid = fork();
    if(pid == 0)
    {
        strcpy(buf, "hello sz");
        strcpy(p, "yyy");
        strcpy((char*)ptr, "hello parent");
        return 0;
    }

    sleep(1);
    printf("%s\n", (char*)ptr); // mmap申请的内存才是共享的
    printf("%s\n", buf); // 局部变量不共享
    printf("%s\n", p); // xxx 堆上的空间也不共享

}
