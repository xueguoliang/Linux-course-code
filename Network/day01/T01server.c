#include "../header.h"
int main()
{
    // 创建一个socket对象
    // 返回值：是一个文件描述符
    // 参数：AF_INET表示这个socket是用于internet网络通信的socket
    //       SOCK_STREAM表示创建流套接字（TCP）,如果你想创建一个UDP套接字，这里应该写SOCK_DGRAM
    //       0表示协议，IP协议
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    // 指定地址:ip地址+端口
    struct sockaddr_in addr;
    addr.sin_family = AF_INET; //表示这个地址用于INET类型网络通信地址
    // addr.sin_addr.s_addr = inet_addr("192.168.19.115"); // sin_addr用来指示ip地址的  
    //  addr.sin_addr.s_addr = INADDR_ANY; // 0(INADDR_ANY)表示程序要监听任何网口
    addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 127.0.0.1是一个特殊的ip地址，表示本机
    addr.sin_port = htons(20001);  // 表示端口  h:host  to   n:network s:short    

    // 要把这个地址和fd 关联，使用绑定函数
    if(bind(fd, (struct sockaddr*)&addr, sizeof(addr)) != 0)
    {
        perror("bind");
        return 0;
    }

    printf("start listen\n");

    // 开始设置监听，第二个参数表示能够同时接纳的连接数
    // 其实是一个缓冲区长度
    listen(fd, 250);

    // 检查是否有人来连接我, 这个调用可能是阻塞的，如果没有人连接，这个函数会阻塞
    // 返回值是一个新的socket，这个socket才是用于数据通信，而服务器的fd用来接收连接
    int newfd = accept(fd, NULL, NULL);
    if(newfd < 0)
    {
        perror("accept");
        return 0;
    }

    int bufsize = 0;
    socklen_t argsize = sizeof(bufsize);

    getsockopt(newfd, SOL_SOCKET, SO_RCVBUF, &bufsize, &argsize);
    printf("recv bufsize is %d\n", bufsize); 
    getsockopt(newfd, SOL_SOCKET, SO_SNDBUF, &bufsize, &argsize);
    printf("send bufsize is %d\n", bufsize); 
    // 从newfd读取数据，显然也是阻塞的
    char buf[1024];
    memset(buf, 0, sizeof(buf));
    read(newfd, buf, sizeof(buf));
    // read返回实际的读取的数量，如果是0表示对方关闭了socket
    // 如果-1表示失败

    printf("buf = %s\n", buf);

    // 关闭文件描述符号
    close(newfd);
    close(fd);

    return 0;
}
