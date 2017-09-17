
#include "../header.h"

int main()
{
    pid_t pid = fork();
    if(pid == 0)
    {
        setsid(); // 进程组长不能创建会话 
        //getchar(); // 终端都没有了，所以不能再getchar了
        sleep(1000);
    }
}
