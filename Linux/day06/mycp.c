
#include "../header.h"

// ./a.out srcfile dstfile 4
//
//不用 ./a.out -j 4 srcfile dstfile
//不用 ./a.out srcfile dstfile -j 4
//

// 41M [0~10M)  [10M-20M) [20M-30M) [30M-40M)
// 41M / 3
int copy_part(int index, const char* srcfile, const char* dstfile, int filesize, int total)
{
    int fd_src = open(srcfile, O_RDONLY);
    int fd_dst = open(dstfile, O_WRONLY);

    int part_size = filesize / (total-1);

    lseek(fd_src, index*part_size, SEEK_SET);
    lseek(fd_dst, index*part_size, SEEK_SET);

    char buf[4096];
    while(part_size > 0)
    {
        int readlen = sizeof(buf);
        if(part_size < readlen)
            readlen = part_size;

        int ret = read(fd_src, buf, readlen);
        if(ret <= 0) 
            break;
        write(fd_dst, buf, ret);
        part_size -= ret;
    }

    return 0;
}

int main(int argc, char* argv[])
{
    const char* srcfile = argv[1];
    const char* dstfile = argv[2];
    int process_count = atoi(argv[3]);

    unlink(argv[2]); // 如果目标文件有，删除它
    close(open(dstfile, O_CREAT, 0777)); // 创建文件

    struct stat stat_buf;
    stat(srcfile, &stat_buf);

    int i=0;
    for(i=0; i<process_count; ++i)
    {
        pid_t pid = fork();
        if(pid == 0)
        {
            copy_part(i, srcfile, dstfile, stat_buf.st_size, process_count);
            return 0;
        }
    }

    for(i=0; i<process_count; ++i)
    {
        wait(NULL);
    }

    return 0;
}
