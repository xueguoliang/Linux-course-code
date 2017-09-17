
#include "../header.h"
void signal_handle(int v)
{}

int main()
{
    // 告诉内核，当SIGHUP信号发生时，调用signal_handle
    signal(SIGHUP, signal_handle);

    while(1)
    {
        sleep(1);
    }


    //getchar(); // 不能演示SIGHUP的原因是因为getchar和终端有关系
    return 0;
}
