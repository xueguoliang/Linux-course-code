
#include "../network.h"
int main()
{
    int fd = myConnect2(9988, "127.0.0.1");
    write(fd, "hello world", 11);
}
