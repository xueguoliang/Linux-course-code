
#include "../header.h"

int main(int argc, char* argv[])
{
    int fd = open("a.txt", O_WRONLY);  // fd is 3

    int flag = fcntl(fd, F_GETFD);
    fcntl(fd, F_SETFD, FD_CLOEXEC | flag);

    // 要执行helloworld程序，要调用exec函数
    // 调用execl函数，需要提供程序路径，和命令行参数
    // 命令行参数的第一个总是程序路径本身，后面可以输入更多参数，最后以NULL结束，NULL是哨兵（结束标记）
    int ret = execl("./writefile", "./writefile", NULL);

    if(ret == 0)
    {
        printf("execl success\n");
    }
    else
    {
        printf("execl failure\n");
    }
    
}
