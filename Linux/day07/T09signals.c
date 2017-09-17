#include "../header.h"

// 实时信号处理函数
void handle_rt_sig(int v)
{
    printf("real time sig\n");

}

void handle_sigint(int v)
{
    printf("signal int\n");
}

void setmask(sigset_t* set)
{
    sigprocmask(SIG_BLOCK, set, NULL);
}

void unsetmask(sigset_t* set)
{
    sigprocmask(SIG_UNBLOCK, set, NULL);
}

int main()
{
//    signal(SIGINT, handle_sigint);    
    signal(34, handle_rt_sig);  
    
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, 34);
    setmask(&mask);

    kill(getpid(), 34);
    kill(getpid(), 34);
    kill(getpid(), 34);
    kill(getpid(), 34);

    unsetmask(&mask);// 调用一次信号处理函数


}
