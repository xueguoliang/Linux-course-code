

#include "../header.h"
void signal_handle(int v)
{
    printf("signal_handle\n");
    exit(0);
}
int main()
{
    //signal(SIGKILL, signal_handle); // error
    signal(SIGFPE, signal_handle);
    int a = 0;
    int b = 10/a;
}
