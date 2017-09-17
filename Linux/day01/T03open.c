
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main()
{
//    int fd = open("a.tmp", O_WRONLY|O_CREAT, 0777);
    int fd = open("a.tmp", O_WRONLY|O_TRUNC);
    printf("fd is %d\n", fd);
}
