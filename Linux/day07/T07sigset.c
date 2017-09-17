
#include "../header.h"
#if 0
uint32_t sig_set = 0; // 没有任何标记

// 2进制
// 0000000001
//
// 0000100000
// 0000000000
// 0000100000
// 
// 0000000001
// 0001000000
//
// 0001100000
//
int main()
{
    sig_set |= 1 << SIGFPE;
    sig_set |= 1 << SIGUSR1;
    sig_set &= ~(1<<SIGUSR1); // 在一个整数中 把一位置0
}

#endif



int main()
{
    sigset_t sigset;
    sigemptyset(&sigset); // sigset = 0;
    sigfillset(&sigset); // sigset = 0xffffffff

    sigaddset(&sigset, SIGUSR1);
    if(!sigismember(&sigset, SIGUSR1))
    {
        printf("not in set\n");
    }

    sigdelset(&sigset, SIGUSR1);

    if(sigismember(&sigset, SIGUSR1))
    {
        printf("in set\n");
    }
}
