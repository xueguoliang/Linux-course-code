
#include "../network.h"

void handle(int newfd)
{
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
    close(newfd); // 关闭socket时，会自动将socket从epoll中取出
}

void epoll_add(int epollfd, int sock, int event)
{
    struct epoll_event ev;
    ev.data.fd = sock;
    ev.events = event;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, sock, &ev);
}

int main()
{
    int server = myServer(9988, "0.0.0.0");

    // 创建文件
    close(open("user.data", O_CREAT|O_EXCL, 0777));

    int epollfd = epoll_create(1024);

    // 把server-->epollfd
    epoll_add(epollfd, server, EPOLLIN);

    struct epoll_event ev[8];
    while(1)
    {
        int ret = epoll_wait(epollfd, ev, sizeof(ev)/sizeof(*ev), 5000);
        if(ret > 0)
        {
            int i;
            for(i=0; i<ret; ++i)
            {
                if(ev[i].data.fd == server)
                {
                    int newfd = myAccept(server, NULL, NULL);
                    epoll_add(epollfd, newfd, EPOLLIN);
                }
                else
                {
                    handle(ev[i].data.fd);                
                }
            }
        }
        else if(ret == 0)
        {
            // 没有socket有数据
        }
        else if(ret < 0 && errno == EINTR)
        {
            // 被信号打断
        }
        else // ret == -1 
        {
            printf("errno=%d\n", errno);
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

        // 读取数据，阻塞
    }
#endif
}
