
#if 0
char* mystrtok(char* str , char* d)
{
    static char* saveptr;
    if(str) // 对saveptr继续拆分
    {
        saveptr = str;
    }
        
    split(saveptr);
}
#endif

#if 0
char* mystrtok_r(char* str, char* d, char** saveptr)
{
    if(str) *saveptr = str;

    split(*saveptr);
}
#endif

void foo()
{
    
}

void sighandle(int v)
{
    foo();
    char* saveptr;
    char buf[] = "www.google.com";
    strtok_r(buf, ".", &saveptr); // www
    strtok_r(NULL, ".", &saveptr); // google
    strtok_r(NULL, ".", &saveptr); // com

}

int main()
{
    foo();
    signal(SIGINT, sighandle);
#if 1
    char buf[] = "hello world";

    strtok(buf, " "); // hello 
    // 此时被打断
    strtok(NULL, " ");  // world
#endif

#if 0

    char buf[] = "hello world";
    char* saveptr;
    strtok_r(buf, " ",  &saveptr); // hello 
    strtok_r(NULL, " ", &saveptr); // world
#endif
}
