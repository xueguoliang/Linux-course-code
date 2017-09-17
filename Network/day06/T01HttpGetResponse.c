#include "../network.h"

char request_buf[] = 
"GET /a.html HTTP/1.1\r\n"
"Host: 127.0.0.1:80\r\n"
"User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:28.0) Gecko/20100101 Firefox/28.0\r\n"
"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
"Accept-Language: en-US,en;q=0.5\r\n"
"Accept-Encoding: gzip, deflate\r\n"
"Connection: keep-alive\r\n"
"\r\n";

int main()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr*)&addr, sizeof(addr));

    // 发送请求给apache
    myWrite(sock, request_buf, strlen(request_buf));

    // apache给返回数据
    
    char response_buf[8192];
    recv(sock, response_buf, sizeof(response_buf), 0);

    printf("%s\n", response_buf);
}
