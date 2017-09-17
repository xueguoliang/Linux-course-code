// server.c
//

#include "../header.h"

int main()
{
    int sock = socket(AF_UNIX, SOCK_STREAM, 0);

    // bind 
    // struct sockaddr_in in: internet
    //                    un: unix
    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, "socket_file");

    bind(sock, (struct sockaddr*)&addr, sizeof(addr));

    listen(sock, 20);

    int newfd = accept(sock, NULL, NULL);

    char buf[1024];
    read(newfd, buf, sizeof(buf));

    printf("read data is: %s\n", buf);
}
