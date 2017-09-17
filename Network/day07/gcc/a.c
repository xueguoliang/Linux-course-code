#include <stdio.h>
#include "a.h"
#include "xx.h"
#define A 5
int foo()
{
    int * p = 0;
    *p = 10;
    int a = A;
    return 0;
}

int main()
{
    int a = XXXX;

    foo();

    int c = a + 5;

    printf("%d\n", c);
}
