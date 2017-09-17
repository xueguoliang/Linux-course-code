#include "../header.h"

int i=0;
void func(int v)
{
    // 打断了主流程，实时系统
    printf("Ctrl+C recv, i=%d\n", i);
}

int main()
{
    // 设置信号响应函数
    signal(SIGINT, func);
    for(i=0; i<0xefffffff; ++i)
    {
        ;
    }
}
