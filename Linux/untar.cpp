
#include "header.h"

// a.out mytar
int main(int argc, char* argv[])
{
    char* path = argv[1];

    FILE* fpin = fopen(path, "r");

    char buf[4096];
    while(1)
    {
        if(fgets(buf, sizeof(buf), fpin) == NULL)
            break;

        if(strcmp(buf, "f\n") == 0)
        {
            fgets(buf, sizeof(buf), fpin);
            buf[strlen(buf)-1] = 0; // 去掉\n
            FILE* fpout = fopen(buf, "w");

            fgets(buf, sizeof(buf), fpin);
            buf[strlen(buf)-1] = 0; // 去掉\n
            int len = atoi(buf);

            while(1)
            {
                int readlen = len > sizeof(buf) ? sizeof(buf) : len;

                int ret = fread(buf, 1, readlen, fpin);
                if(ret <= 0) 
                {
                    // 文件格式有问题
                    printf("error untar\n");
                    exit(1);
                }
                fwrite(buf, ret, 1, fpout);

                len -= ret;
                if(len <= 0) break;
            }

            fclose(fpout);
        }
        else if(strcmp(buf, "d\n") == 0)
        {
            fgets(buf, sizeof(buf), fpin);
            buf[strlen(buf)-1] = 0; // 去掉\n
            mkdir(buf, 0777);
        }
    }
}
