#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    // 0, stdin, 1, stdout, 2 stderr
    int fd = open("a.tmp", O_WRONLY|O_APPEND); // fd1 == 3

    for(int i=0; i<10; ++i)
    {
        write(fd, "hello", 5);

        sleep(1);
    }

}
