#include "../header.h"
int main()
{
    pid_t pid = fork(); // 创建子进程
    if(pid > 0)
    {
        // 父亲进程
        return 0;
    }

    getchar();
}
