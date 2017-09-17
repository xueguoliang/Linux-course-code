
#include "../network.h"
int main()
{
    int fd = myConnect2(9988, "127.0.0.1");

#if 0
    "hello"

        "world"
#endif

    myWriteBuf(fd, "hello");
    myWriteBuf(fd, "world");

    close(fd);

}
