
#include "../header.h"

int main()
{
    printf("write \n");
    printf("1\n");
    int fd = open("a.fifo", O_WRONLY);
    printf("2\n");

    if(fd < 0)
    {
        perror("open");
        return 0;
    }

    char buf[1024];
    fgets(buf, sizeof(buf), stdin);

    printf("3\n");
    write(fd, buf, strlen(buf));
    printf("4\n");

#if 0
    char buf[1024];
    printf("3\n");
    int ret = read(fd, buf, sizeof(buf));
    printf("4\n");
    printf("%s\n", buf);
#endif
}
