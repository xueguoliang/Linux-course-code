
#include "../network.h"

// ./client src dst ip port
// 遍历src目录
//
void recvDir(int fd)
{
    uint32_t len;
    myReadLen(fd, &len);

    char* dirpath = alloca(len+1);
    myRead(fd, dirpath, len);
    dirpath[len] = 0;
    mkdir(dirpath, 0777);
}

int recvFile(int fd)
{
    uint32_t len;
    myReadLen(fd, &len);

    char* filepath = alloca(len+1);
    myRead(fd, filepath, len);
    filepath[len] = 0;

    myReadLen(fd, &len); // filelen;
    char buf[4096];
    int filefd = open(filepath, O_CREAT|O_WRONLY|O_TRUNC, 0777);

    while(len > 0)
    {
        int readlen = len;
        if(readlen > 4096) readlen = 4096;

        int ret = myRead(fd, buf, readlen);
        if(ret > 0)
        {
            write(filefd, buf, ret);
            len -= ret;
        }
        else 
        {
            break;
        }
    }

    close(filefd);

    if(len == 0) return 0;
    return -1;
}

int main()
{
    int server = myServer(9988, "127.0.0.1");

    while(1)
    {
        int fd = myAccept(server, NULL, NULL);
        if(fd < 0) return 1;

        while(1)
        {
            char flag;
            myRead(fd, &flag, 1);

            if(flag == 'd')
            {
                recvDir(fd);
            }
            else if(flag == 'r')
            {
                if(recvFile(fd) < 0)
                    break;
            }
            else if(flag == 'x')
                break;
        }

        close(fd);
    }
}
