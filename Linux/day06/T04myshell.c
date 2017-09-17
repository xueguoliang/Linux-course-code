#include "../header.h"
int print_tips()
{
    char cwd[2048];
    getcwd(cwd, sizeof(cwd));
    printf("myshell:%s > ", cwd);
}

#if 0
//1. 执行最简单的命令
void handle_cmd(char* cmd)
{
    pid_t pid = fork();
    if(pid == 0)
    {
        execlp(cmd, cmd, NULL);
        exit(0); // 为了在执行execlp错误时，子进程也要结束
    }

    wait(NULL);
}
#endif

#if 0
/*执行一些复杂的命令，ls -a -l, ls*/
void handle_cmd(char* cmd)
{
    char* args[250];
    int i = 0;

    char* c = strtok(cmd, " \t");
    args[i++] = c;

    while(1)
    {
        char* arg = strtok(NULL, " \t");
        args[i++] = arg;
        if(arg == NULL) break;
    }

    pid_t pid = fork();
    if(pid == 0)
    {
        execvp(c, args);
        exit(0);
    }

    wait(NULL);
}
#endif

/* ps aux | grep a.out 管道以后讲  */

int handle_builtin_cmd(char* cmd1)
{
    int ret = 0;
    // 是不是内置命令
    // cd
    //             cd            ../aaa bbb
    char* cmd = strdup(cmd1);
    char* c = strtok(cmd, " \t");
    if(strcmp(c, "cd") == 0)
    {
        ret = 1;
        // 实现cd
        char* path = strtok(NULL, " \t");
        chdir(path);
    }

    free(cmd);
    return ret;
}

/* ps axu > 1.txt */
/* ps aux >> 1.txt */
void handle_cmd(char* cmd)
{
    int isAppend = 0;
    if(strstr(cmd, ">>")) isAppend = 1;

    char* cmd1 = strtok(cmd, ">");
    char* filename = strtok(NULL, ">");
    // 把文件名中，可能有的空格去掉
    filename = strtok(filename, " \t");
    printf("***%s***\n", filename);

    // 处理前面的命令部分
    char* args[250];
    int i = 0;

    char* c = strtok(cmd1, " \t");
    args[i++] = c;

    while(1)
    {
        char* arg = strtok(NULL, " \t");
        args[i++] = arg;
        if(arg == NULL) break;
    }

    pid_t pid = fork();
    if(pid == 0)
    {
        if(filename)
        {
            int flag;
            if(isAppend)
                flag = O_WRONLY|O_APPEND|O_CREAT;
            else
                flag = O_WRONLY|O_TRUNC|O_CREAT;
            // 子进程要重定向，执行命令之前，把标准输出，重定向到文件
            int fd = open(filename, flag, 0777);
            dup2(fd, 1);
            close(fd);
        }
        execvp(c, args);
        exit(0); // 如果忘记了exit，表面上程序是正常执行的，其实进程数量会越来越多
    }

    wait(NULL);
}

int main()
{
    while(1)
    {
        print_tips();

        char cmd[2048];
        fgets(cmd, sizeof(cmd), stdin);
        cmd[strlen(cmd)-1] = 0;


        if(!handle_builtin_cmd(cmd)) // 先把它当成内置命令去处理
            handle_cmd(cmd);
    }
}
