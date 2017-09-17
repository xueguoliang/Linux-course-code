#include "network.h"

int myFileSize(const char* path)
{
    struct stat buf;
    stat(path, &buf);
    return buf.st_size;
}

/*用于创建server*/
int myServer(uint16_t port, const char* ip)
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr; 
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);

    int ret = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret < 0)
    {
        perror("bind");
        close(fd);
        return -1;
    }

    listen(fd, 250);

    return fd;
}

int myAccept(int server, struct sockaddr* addr, socklen_t* addrlen)
{
    while(1)
    {
        int fd = accept(server, addr, addrlen);
        if(fd < 0)
        {
            if(errno == EINTR)
            {
                continue;    
            }
            perror("accept");
            break;
        }
        return fd;
    }

    return -1;
}

int myRead(int fd, char buf[], int len)
{
    int readlen = 0;

    while(readlen != len)
    {
        int ret = read(fd, buf+readlen, len-readlen);
        if(ret > 0)
            readlen += ret;
        else if(ret < 0)
        {
            if(errno == EINTR)
                continue;

            return ret;
        }
        else if(ret == 0)
            break;
    }

    return readlen;
}

int myRead2(int fd, char buf[], int len)
{
    int ret = -1;
    while(1)
    {
        ret = read(fd, buf, len);
        if(ret == -1 && errno == EINTR)
            continue;
        
        return ret;
    }
    return ret;
}

int myWrite(int fd, const char buf[], int len)
{
    int writelen = 0;
    while(writelen != len)
    {
        int ret = write(fd, buf+writelen, len-writelen);
        if(ret > 0) writelen += ret;
        else if(ret < 0)
        {
            if(errno == EINTR) continue;
            break;
        }
    }
    return writelen;
}

int myConnect2(uint16_t port, const char* ip)
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(myConnect(fd, port, ip) == 0)
        return fd;

    close(fd);
    return -1;
}
int myConnect(int fd, uint16_t port, const char* ip)
{
    
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port=  htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);

    int ret = connect(fd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret < 0)
    {
        perror("connect");
        return -1;
    }
    return 0;
}

int myWriteLen(int fd, uint32_t len)
{
    len = htonl(len);
    return myWrite(fd, (char*)&len, sizeof(len));
}

int myReadLen(int fd, uint32_t* len)
{
    int ret = myRead(fd, (char*)len, sizeof(uint32_t));
    *len = ntohl(*len);
    return ret;
}

char* myReadBuf(int fd)
{
    uint32_t len; // 500
    myReadLen(fd, &len);
    char* buf = (char*)malloc(len+1);
    buf[len] = 0;
    myRead(fd, buf, len); // 50个字节
    return buf;
}

void myWriteBuf(int fd, const char* buf)
{
    uint32_t len = strlen(buf);
    myWriteLen(fd, len);
    myWrite(fd, buf, len);
}
