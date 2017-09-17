
#include "../network.h"

void sig_handle(int v)
{
    if(v == SIGCHLD)
    {
        // SIGCHLD是不可靠的信号
        while(1)
        {
            // 如果有进程挂掉，那么返回挂掉的子进程的pid
            // 如果没有进程挂掉，那么返回0
            // 如果有错误，返回-1
            pid_t pid = waitpid(-1, NULL, WNOHANG);
            if(pid <= 0) 
                break;
        }
    }
}

int main()
{
    signal(SIGCHLD, sig_handle);

    int server = myServer(9988, "0.0.0.0");

    // 创建文件
    close(open("user.data", O_CREAT|O_EXCL, 0777));
    int newfd;

    while(1)
    {
        // 阻塞
        newfd = myAccept(server, NULL, NULL);
        if(newfd == -1)
        {
            return 0;
        }

        pid_t pid = fork();
        if(pid == 0) // 子进程
        {
            break;
        }

        close(newfd); // 父进程 必须要close(newfd)，以免文件描述符资源被占用
    }

    // 子进程
    close(server);

    // 接收数据
    // [报文长度]r|username|password
    // [报文长度]l|username|password

    // 读取数据，阻塞
    char* buf = myReadBuf(newfd);
    char* saveptr;
    const char* type = strtok_r(buf, "|", &saveptr);
    const char* user = strtok_r(NULL, "|", &saveptr);
    const char* pass = strtok_r(NULL, "|", &saveptr);

    if(type[0] == 'r')
    {
        // 注册, 用户名一行，密码一行
        FILE* fp = fopen("user.data", "a+");
        fprintf(fp, "%s\n%s\n", user, pass);
        fclose(fp);

        // [报文长度]r|ok
        myWriteBuf(newfd, "r|ok");
    }
    else if(type[0] == 'l')
    {
        char ubuf[1024];
        char pbuf[1024];
        char* p;
        // 登录
        FILE* fp = fopen("user.data", "r");
        while(1)
        {
            p = fgets(ubuf, sizeof(ubuf), fp);
            if(p == NULL)
                break;
            fgets(pbuf, sizeof(pbuf), fp); 

            ubuf[strlen(ubuf)-1] = 0; // remove \n
            pbuf[strlen(pbuf)-1] = 0; // remove \n

            if(strcmp(ubuf, user) == 0 &&
                    strcmp(pbuf, pass) == 0)
            {
                myWriteBuf(newfd, "l|ok");
                break;
            }
        }

        if(p == NULL) // 遍历整个文件，都没有匹配
        {
            myWriteBuf(newfd, "l|err");
        }

        fclose(fp);
    }
    else
    {
        myWriteBuf(newfd, "unknown command");
        // 说明有人捣乱，直接关闭socket
    }

    free(buf);      
    close(newfd);
}
