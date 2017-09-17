
#include "../network.h"
// 短连接
// 长连接
int main()
{
    char cmd[16];
    char user[32];
    char pass[32];

    while(1)
    {
        int fd = myConnect2(9989, "127.0.0.1");

        printf("please input cmd:");
        fgets(cmd, sizeof(cmd), stdin);
        cmd[strlen(cmd)-1] = 0;
        if(strcmp(cmd, "r") == 0 
                    || strcmp(cmd, "l") == 0)
        {
            printf("input name:");
            fgets(user, sizeof(user), stdin);
            user[strlen(user)-1] = 0;

            printf("input pass:");
            fgets(pass, sizeof(pass), stdin);
            pass[strlen(pass)-1] = 0;

            char packet[2048];
            sprintf(packet, "%s|%s|%s", cmd, user, pass);

            myWriteBuf(fd, packet);

            char* resp = myReadBuf(fd);
            printf("server resp is %s\n", resp);
            free(resp);
        }
        close(fd);
    } 
}
