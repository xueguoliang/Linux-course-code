#include "../header.h"
int main()
{
    int fd[2];
    socketpair(AF_UNIX, SOCK_STREAM, 0, fd);

    write(fd[1], "buf", 3);

    char buf[1024];
    int ret = read(fd[0], buf, sizeof(buf));
    printf("%s\n", buf);

}
