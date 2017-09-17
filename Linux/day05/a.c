#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
void myfree(void* ptr)
{
    if((intptr_t)ptr > (intptr_t)&ptr)
    {
        // 在栈上
        printf("in stack\n");
    }
    else
    {
        // 在堆上
        printf("in heap\n");
    }
}

int main()
{
    char a;  // 4个字节
    int b;   // 4个字节
    char c;  // 4个字节

    printf("a is %p, b is %p, c is %p\n", &a, &b, &c);

    char* p = malloc(100);
    char q;

    myfree(p);
    myfree(&q);

}
