
int main()
{
    // 0, stdin, 1, stdout, 2 stderr
    int fd1 = open("fd.c", O_RDONLY); // fd1 == 3
    int fd2 = open("fd.c", O_RDONLY); // fd2 == 4
    int fd3 = open("fd.c", O_RDONLY); // fd3 == 5

    close(fd2); // 4 is closed

    int fd4 = open("fd.c", O_RDONLY); // fd4 == 4

}
