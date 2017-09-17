
#include "../network.h"

// 保存着所有的socket文件描述符
fd_set saveSet;
int maxfd;

void handle(int newfd)
{
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

    // 清理集合
    FD_CLR(newfd, &saveSet);
    if(maxfd == newfd) maxfd --;
}

int main()
{
    int server = myServer(9988, "0.0.0.0");

    maxfd = server;
    FD_ZERO(&saveSet);  // server in saveSet
    FD_SET(server, &saveSet);

    // 创建文件
    close(open("user.data", O_CREAT|O_EXCL, 0777));


    while(1)
    {
        fd_set set;
        struct timeval tv;

        // 初始化 maxfd, set, tv
        memcpy(&set, &saveSet, sizeof(set)); // sever, newfd --> set
        tv.tv_sec = 5;
        tv.tv_usec = 0;

        int ret = select(maxfd+1, &set, NULL, NULL, &tv);
        if(ret > 0)
        {
            if(FD_ISSET(server, &set))
            {
                int newfd = myAccept(server, NULL, NULL);
                if(maxfd < newfd) maxfd = newfd;
                FD_SET(newfd, &saveSet);  // server, newfd --> saveSet
            }

            int i;
            for(i=server+1; i<=maxfd; ++i)
            {
                if(FD_ISSET(i, &set))
                {
                    handle(i);
                } 
            }
        }
        else if(ret == 0)
        {
            // 没有socket有消息
        }
        else if(ret < 0 && errno == EINTR)
        {
            // 被中断打断
        }
        else // ret < 0
        {
            // 严重错误
            printf("error, errno=%d\n", errno);
            break;
        }
    }


#if 0

    while(1)
    {
        // 阻塞
        int newfd = myAccept(server, NULL, NULL);
        if(newfd == -1)
        {
            break;
        }

        // 接收数据
        // [报文长度]r|username|password
        // [报文长度]l|username|password

    }
#endif
}
