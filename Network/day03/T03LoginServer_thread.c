
#include "../network.h"

void* thread_func(void* ptr)
{
    int newfd = (int)(intptr_t)ptr;
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
        // 文件锁

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

    return NULL;

}

int main()
{
    int server = myServer(9988, "0.0.0.0");

    // 创建文件
    close(open("user.data", O_CREAT|O_EXCL, 0777));

    while(1)
    {
        // 阻塞
        int newfd = myAccept(server, NULL, NULL);
        if(newfd == -1)
        {
            break;
        }

        pthread_t tid;
        pthread_create(&tid, NULL, thread_func, (void*)(intptr_t)newfd);
        pthread_detach(tid); // 自生自灭
    }
}
