#include "../header.h"
// 将参数所指的集合，写入到PCB
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
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);

//    sigprocmask(
    setmask(&mask);

    int i = 0;
    while(1)
    {
        sleep(1);
        ++i;
        if(i >= 5)
        {
            // 掩码取消，取消之后，该进程处理SIGINT信号，这个信号导致程序退出
            unsetmask(&mask);
        }
    }

}
