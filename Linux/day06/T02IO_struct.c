
#include "../header.h"
struct student
{
    char* name;
    int score;
    int age;
};

int main()
{
    struct student s;
    int fd = open("s.info", O_RDONLY); 

#if 0
    write(fd, &s.age, sizeof(s.age));
    write(fd, &s.score, sizeof(s.score));
    write(fd, &len, sizeof(len)); // 标记
    write(fd, s.name, strlen(s.name));
#endif

    int tmp;
    read(fd, &tmp, sizeof(tmp));
    s.age = ntohl(tmp);   // htonl

    read(fd, &tmp, sizeof(tmp));
    s.score = ntohl(tmp);

    int len;
    read(fd, &tmp, sizeof(tmp));
    len = ntohl(tmp);

    s.name = malloc(len + 1);
    s.name[len] = '\0';
    read(fd, s.name, len);

    printf("name=%s, age=%d, score=%d\n", s.name, s.age, s.score);


    close(fd);

    free(s.name);
}
