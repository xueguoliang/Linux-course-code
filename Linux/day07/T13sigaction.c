#include "../header.h"
// 信号处理函数
void sig_handle(int v)
{

}

// 信号处理函数
void sig_action(int v, siginfo_t* info, void* ptr)
{
    // 掩盖一些信号

    // 正在处理一个全局链表
    // 然后被别的信号打断
    printf("int signal\n");
    printf("arg is: %d\n", info->si_int);

    // 恢复这些被掩盖的信号
    // 目的：为了避免可重入问题
}

int main()
{
    struct sigaction action;
//    action.sa_handler = sig_handle;
    action.sa_handler = NULL;
    action.sa_sigaction = sig_action; // 两个处理函数不用同时赋值
    sigemptyset(&action.sa_mask); // 很关键
    sigaddset(&action.sa_mask, SIGUSR1); // 当这个信号函数被处理时，SIGUSR1暂时被屏蔽，当这个信号处理结束之后，自动会放开屏蔽
    action.sa_flags = SA_SIGINFO;
    action.sa_restorer = NULL;  // 手册里说这个参数没有用了，直接NULL就可以了

    sigaction(SIGINT, &action, NULL);

//    kill(getpid(), SIGINT);

    union sigval v;
    v.sival_int = 250;
    sigqueue(getpid(), SIGINT, v);

    sleep(1);
}
