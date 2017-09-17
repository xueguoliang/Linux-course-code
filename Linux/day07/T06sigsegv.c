
#include "../header.h"

void signal_handle(int v)
{
    printf("segv\n");
}

int main()
{
    signal(SIGSEGV, signal_handle);
    int * p = NULL;


    // 应该要收到惩罚的代码
    *p = 100;

    printf("haha\n");
}
