
#include <sys/stat.h>
#include <stdio.h>

int main()
{
    {
        struct stat buf;
        // buf --> abc   buf-->mount-dir/aaaa
        lstat("dir2/abc", &buf);

        printf("%d\n", (int)buf.st_ino);

    }
}
