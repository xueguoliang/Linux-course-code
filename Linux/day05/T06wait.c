
#include "../header.h"

// wait 函数函数作用：
// 1. 收尸
// 2. 能感知进程状态变化（STOP --> RUNNING, RUNNING --> STOP)
// 3. 获取子进程结束状态
int main()
{
    pid_t pid = fork();
    if(pid == 0)
    {
        sleep(2);
        return 111; // 子进程结束
    }
    else
    {
        //sleep(1); // 让子进程先结束
//        getchar(); // 父进程不退出，所以子进程变成了僵尸
        int status;
        // wait函数用于收尸
        // 由于一个父进程，可能有很多子进程，而且有可能有很多子进程都处于僵尸状态
        // 所以它有一个返回值，表示收的是哪个进程的尸体
        // 这也意味着，一次wait只能收一个尸体
        // 收尸的同时，也获得子进程的运行结果，运行结果在status中，如果你不关心结果，可以用NULL
        pid_t pid = wait(&status);
        printf("hello world, status=%d\n", status);

        /*status有32位，其中只有8位用于保存子进程return的值，其他位有其他用途*/

        // pid_t pid = wait(NULL); 也是可以的
        // wait是一个阻塞调用，如果没有尸体，他会一直等尸体

        if(WIFEXITED(status))
        {
            printf("normal exit\n");
            printf("exit code is %d\n", WEXITSTATUS(status));
        }
        getchar();
    }
}
