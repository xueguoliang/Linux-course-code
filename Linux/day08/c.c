
// 让别人成为daemon
//

int main(int argc, char* argv[])
{
    pid_t pid = fork();
    if(pid == 0)
    {
        setsid();
        chcwd();
        chroot();
        execlp(argv[1], argv[1], NULL);
        return 0;
    }
}
