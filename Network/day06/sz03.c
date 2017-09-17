#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char** environ;
int main()
{
    // 规定
    printf("Content-type:text/html\r\n\r\n");


    int i;
    for(i=0; ; ++i)
    {
        if(environ[i] == NULL) break;
        //        printf("%s<br>\n", environ[i]);
    }

#if 0
    // FOR GET METHOD
    char* arg = getenv("QUERY_STRING"); // user=aaa&pass=bbb
    strtok(arg, "=&");
    char* user = strtok(NULL, "=&");
    strtok(NULL, "=&");
    char* pass = strtok(NULL, "=&");

    printf("username is %s password is %s", user, pass);
#else
    if(strcmp(getenv("REQUEST_METHOD"), "POST") == 0)
    {
        // POST
        int content_length = atoi(getenv("CONTENT_LENGTH"));
        char* buf = malloc(content_length+1);
        buf[content_length] = 0;

        // 从标准输入读取，也就是从socket读取
        fread(buf, content_length, 1, stdin);
        char* arg = buf;

        strtok(arg, "=&");
        char* user = strtok(NULL, "=&");
        strtok(NULL, "=&");
        char* pass = strtok(NULL, "=&");

        printf("username is %s password is %s", user, pass);
    }
#endif
}
