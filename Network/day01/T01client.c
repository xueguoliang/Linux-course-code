#include "../header.h"
int main()
{
    signal(SIGPIPE, SIG_IGN);

    int fd = socket(AF_INET, SOCK_STREAM, 0);

    // 客户端不需要指定地址，也就是说不需要bind，它的端口是系统随机分配的
    // 需要做连接操作
    //

    // 初始化服务器的地址信息
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(20001); 

    // 执行连接操作
    if(connect(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
    {
        perror("connect");
    }

    // write时，如果对方关闭了socket，那么会收到SIGPIPE（管道破裂），会导致程序退出
    int ret = write(fd, "hello world", 11);
    // 返回实际写入的数据数量，注意有可能没有将所有数据写入socket

}
