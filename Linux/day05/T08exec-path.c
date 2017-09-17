#include "../header.h"
int main()
{
    // execl时找不到ls程序
    // 使用execlp时，execlp会到环境变量的PATH变量中，寻找ls
    //execlp("ls", "ls", NULL);
    execlp("/bin/ls", "/bin/ls", NULL);
    printf("error\n");
}
