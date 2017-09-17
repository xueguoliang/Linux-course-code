
#include "../network.h"

int sendFile(const char* dst, const char* src, int fd)
{
    myWrite(fd, "r", 1);
    myWriteLen(fd, strlen(dst));
    myWrite(fd, dst, strlen(dst));
    myWriteLen(fd, myFileSize(src));

    int filefd = open(src, O_RDONLY);
    char buf[4096];
    while(1)
    {
        int ret = read(filefd, buf, sizeof(buf));
        if(ret == 0) break;

        myWrite(fd, buf, ret);
    }
    close(filefd);
}

int sendDir(const char* dst, const char* src, int fd)
{
    myWrite(fd, "d", 1);
    myWriteLen(fd, strlen(dst));
    myWrite(fd, dst, strlen(dst));
            
    char* bufDst = malloc(1024);
    char* bufSrc = malloc(1024);

    // 遍历目录项
    DIR* dir = opendir(src);
    struct dirent* entry;
    do {
        entry = readdir(dir);
        if(entry == NULL)
            break;
    

        if(entry->d_name[0] == '.') continue; // ignore . .. 隐藏文件
        sprintf(bufDst, "%s/%s", dst, entry->d_name);
        sprintf(bufSrc, "%s/%s", src, entry->d_name);

        if(entry->d_type == DT_REG)
        {
            sendFile(bufDst, bufSrc, fd);
        }
        else if(entry->d_type == DT_DIR)
        {
            sendDir(bufDst, bufSrc, fd);
        }
    }
    while(1);

    free(bufDst);
    free(bufSrc);
    closedir(dir);
    return 0;
}

int main(int argc, char* argv[])
{
    const char* src = argv[1];
    const char* dst = argv[2];
    const char* ip = argv[3];
    const char* port = argv[4];

    int fd = socket(AF_INET, SOCK_STREAM, 0);

    int ret = myConnect(fd, atoi(port), ip);
    if(ret < 0)
    {
        return -1;
    }


    sendDir(dst, src, fd);
    myWrite(fd, "x", 1); // 结束标记
}












