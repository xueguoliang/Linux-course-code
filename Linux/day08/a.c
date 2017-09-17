#include "../header.h"

int main()
{
    FILE* fp = popen("ifconfig | grep inet | grep -v inet6 | awk '{print $2}' | awk -F \":\" '{print $2}'", "r");

    char buf[1024];
    
    while(fgets(buf, sizeof(buf), fp))
    {
        printf("buf is %s", buf);
    }

    pclose(fp);
}
