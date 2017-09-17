#include "../header.h"

int main()
{
    char* str = malloc(1000);
    void* ptr = mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANON, -1, 0);

    strcpy(str, "a");
    strcpy((char*)ptr, "b");


    pid_t pid = fork();
    if(pid == 0)
    {
        strcpy(str, "hello");
        strcpy((char*)ptr, "hello fork\n");
    }
    else
    {
        sleep(1);
        printf("%s\n", str); // a? hello? 
        printf("%s\n", (char*)ptr); // b? hello fork?
    }
}

