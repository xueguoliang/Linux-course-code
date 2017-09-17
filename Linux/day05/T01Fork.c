
#include "../header.h"

int a = 100;

int main()
{
    a++;
    // 调用fork之后，以下的执行代码，分成两路
    // 一路的执行过程是主进程
    // 一路子进程
    //
    // 但是fork之前的代码，子进程不会再执行一次，但是
    // 效果是有保存的
    pid_t pid = fork();
    // 如果返回值大于0，那么说明这个分支是父进程
    // 如果==0，那么是子进程
    if(pid == 0) // 子进程
    {
        a = 10000; // 只是修改子进程自己的变量，没有也没办法去修改父进程a变量
        printf("child getpid()=%d\n", (int)getpid());
        printf("this is child, getppid()=%d\n", (int)getppid());
    }
    else
    {
        sleep(1);
        printf("a is %d\n", a);
        printf("child pid=%d\n", (int)pid);
        printf("this is parent, getpid()=%d\n", (int)getpid());
    }
}
