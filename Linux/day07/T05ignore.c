
#include "../header.h"


int main()
{
    signal(SIGINT, SIG_IGN); // 忽略SIGINT信号
    // 忽略也是注册了信号处理函数，只是信号处理函数什么也不做
    getchar();
}
