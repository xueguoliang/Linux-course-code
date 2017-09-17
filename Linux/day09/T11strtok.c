
#include "../header.h"

void* thread_func(void* ptr)
{
    char buf[] = "aaa bbb";
    char* saveptr;
    sleep(1);
    char* a = strtok_r(buf, " ", &saveptr);
    char* b = strtok_r(NULL, " ", &saveptr); 
    printf("a=%s, b=%s\n", a, b);
}

int main()
{
    pthread_t tid;
    pthread_create(&tid, NULL, thread_func, NULL);

    char* saveptr;
    char buf[] =  "xxx yyy";
    char* x = strtok_r(buf, " ", &saveptr);  // xxx
    sleep(2);
    char* y = strtok_r(NULL, " ", &saveptr);  // yyy

    printf("x=%s, y=%s\n", x, y);

    pthread_join(tid, NULL);
}
