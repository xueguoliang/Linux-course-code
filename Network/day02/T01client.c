
#include "../header.h"

// ./a.out src ip port
int main(int argc, char** argv)
{
    signal(SIGPIPE, SIG_IGN);
    const char* file = argv[1];
    const char* ip = argv[2];
    const char* port = argv[3];

    int fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port=  htons(atoi(port));
    addr.sin_addr.s_addr = inet_addr(ip);

    int ret = connect(fd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret < 0)
    {
        perror("connect");
        return 0;
    }


    int filefd = open(file, O_RDONLY);

    char buf[4096];
    while(1)
    {
        int ret = read(filefd, buf, sizeof(buf));
        if(ret == 0) break;

        int writelen = 0;
        while(1)
        {
            int r = write(fd, buf+writelen, ret-writelen);
            if(r > 0) writelen += r;
            if(r < 0) 
            {
                perror("write");
                break;
            }
            if(writelen == ret) break;
        }
    }
}



