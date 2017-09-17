#include "../header.h"
#include <stdarg.h>

#if 0
int add(int count, int a, int b, ...)
{
    int c = a+b;
    // 表示后面不定参数，其实是个指针
    va_list ap;

    va_start(ap, b); // ap指向b的后面

    int i;
    for(i=0;i <count; ++i)
    {
        int arg = va_arg(ap, int);
        c += arg;
    }

    va_end(ap); // 清理

    return c;
}

int main()
{
    int c = add(1, 2, 3, 4);
    printf("c=%d\n", c);
}
#endif

void __mylog(const char* file, int line, const char* fmt, ...)
{
    char buf[2048];
    va_list ap;
    va_start(ap, fmt);
    vsprintf(buf, fmt, ap);
    va_end(ap);

    printf("[%s:%d]:%s", file, line, buf);
}

#define mylog(fmt, ...) __mylog(__FILE__, __LINE__, fmt, ##__VA_ARGS__)

int main()
{
    mylog("a is %d\n", 100);
}

