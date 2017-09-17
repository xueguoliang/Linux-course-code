
#include "../header.h"

#if 0
int main()
{
    // dir中保存了一个指针
    DIR* dir = opendir(".");

    while(1)
    {
        // 每次readdir会指针会偏移
        struct dirent* entry = readdir(dir);

        if(entry == NULL) break;
        if(entry->d_name[0] == '.') continue;

        printf("%s  ", entry->d_name);
    }
    printf("\n");

    closedir(dir);
}
#endif

int main()
{
    // dir中保存了一个指针
    DIR* dir = opendir(".");
    long location;
    int index = 0;

    while(1)
    {
        // 每次readdir会指针会偏移
        struct dirent* entry = readdir(dir);

        if(entry == NULL) break;
        index ++;
        if(entry->d_name[0] == '.') continue ;
        
            location = telldir(dir);
            printf("index is %d, location=%ld, name=%s\n", index, location, entry->d_name);
        

        printf("%s  ", entry->d_name);
    }
    printf("\n");

    closedir(dir);
}
