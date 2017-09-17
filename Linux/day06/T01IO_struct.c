
#include "../header.h"
struct student
{
    char* name;
    int score;
    int age;
};

int main()
{
    char buf[2048];
    struct student s;
    int len;

    printf("please input name:");  // tom\n
    fgets(buf, sizeof(buf), stdin);
    len = strlen(buf);
    buf[len-1] = 0;  // remove \n

    s.name = strdup(buf);

#if 0
    int len = strlen(buf);
    s.name = malloc(len); // not include \n
    memset(s.name, 0, len);  // \0\0\0\0
    memcpy(s.name, buf, len - 1); // 3
#endif

    printf("please inupt age:");
    fgets(buf, sizeof(buf), stdin);
    len = strlen(buf);
    buf[len-1] = 0;  // remove \n

    s.age = atoi(buf);

    printf("please input score:");
    fgets(buf, sizeof(buf), stdin);
    len = strlen(buf);
    buf[len-1] = 0;  // remove \n

    s.score = atoi(buf);

    // 结构体构造结束，打开文件
    int fd = open("s.info", O_WRONLY|O_CREAT|O_TRUNC, 0777); 
    len = strlen(s.name);

    // 把age转化成大端，再保存
    int tmp;
    tmp = htonl(s.age);  // 将一个整数，转化成大端字节序，htonl --> host to network long 
    // htons htonll, 网络序固定用大端字节序
    write(fd, &tmp, sizeof(tmp));
    tmp = htonl(s.score);
    write(fd, &tmp, sizeof(tmp));
    tmp = htonl(len);
    write(fd, &tmp, sizeof(tmp)); // 标记
    write(fd, s.name, strlen(s.name));

    close(fd);

    free(s.name);
}
