
#include "../header.h"

/* 当进程退出时，会自动调用on_exit注册的函数 */
/* on_exit注册的函数，只有进程正常退出时，会被调用，异常退出时，不会被调用 */
/* ret 是main函数的返回值 */
void func(int ret, void* ptr)
{
    printf("ret is %d\n", ret);
}

int foo()
{
    exit(100);
}

int main()
{
    on_exit(func, NULL); 
 //   foo();
    getchar();
    return 7;
}
