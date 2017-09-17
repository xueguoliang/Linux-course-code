
#include "../header.h"

int main()
{
    printf("1\n");
    int fd = open("a.fifo", O_RDONLY);
    printf("2\n");

    if(fd < 0)
    {
        perror("open");
        return 0;
    }

    char buf[1024];
    printf("3\n");
    int ret = read(fd, buf, sizeof(buf));
    printf("4\n");
    printf("%s\n", buf);
}
